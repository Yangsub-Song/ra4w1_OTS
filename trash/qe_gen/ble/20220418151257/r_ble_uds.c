/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name: r_ble_uds.c
 * Description : This module implements User Data Service Server.
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include "profile_cmn/r_ble_serv_common.h"
#include "r_ble_uds.h"
#include "gatt_db.h"
#include "r_ble_uds_record.h"

  /***********************************************************************************************************************
  Macro definitions
  ***********************************************************************************************************************/

  /* Version number */
#define BLE_UDS_PRV_VERSION_MAJOR                         (1)
#define BLE_UDS_PRV_VERSION_MINOR                         (0)

#define BLE_UDS_PRV_USER_CONSENT_CODE_LOWER_LIMIT_VALUE   (0x0000)
#define BLE_UDS_PRV_USER_CONSENT_CODE_UPPER_LIMIT_VALUE   (0x270F)

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
uint8_t   g_current_user_index = (uint8_t)BLE_UDS_USER_INDEX_UNKNOWN_USER;
uint16_t  g_uds_conn_hdl;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

static ble_uds_app_cb_t                gs_uds_cb;
static st_ble_uds_user_control_point_t gs_user_control_point;
static bool                            gs_user_cp_in_progress = false;

static void uds_cp_event_cb(void);

/***********************************************************************************************************************
 * Function Name: set_cli_cnfg
 * Description  : Set Characteristic value notification configuration in local GATT DB.
 * Arguments    : conn_hdl - handle to connection.
 *                attr_hdl - handle to the attribute
 *                cli_cnfg - configuration value
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t set_cli_cnfg(uint16_t conn_hdl, uint16_t attr_hdl, uint16_t cli_cnfg)
{
    uint8_t data[2];

    BT_PACK_LE_2_BYTE(data, &cli_cnfg);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = data,
        .value_len = 2,
    };

    return R_BLE_GATTS_SetAttr(conn_hdl, attr_hdl, &gatt_value);
}

/***********************************************************************************************************************
 * Function Name: get_cli_cnfg
 * Description  : Get Characteristic value notification configuration from local GATT DB.
 * Arguments    : conn_hdl - handle to connection.
 *                attr_hdl - handle to the attribute
 *                p_cli_cnfg - pointer to variable to store configuration value
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t get_cli_cnfg(uint16_t conn_hdl, uint16_t attr_hdl, uint16_t *p_cli_cnfg)
{
    ble_status_t ret;
    st_ble_gatt_value_t gatt_value;

    ret = R_BLE_GATTS_GetAttr(conn_hdl, attr_hdl, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_2_BYTE(p_cli_cnfg, gatt_value.p_value);
    }

    return ret;
}

/***********************************************************************************************************************
 * Function Name: encode_first_name
 * Description  : This function converts First Name characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the First Name  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_first_name(const st_ble_uds_first_name_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    strncpy((char*)p_gatt_value->p_value, (const char*)p_app_value->first_name, BLE_UDS_FIRST_NAME_LEN);

    /* Type case size_t to uint16_t */
    p_gatt_value->value_len = (uint16_t)(strlen((const char*)p_app_value->first_name));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_first_name
 * Description  : This function converts First Name characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the First Name value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_first_name(st_ble_uds_first_name_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    ble_status_t ret = BLE_SUCCESS;

    if (BLE_UDS_FIRST_NAME_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the first name string */
    memset(p_app_value->first_name, 0x00, BLE_UDS_FIRST_NAME_LEN);

    strncpy((char*)p_app_value->first_name, (const char*)p_gatt_value->p_value, p_gatt_value->value_len);

    return ret;

}

/***********************************************************************************************************************
 * Function Name: evt_write_req_first_name
 * Description  : This function handles the First Name characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_first_name(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_first_name_t app_value;
    st_ble_uds_record_t     *p_user_record;
    ble_status_t ret;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_first_name(&app_value, p_gatt_value);

        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update first name of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->first_name.first_name[0], (const char*)&app_value.first_name[0], sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_FIRST_NAME_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_last_name
 * Description  : This function converts Last Name characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Last Name  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_last_name(const st_ble_uds_last_name_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    strncpy((char*)p_gatt_value->p_value, (const char*)p_app_value->last_name, BLE_UDS_LAST_NAME_LEN);

    p_gatt_value->value_len = (uint16_t)(strlen((const char*)p_app_value->last_name));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_last_name
 * Description  : This function converts Last Name characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Last Name value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_last_name(st_ble_uds_last_name_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    ble_status_t ret = BLE_SUCCESS;

    if (BLE_UDS_LAST_NAME_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the last name string */
    memset(p_app_value->last_name, 0x00, BLE_UDS_LAST_NAME_LEN);

    strncpy((char*)p_app_value->last_name, (const char*)p_gatt_value->p_value, p_gatt_value->value_len);

    return ret;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_last_name
 * Description  : This function handles the Last Name characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_last_name(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_last_name_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t     *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_last_name(&app_value, p_gatt_value);
        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update last name of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->last_name.last_name[0], (const char*)&app_value.last_name[0], sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_LAST_NAME_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_email_address
 * Description  : This function converts Email Address characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Email Address  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_email_address(const st_ble_uds_email_address_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    /* Copy the email address string */
    strncpy((char*)p_gatt_value->p_value, (const char*)p_app_value->email_address, BLE_UDS_EMAIL_ADDR_LEN);

    /* Type case size_t to uint16_t */
    p_gatt_value->value_len = (uint16_t)(strlen((const char*)p_app_value->email_address));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_email_address
 * Description  : This function converts Email Address characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Email Address value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_email_address(st_ble_uds_email_address_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    ble_status_t ret = BLE_SUCCESS;

    if (BLE_UDS_EMAIL_ADDR_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the mail address string */
    memset(p_app_value->email_address, 0x00, BLE_UDS_EMAIL_ADDR_LEN);

    strncpy((char*)p_app_value->email_address, (const char*)p_gatt_value->p_value, p_gatt_value->value_len);

    return ret;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_email_address
 * Description  : This function handles the Email Address characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_email_address(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_email_address_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t     *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_email_address(&app_value, p_gatt_value);
        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update email address of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->email_address.email_address[0], (const char*)&app_value.email_address[0], sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_EMAIL_ADDR_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_age
 * Description  : This function handles the Age characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_age(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t     *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* Check the length */
        if (BLE_UDS_AGE_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update age of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->age, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_AGE_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_date_of_birth
 * Description  : This function converts Date of Birth characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Date of Birth  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_date_of_birth(const st_ble_uds_date_of_birth_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt_db byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    /* Copy the year */
    BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->year);
    pos += 2;

    /* Copy the Month */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->month);

    /* Copy the Day */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->day);

    /* Update the value_len */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_date_of_birth
 * Description  : This function converts Date of Birth characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Date of Birth value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_date_of_birth(st_ble_uds_date_of_birth_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    if (BLE_UDS_DATE_OF_BIRTH_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the app_value structure */
    memset(p_app_value, 0x00, sizeof(st_ble_uds_date_of_birth_t));

    /* Copy the year */
    BT_UNPACK_LE_2_BYTE(&(p_app_value->year), &(p_gatt_value->p_value[pos]));
    pos += 2;

    /* Copy the Month */
    BT_UNPACK_LE_1_BYTE(&p_app_value->month, &(p_gatt_value->p_value[pos++]));

    /* Copy the Day */
    BT_UNPACK_LE_1_BYTE(&p_app_value->day, &(p_gatt_value->p_value[pos++]));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_date_of_birth
 * Description  : This function handles the Date of Birth characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_date_of_birth(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_date_of_birth_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_date_of_birth(&app_value, p_gatt_value);

        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update date of birth of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->date_of_birth, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_DATE_OF_BIRTH_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_gender
 * Description  : This function handles the Gender characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_gender(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_GENDER_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update gender of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->gender, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_GENDER_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_weight
 * Description  : This function handles the Weight characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_weight(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint16_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_WEIGHT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_2_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update weight of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->weight, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_WEIGHT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_height
 * Description  : This function handles the Height characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_height(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint16_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_HEIGHT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_2_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update height of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->height, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_HEIGHT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_vo2_max
 * Description  : This function handles the VO2 Max characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_vo2_max(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_VO2_MAX_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update vo2_max of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->vo2_max, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_VO2_MAX_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_heart_rate_max
 * Description  : This function handles the Heart Rate Max characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_heart_rate_max(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_HEART_RATE_MAX_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update heart_rate_max of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->heart_rate_max, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_HEART_RATE_MAX_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_resting_heart_rate
 * Description  : This function handles the Resting Heart Rate characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_resting_heart_rate(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_RESTING_HEART_RATE_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update resting_heart_rate of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->resting_heart_rate, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_RESTING_HEART_RATE_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_maximum_recommended_heart_rate
 * Description  : This function handles the Maximum Recommended Heart Rate characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_maximum_recommended_heart_rate(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_MAX_RECOMMENDED_HEART_RATE_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update max_recommended_heart_rate of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->max_recommended_heart_rate, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_MAX_RECOMMENDED_HEART_RATE_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_aerobic_threshold
 * Description  : This function handles the Aerobic Threshold characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_aerobic_threshold(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_AEROBIC_THRESHOLD_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update aerobic_threshold of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->aerobic_threshold, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_AEROBIC_THRESHOLD_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_anaerobic_threshold
 * Description  : This function handles the Anaerobic Threshold characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_anaerobic_threshold(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_ANAEROBIC_THRESHOLD_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update anaerobic_threshold of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->anaerobic_threshold, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_ANAEROBIC_THRESHOLD_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_sport_type_for_aerobic_and_anaerobic_thresholds
 * Description  : This function handles the Sport Type for Aerobic and Anaerobic Thresholds characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_sport_type_for_aerobic_and_anaerobic_thresholds(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update sport_type_for_aerobic_and_anaerobic_thresholds of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->sport_type_for_aerobic_and_anaerobic_thresholds, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_SPORT_TYPE_FOR_THRESHOLD_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_date_of_threshold_assessment
 * Description  : This function converts Date of Threshold Assessment characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Date of Threshold Assessment  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_date_of_threshold_assessment(const st_ble_uds_date_of_threshold_assessment_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt_db byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    /* Copy the year */
    BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->year);
    pos += 2;

    /* Copy the Month */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->month);

    /* Copy the Day */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->day);

    /* Update the value_len */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_date_of_threshold_assessment
 * Description  : This function converts Date of Threshold Assessment characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Date of Threshold Assessment value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_date_of_threshold_assessment(st_ble_uds_date_of_threshold_assessment_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    if (BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the app_value structure */
    memset(p_app_value, 0x00, sizeof(st_ble_uds_date_of_threshold_assessment_t));

    /* Copy the year */
    BT_UNPACK_LE_2_BYTE(&p_app_value->year, &(p_gatt_value->p_value[pos]));
    pos += 2;

    /* Copy the Month */
    BT_UNPACK_LE_1_BYTE(&p_app_value->month, &(p_gatt_value->p_value[pos++]));

    /* Copy the Day */
    BT_UNPACK_LE_1_BYTE(&p_app_value->day, &(p_gatt_value->p_value[pos++]));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_date_of_threshold_assessment
 * Description  : This function handles the Date of Threshold Assessment characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_date_of_threshold_assessment(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_date_of_threshold_assessment_t app_value;
    st_ble_uds_record_t *p_user_record;
    ble_status_t ret;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_date_of_threshold_assessment(&app_value, p_gatt_value);
        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update date_of_threshold_assessment of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->date_of_threshold_assessment, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_DATE_OF_THRESHOLD_ASSESSMENT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_waist_circumference
 * Description  : This function handles the Waist Circumference characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_waist_circumference(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint16_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_WAIST_CIRCUMFERENCE_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_2_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update waist_circumference of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->waist_circumference, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_WAIST_CIRCUMFERENCE_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_hip_circumference
 * Description  : This function handles the Hip Circumference characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_hip_circumference(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint16_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /*  check the value_len */
        if (BLE_UDS_HIP_CIRCUMFERENCE_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_2_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update hip_circumference of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->hip_circumference, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_HIP_CIRCUMFERENCE_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_fat_burn_heart_rate_lower_limit
 * Description  : This function handles the Fat Burn Heart Rate Lower Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_fat_burn_heart_rate_lower_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_FAT_BURN_HEART_RATE_LOWER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update fat_burn_heart_rate_lower_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->fat_burn_heart_rate_lower_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_FAT_BURN_HEART_RATE_LOWER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_fat_burn_heart_rate_upper_limit
 * Description  : This function handles the Fat Burn Heart Rate Upper Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_fat_burn_heart_rate_upper_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_FAT_BURN_HEART_RATE_UPPER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update fat_burn_heart_rate_upper_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->fat_burn_heart_rate_upper_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_FAT_BURN_HEART_RATE_UPPER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_aerobic_heart_rate_lower_limit
 * Description  : This function handles the Aerobic Heart Rate Lower Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_aerobic_heart_rate_lower_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_AEROBIC_HEART_RATE_LOWER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update aerobic_heart_rate_lower_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->aerobic_heart_rate_lower_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_AEROBIC_HEART_RATE_LOWER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_aerobic_heart_rate_upper_limit
 * Description  : This function handles the Aerobic Heart Rate Upper Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_aerobic_heart_rate_upper_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_AEROBIC_HEART_RATE_UPPER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update aerobic_heart_rate_upper_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->aerobic_heart_rate_upper_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_AEROBIC_HEART_RATE_UPPER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_anaerobic_heart_rate_lower_limit
 * Description  : This function handles the Anaerobic Heart Rate Lower Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_anaerobic_heart_rate_lower_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_ANAEROBIC_HEART_RATE_LOWER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update anaerobic_heart_rate_lower_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->anaerobic_heart_rate_lower_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_LOWER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_anaerobic_heart_rate_upper_limit
 * Description  : This function handles the Anaerobic Heart Rate Upper Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_anaerobic_heart_rate_upper_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* check the value_len */
        if (BLE_UDS_ANAEROBIC_HEART_RATE_UPPER_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update anaerobic_heart_rate_upper_limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->anaerobic_heart_rate_upper_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_UPPER_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }

        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_five_zone_heart_rate_limits
 * Description  : This function converts Five Zone Heart Rate Limits characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Five Zone Heart Rate Limits  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_five_zone_heart_rate_limits(const st_ble_uds_five_zone_heart_rate_limits_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    /* Copy all the heart rate limits values */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->five_zone_heart_rate_limits___very_light___light_limit);
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->five_zone_heart_rate_limits___light___moderate_limit);
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->five_zone_heart_rate_limits___moderate___hard_limit);
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->five_zone_heart_rate_limits___hard___maximum_limit);

    /* Update the value_len byte */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_five_zone_heart_rate_limits
 * Description  : This function converts Five Zone Heart Rate Limits characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Five Zone Heart Rate Limits value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_five_zone_heart_rate_limits(st_ble_uds_five_zone_heart_rate_limits_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    if (BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the gatt_value byte array */
    memset(p_app_value, 0x00, sizeof(st_ble_uds_five_zone_heart_rate_limits_t));

    /* Copy all the heart rate limits values */
    BT_UNPACK_LE_1_BYTE(&p_app_value->five_zone_heart_rate_limits___very_light___light_limit,
        &p_gatt_value->p_value[pos++]);
    BT_UNPACK_LE_1_BYTE(&p_app_value->five_zone_heart_rate_limits___light___moderate_limit,
        &p_gatt_value->p_value[pos++]);
    BT_UNPACK_LE_1_BYTE(&p_app_value->five_zone_heart_rate_limits___moderate___hard_limit,
        &p_gatt_value->p_value[pos++]);
    BT_UNPACK_LE_1_BYTE(&p_app_value->five_zone_heart_rate_limits___hard___maximum_limit,
        &p_gatt_value->p_value[pos++]);

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_five_zone_heart_rate_limits
 * Description  : This function handles the Five Zone Heart Rate Limits characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_five_zone_heart_rate_limits(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_five_zone_heart_rate_limits_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_five_zone_heart_rate_limits(&app_value, p_gatt_value);
        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update five_zone_heart_rate_limits of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->five_zone_heart_rate_limits, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_FIVE_ZONE_HEART_RATE_LIMITS_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_three_zone_heart_rate_limits
 * Description  : This function converts Three Zone Heart Rate Limits characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Three Zone Heart Rate Limits  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_three_zone_heart_rate_limits(const st_ble_uds_three_zone_heart_rate_limits_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    /* Copy all the heart rate limits values */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++],
        &p_app_value->three_zone_heart_rate_limits___light__fat_burn____moderate__aerobic__limit);
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++],
        &p_app_value->three_zone_heart_rate_limits___moderate__aerobic____hard__anaerobic__limit);

    /* Update the value_len byte */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_three_zone_heart_rate_limits
 * Description  : This function converts Three Zone Heart Rate Limits characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Three Zone Heart Rate Limits value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_three_zone_heart_rate_limits(st_ble_uds_three_zone_heart_rate_limits_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    if (BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the gatt_value byte array */
    memset(p_app_value, 0x00, sizeof(st_ble_uds_three_zone_heart_rate_limits_t));

    /* Copy all the heart rate limits values */
    BT_UNPACK_LE_1_BYTE(&p_app_value->three_zone_heart_rate_limits___light__fat_burn____moderate__aerobic__limit,
        &p_gatt_value->p_value[pos++]);
    BT_UNPACK_LE_1_BYTE(&p_app_value->three_zone_heart_rate_limits___moderate__aerobic____hard__anaerobic__limit,
        &p_gatt_value->p_value[pos++]);

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_three_zone_heart_rate_limits
 * Description  : This function handles the Three Zone Heart Rate Limits characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_three_zone_heart_rate_limits(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_three_zone_heart_rate_limits_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        ret = decode_three_zone_heart_rate_limits(&app_value, p_gatt_value);
        if (BLE_SUCCESS == ret)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update three_zone_heart_rate_limits of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->three_zone_heart_rate_limits, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_THREE_ZONE_HEART_RATE_LIMITS_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_two_zone_heart_rate_limit
 * Description  : This function handles the Two Zone Heart Rate Limit characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_two_zone_heart_rate_limit(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint8_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* Check value_len */
        if (BLE_UDS_TWO_ZONE_HEART_RATE_LIMIT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_1_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update two zone heart rate limit of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->two_zone_heart_rate_limit, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_TWO_ZONE_HEART_RATE_LIMIT_WRITE_REQ, BLE_SUCCESS, &evt_data);
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_DB_change_increment
 * Description  : This function handles the DB Change Increment characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_DB_change_increment(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t app_value;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
    }
    else
    {
        /* Check value_len */
        if (BLE_UDS_DB_CHANGE_INCREMENT_LEN == p_gatt_value->value_len)
        {
            BT_UNPACK_LE_4_BYTE(&app_value, p_gatt_value->p_value);

            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = conn_hdl,
                .param_len = sizeof(app_value),
                .p_param = &app_value,
            };

            /* Update DB_change_increment of the current user in the local DB */
            p_user_record = uds_db_get_record(g_current_user_index);
            if (NULL != p_user_record)
            {
                strncpy((char*)&p_user_record->database_change_increment, (const char*)&app_value, sizeof(app_value));
                gs_uds_cb(BLE_UDS_EVENT_DB_CHANGE_INCREMENT_WRITE_REQ, BLE_SUCCESS, &evt_data);

                /* TODO: In multi client scenario, notify the other clients */
            }
            else
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: encode_user_control_point
 * Description  : This function converts User Control Point characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the User Control Point  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_user_control_point(const st_ble_uds_user_control_point_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt_value byte sequence */
    memset(p_gatt_value->p_value, 0x0, BLE_UDS_USER_CNTRL_PNT_LEN);

    /* Copy the Response Code */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->response_code);

    /* Copy the Request Op Code */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->opcode);

    /* Copy the Response Value */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->response_value);

    /* Copy the Response Parameter if applicable */
    if ((BLE_UDS_USER_CNTRL_PNT_OPCODE_REGISTER_NEW_USER == p_app_value->opcode) &&
        (BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_SUCCESS == p_app_value->response_value))
    {
        /* Copy the User Index for the new record */
        BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->user_index);
    }

    /* Update the length */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_user_control_point
 * Description  : This function converts User Control Point characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the User Control Point value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_user_control_point(st_ble_uds_user_control_point_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    ble_status_t ret;
    st_ble_uds_user_control_point_t temp_app_value;

    /* Check the length */
    if (BLE_UDS_USER_CNTRL_PNT_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_GATT_WRITE_REQ_REJECTED;
    }

    /* Clear the user control point app value structure */
    memset(&temp_app_value, 0x00, sizeof(st_ble_uds_user_control_point_t));

    /* Copy the opcode */
    BT_UNPACK_LE_1_BYTE(&temp_app_value.opcode, &p_gatt_value->p_value[pos++]);

    if (BLE_UDS_USER_CNTRL_PNT_OPCODE_REGISTER_NEW_USER == temp_app_value.opcode)
    {
        /* Copy the Consent Code */
        BT_UNPACK_LE_2_BYTE(&temp_app_value.consent_code, &p_gatt_value->p_value[pos]);
        pos += 2;
    }
    else if (BLE_UDS_USER_CNTRL_PNT_OPCODE_CONSENT == temp_app_value.opcode)
    {
        /* Copy the User Index and the Consent Code for the User */
        BT_UNPACK_LE_1_BYTE(&temp_app_value.user_index, &p_gatt_value->p_value[pos++]);
        BT_UNPACK_LE_2_BYTE(&temp_app_value.consent_code, &p_gatt_value->p_value[pos]);
        pos += 2;
    }
    else
    {
        /* BLE_UDS_USER_CNTRL_PNT_OPCODE_DELETE_USER_DATA or Not a valid Op Code. */
        /* Do Nothing */
    }

    /* check the value_len and pos and return appropriate value */
    if (p_gatt_value->value_len == pos)
    {
        memset(p_app_value, 0x00, sizeof(st_ble_uds_user_control_point_t));
        memcpy(p_app_value, &temp_app_value, sizeof(st_ble_uds_user_control_point_t));

        ret = BLE_SUCCESS;
    }
    else
    {
        ret = BLE_ERR_GATT_WRITE_REQ_REJECTED;
    }

    return ret;
}

/***********************************************************************************************************************
 * Function Name: uds_indicate_user_control_point
 * Description  : This function indicates User Control Point characteristic
 * Arguments    : conn_hdl - connection handle
 *                p_app_value - pointer to the application control point structure
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t uds_indicate_user_control_point(uint16_t conn_hdl, const st_ble_uds_user_control_point_t *p_app_value)
{
    ble_status_t ret;
    uint16_t cli_cnfg;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = get_cli_cnfg(conn_hdl, BLE_UDS_USER_CNTRL_PNT_CLI_CNFG_DESC_HDL, &cli_cnfg);
    if (BLE_SUCCESS != ret)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if ((cli_cnfg & BLE_GATTS_CLI_CNFG_INDICATION) == 0x0000)
    {
        return BLE_ERR_INVALID_STATE;
    }

    uint8_t byte_value[BLE_UDS_USER_CNTRL_PNT_LEN] = { 0 };

    st_ble_gatt_hdl_value_pair_t ind_data =
    {
        .attr_hdl = BLE_UDS_USER_CNTRL_PNT_VAL_HDL,
        .value.p_value = byte_value,
        .value.value_len = BLE_UDS_USER_CNTRL_PNT_LEN
    };

    ret = encode_user_control_point(p_app_value, &ind_data.value);
    ret = R_BLE_GATTS_Indication(conn_hdl, &ind_data);

    return ret;
}

/***********************************************************************************************************************
 * Function Name: uds_register_new_user
 * Description  : This function registers a new user and consent code and sends response once the procedure is complete.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void uds_register_new_user(void)
{
    uint8_t  user_index;

    if (
        #if BLE_UDS_PRV_USER_CONSENT_CODE_LOWER_LIMIT_VALUE != 0
        (gs_user_control_point.consent_code < BLE_UDS_PRV_USER_CONSENT_CODE_LOWER_LIMIT_VALUE) ||
        #endif
        (gs_user_control_point.consent_code > BLE_UDS_PRV_USER_CONSENT_CODE_UPPER_LIMIT_VALUE))
    {
        /* Send Invalid parameter response */
        gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
        gs_user_control_point.request_opcode = gs_user_control_point.opcode;
        gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_INVALID_PARAMETER;
    }
    else
    {
        /* Add new user and save the consent code for the future use */
        user_index = uds_db_add_user(gs_user_control_point.consent_code);
        if (BLE_UDS_DB_MAX_NUM_OF_RECORDS == user_index)
        {
            /* Send operation failed response */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OPERATION_FAILED;
        }
        else
        {
            /* Send Success response along with user index */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_SUCCESS;
            gs_user_control_point.user_index = user_index;
        }
    }

    /* Indicate the Response */
    uds_indicate_user_control_point(g_uds_conn_hdl, &gs_user_control_point);
}

/***********************************************************************************************************************
 * Function Name: uds_consent_procedure
 * Description  : This function checks the consent code and provides consent to access the user data for the
                  given user_index and send response once the procedure is complete.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void uds_consent_procedure(void)
{
    ble_status_t ret;

    if ((BLE_UDS_USER_INDEX_UNKNOWN_USER == gs_user_control_point.user_index) ||
        #if BLE_UDS_PRV_USER_CONSENT_CODE_LOWER_LIMIT_VALUE != 0
        (gs_user_control_point.consent_code < BLE_UDS_PRV_USER_CONSENT_CODE_LOWER_LIMIT_VALUE) ||
        #endif
        (gs_user_control_point.consent_code > BLE_UDS_PRV_USER_CONSENT_CODE_UPPER_LIMIT_VALUE))
    {
        /* Send Response with Response value = Invalid Parameter*/
        gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
        gs_user_control_point.request_opcode = gs_user_control_point.opcode;
        gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_INVALID_PARAMETER;
    }
    else
    {
        /* Check the User index and Consent code and set the current user index */
        ret = uds_db_set_current_user(gs_user_control_point.user_index, gs_user_control_point.consent_code);

        if (BLE_SUCCESS == ret)
        {
            g_current_user_index = gs_user_control_point.user_index;

            /* Set all the UDS characteristics here reading from the DB, User Index characteristic
               and DB Change Increment characteristic for the current user*/
            uds_db_populate_current_user_data(g_current_user_index);

            /* Send Response with Success */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_SUCCESS;
        }
        else if (BLE_ERR_LIMIT_EXCEEDED == ret)
        {
            /* Send Response with Operation Failed value */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OPERATION_FAILED;
        }
        else
        {
            /* Send Response with User Not Authorized value */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_USER_NOT_AUTHORIZED;
        }
    }

    /* Indicate the Response */
    uds_indicate_user_control_point(g_uds_conn_hdl, &gs_user_control_point);
    gs_user_cp_in_progress = false;
}

/***********************************************************************************************************************
 * Function Name: uds_delete_user_data
 * Description  : This function deletes the user data and the consent code of the current user and send response once
                  the user data is deleted.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void uds_delete_user_data(void)
{
    ble_status_t ret;

    /* Delete the User */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER != g_current_user_index)
    {
        ret = uds_db_delete_user(g_current_user_index);
        if (BLE_SUCCESS == ret)
        {
            /* Set User Index characteristic to UNKNOWN USER */
            g_current_user_index = BLE_UDS_USER_INDEX_UNKNOWN_USER;
            R_BLE_UDS_SetUserIndex(BLE_UDS_USER_INDEX_UNKNOWN_USER);

            /* Send SUCCESS Response */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.request_opcode = gs_user_control_point.opcode;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_SUCCESS;
        }
    }
    else
    {
        /* Send User Not Authorized Response */
        gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
        gs_user_control_point.request_opcode = gs_user_control_point.opcode;
        gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_USER_NOT_AUTHORIZED;
    }

    /* Indicate the Response */
    uds_indicate_user_control_point(g_uds_conn_hdl, &gs_user_control_point);
    gs_user_cp_in_progress = false;
}

/***********************************************************************************************************************
 * Function Name: uds_cp_event_cb
 * Description  : This function is callback to the User Control Point events.
 * Arguments    : event event id
 * Return Value : none
 **********************************************************************************************************************/
static void uds_cp_event_cb(void)
{
    /*uint8_t user_index;*/
    /*uint16_t consent_code;*/

    st_ble_uds_evt_data_t evt_data =
    {
        .conn_hdl = g_uds_conn_hdl,
        .param_len = sizeof(gs_user_control_point),
        .p_param = &gs_user_control_point,
    };

    switch (gs_user_control_point.opcode)
    {
        case BLE_UDS_USER_CNTRL_PNT_OPCODE_REGISTER_NEW_USER:
        {
            uds_register_new_user();
        }
        break;

        case BLE_UDS_USER_CNTRL_PNT_OPCODE_CONSENT:
        {
            uds_consent_procedure();
        }
        break;

        case BLE_UDS_USER_CNTRL_PNT_OPCODE_DELETE_USER_DATA:
        {
            uds_delete_user_data();
        }
        break;

        default:
        {
            /* Send response with Response value BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OP_CODE_NOT_SUPPORTED */
            gs_user_control_point.response_code = BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE;
            gs_user_control_point.response_value = BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OP_CODE_NOT_SUPPORTED;

            /* Indicate the Response */
            uds_indicate_user_control_point(g_uds_conn_hdl, &gs_user_control_point);
            gs_user_cp_in_progress = false;
        }
        break;
    }

    /* TODO: remove this if not required */
    gs_uds_cb(BLE_UDS_EVENT_USER_CNTRL_PNT_WRITE_REQ, BLE_SUCCESS, &evt_data);
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_user_control_point
 * Description  : This function handles the User Control Point characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_user_control_point(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    ble_status_t ret;
    uint16_t cli_cfg;

    if (gs_user_cp_in_progress)
    {
        /* Previous Opcode in progress, send error response */
        R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_PROC_ALREADY_IN_PROGRESS);
        return;
    }

    /* Check if Indication Enabled for the Control Point */
    get_cli_cnfg(conn_hdl, BLE_UDS_USER_CNTRL_PNT_CLI_CNFG_DESC_HDL, &cli_cfg);

    if (cli_cfg & BLE_GATTS_CLI_CNFG_INDICATION)
    {
        ret = decode_user_control_point(&gs_user_control_point, p_gatt_value);

        if (BLE_SUCCESS == ret)
        {
            g_uds_conn_hdl = conn_hdl;

            /* Set an event to execute the control point opcode on next schedule  */
            R_BLE_SetEvent(uds_cp_event_cb);

            gs_user_cp_in_progress = true;
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(ret);
        }
    }
    else
    {
        /* Indication Disabled */
        R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_CCCD_IMPROPERLY_CFG);
    }
}

/***********************************************************************************************************************
 * Function Name: encode_language
 * Description  : This function converts Language characteristic value representation in
 *                application layer (struct) to a characteristic value representation in GATT (uint8_t[]).
 * Arguments    : p_app_value - pointer to the Language  value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t encode_language(const st_ble_uds_language_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Clear the gatt_value byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    strncpy((char*)p_gatt_value->p_value, (const char*)p_app_value->language, BLE_UDS_LANGUAGE_LEN);

    /* Type case size_t to uint16_t */
    p_gatt_value->value_len = (uint16_t)(strlen((const char*)p_app_value->language));

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: decode_language
 * Description  : This function converts Language characteristic value representation in
 *                GATT (uint8_t[]) to representation in application layer (struct).
 * Arguments    : p_app_value - pointer to the Language value in the application layer
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : ble_status_t
 **********************************************************************************************************************/
static ble_status_t decode_language(st_ble_uds_language_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    ble_status_t ret = BLE_SUCCESS;

    if (BLE_UDS_LANGUAGE_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the app_value language string */
    memset(p_app_value->language, 0x00, BLE_UDS_LANGUAGE_LEN);

    strncpy((char*)p_app_value->language, (const char*)p_gatt_value->p_value, p_gatt_value->value_len);

    return ret;
}

/***********************************************************************************************************************
 * Function Name: evt_write_req_language
 * Description  : This function handles the Language characteristic write request event.
 * Arguments    : conn_hdl - connection handle
 *                p_gatt_value - pointer to the characteristic value in the GATT DB
 * Return Value : none
 **********************************************************************************************************************/
static void evt_write_req_language(uint16_t conn_hdl, st_ble_gatt_value_t *p_gatt_value)
{
    st_ble_uds_language_t app_value;
    ble_status_t ret;
    st_ble_uds_record_t *p_user_record;

    /* if no consent send error response TODO: check if anything more required when connected to multiple devices */
    if (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index)
    {
        R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
        return;
    }

    ret = decode_language(&app_value, p_gatt_value);

    if (BLE_SUCCESS == ret)
    {
        st_ble_uds_evt_data_t evt_data =
        {
            .conn_hdl = conn_hdl,
            .param_len = sizeof(app_value),
            .p_param = &app_value,
        };

        /* Update language of the current user in the local DB */
        p_user_record = uds_db_get_record(g_current_user_index);
        if (NULL != p_user_record)
        {
            strncpy((char*)&p_user_record->language.language[0], (const char*)&app_value.language[0], sizeof(app_value));
            gs_uds_cb(BLE_UDS_EVENT_LANGUAGE_WRITE_REQ, BLE_SUCCESS, &evt_data);
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
        }
    }
    else
    {
        R_BLE_GATTS_SendErrRsp(BLE_ERR_GATT_WRITE_REQ_REJECTED);
    }
}


/***********************************************************************************************************************
 * Function Name: uds_gatt_db_cb
 * Description  : Callback function for User Data GATT Server events.
 * Arguments    : conn_hdl - handle to the connection
 *                p_params - pointer to GATTS db parameter
 * Return Value : none
 **********************************************************************************************************************/
static void uds_gatts_db_cb(uint16_t conn_hdl, st_ble_gatts_db_params_t *p_params) // @suppress("Function length")
{
    switch (p_params->db_op)
    {
        case BLE_GATTS_OP_CHAR_PEER_READ_REQ:
        {
            /* User Index Characteristic Read */
            if ((BLE_UDS_USER_INDEX_VAL_HDL != p_params->attr_hdl) &&
                (BLE_UDS_USER_INDEX_UNKNOWN_USER == g_current_user_index))
            {
                R_BLE_GATTS_SendErrRsp(BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED);
            }
        }
        break;

        case BLE_GATTS_OP_CHAR_PEER_WRITE_REQ:
        {
            if (BLE_UDS_FIRST_NAME_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_first_name(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_LAST_NAME_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_last_name(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_EMAIL_ADDR_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_email_address(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_AGE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_age(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_DATE_OF_BIRTH_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_date_of_birth(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_GENDER_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_gender(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_WEIGHT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_weight(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_HEIGHT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_height(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_VO2_MAX_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_vo2_max(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_HEART_RATE_MAX_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_heart_rate_max(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_RESTING_HEART_RATE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_resting_heart_rate(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_MAX_RECOMMENDED_HEART_RATE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_maximum_recommended_heart_rate(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_AEROBIC_THRESHOLD_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_aerobic_threshold(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_ANAEROBIC_THRESHOLD_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_anaerobic_threshold(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_sport_type_for_aerobic_and_anaerobic_thresholds(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_date_of_threshold_assessment(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_WAIST_CIRCUMFERENCE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_waist_circumference(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_HIP_CIRCUMFERENCE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_hip_circumference(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_FAT_BURN_HEART_RATE_LOWER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_fat_burn_heart_rate_lower_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_FAT_BURN_HEART_RATE_UPPER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_fat_burn_heart_rate_upper_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_AEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_aerobic_heart_rate_lower_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_AEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_aerobic_heart_rate_upper_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_ANAEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_anaerobic_heart_rate_lower_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_ANAEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_anaerobic_heart_rate_upper_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_five_zone_heart_rate_limits(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_three_zone_heart_rate_limits(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_TWO_ZONE_HEART_RATE_LIMIT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_two_zone_heart_rate_limit(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_DB_CHANGE_INCREMENT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_DB_change_increment(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_USER_CNTRL_PNT_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_user_control_point(conn_hdl, &p_params->value);
            }
            else if (BLE_UDS_LANGUAGE_VAL_HDL == p_params->attr_hdl)
            {
                evt_write_req_language(conn_hdl, &p_params->value);
            }
            else
            {
                /* Do nothing. */
            }
        }
        break;

        case BLE_GATTS_OP_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            uint16_t cli_cnfg;

            st_ble_uds_evt_data_t evt_data =
            {
                    .conn_hdl = conn_hdl,
                    .param_len = 0,
                    .p_param = NULL,
            };

            BT_UNPACK_LE_2_BYTE(&cli_cnfg, p_params->value.p_value);

            if (BLE_UDS_DB_CHANGE_INCREMENT_CLI_CNFG_DESC_HDL == p_params->attr_hdl)
            {
                if (BLE_GATTS_CLI_CNFG_NOTIFICATION == cli_cnfg)
                {
                    gs_uds_cb(BLE_UDS_EVENT_DB_CHANGE_INCREMENT_CLI_CNFG_ENABLED, BLE_SUCCESS, &evt_data);
                }
                else
                {
                    gs_uds_cb(BLE_UDS_EVENT_DB_CHANGE_INCREMENT_CLI_CNFG_DISABLED, BLE_SUCCESS, &evt_data);
                }
            }
            else if (BLE_UDS_USER_CNTRL_PNT_CLI_CNFG_DESC_HDL == p_params->attr_hdl)
            {
                if (BLE_GATTS_CLI_CNFG_INDICATION == cli_cnfg)
                {
                    gs_uds_cb(BLE_UDS_EVENT_USER_CNTRL_PNT_CLI_CNFG_ENABLED, BLE_SUCCESS, &evt_data);
                }
                else
                {
                    gs_uds_cb(BLE_UDS_EVENT_USER_CNTRL_PNT_CLI_CNFG_DISABLED, BLE_SUCCESS, &evt_data);
                }
            }
            else
            {
                /* Do nothing. */
            }
        }
        break;

        default:
        {
            /* Do nothing. */
        }
        break;
    }
}

/***********************************************************************************************************************
 * Function Name: uds_gatts_cb
 * Description  : Callback function for the GATT Server events.
 * Arguments    : type - event id
 *                result - ble status
 *                p_data - pointer to the event data
 * Return Value : none
 **********************************************************************************************************************/
static void uds_gatts_cb(uint16_t type, ble_status_t result, st_ble_gatts_evt_data_t *p_data)
{
    switch (type)
    {
    case BLE_GATTS_EVENT_DB_ACCESS_IND:
    {
        /* Type cast to st_ble_gatts_db_access_evt_t* */
        st_ble_gatts_db_access_evt_t *p_db_access_evt_param = (st_ble_gatts_db_access_evt_t *)p_data->p_param;

        uds_gatts_db_cb(p_data->conn_hdl, p_db_access_evt_param->p_params);
    }
    break;

    case BLE_GATTS_EVENT_HDL_VAL_CNF:
    {
        /* Type cast to st_ble_gatts_cfm_evt_t* */
        st_ble_gatts_cfm_evt_t *p_cfm_evt_param = (st_ble_gatts_cfm_evt_t *)p_data->p_param;

        if (BLE_UDS_USER_CNTRL_PNT_VAL_HDL == p_cfm_evt_param->attr_hdl)
        {
            st_ble_uds_evt_data_t evt_data =
            {
                .conn_hdl = p_data->conn_hdl,
                .param_len = 0,
                .p_param = NULL,
            };

            gs_uds_cb(BLE_UDS_EVENT_USER_CNTRL_PNT_HDL_VAL_CNF, result, &evt_data);

            gs_user_cp_in_progress = false;
        }
    }
    break;

    default:
    {
        /* Do nothing. */
    }
    break;
    }
}

ble_status_t R_BLE_UDS_Init(const st_ble_uds_init_param_t *p_param) // @suppress("API function naming")
{
    if (NULL == p_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    if (NULL == p_param->cb)
    {
        return BLE_ERR_INVALID_ARG;
    }

    R_BLE_GATTS_RegisterCb(uds_gatts_cb, 3);

    gs_uds_cb = p_param->cb;

    return BLE_SUCCESS;
}

ble_status_t R_BLE_UDS_Connect(uint16_t conn_hdl, const st_ble_uds_connect_param_t *p_param) // @suppress("API function naming")
{
    if (BLE_GAP_INVALID_CONN_HDL == conn_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if (NULL != p_param)
    {
        set_cli_cnfg(conn_hdl, BLE_UDS_DB_CHANGE_INCREMENT_CLI_CNFG_DESC_HDL, p_param->database_change_increment_cli_cnfg);
        set_cli_cnfg(conn_hdl, BLE_UDS_USER_CNTRL_PNT_CLI_CNFG_DESC_HDL, p_param->user_control_point_cli_cnfg);

        /* Set the current user index to to UNKNOWN */
        g_current_user_index = BLE_UDS_USER_INDEX_UNKNOWN_USER;
    }

    return BLE_SUCCESS;
}

ble_status_t R_BLE_UDS_Disconnect(uint16_t conn_hdl, st_ble_uds_disconnect_param_t *p_param) // @suppress("API function naming")
{
    if (BLE_GAP_INVALID_CONN_HDL == conn_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if (NULL != p_param)
    {
        get_cli_cnfg(conn_hdl, BLE_UDS_DB_CHANGE_INCREMENT_CLI_CNFG_DESC_HDL, &p_param->database_change_increment_cli_cnfg);
        get_cli_cnfg(conn_hdl, BLE_UDS_USER_CNTRL_PNT_CLI_CNFG_DESC_HDL, &p_param->user_control_point_cli_cnfg);
        /* set gs_current_user to UNKNOWN. */
        g_current_user_index = BLE_UDS_USER_INDEX_UNKNOWN_USER;
    }

    return BLE_SUCCESS;
}

ble_status_t R_BLE_UDS_GetFirstName(st_ble_uds_first_name_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FIRST_NAME_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_first_name(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetFirstName(const st_ble_uds_first_name_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_FIRST_NAME_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_FIRST_NAME_LEN,
    };

    ret = encode_first_name(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FIRST_NAME_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetLastName(st_ble_uds_last_name_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_LAST_NAME_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_last_name(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetLastName(const st_ble_uds_last_name_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_LAST_NAME_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_LAST_NAME_LEN,
    };

    ret = encode_last_name(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_LAST_NAME_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetEmailAddress(st_ble_uds_email_address_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_EMAIL_ADDR_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_email_address(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetEmailAddress(const st_ble_uds_email_address_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_EMAIL_ADDR_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_EMAIL_ADDR_LEN,
    };

    ret = encode_email_address(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_EMAIL_ADDR_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetAge(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AGE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAge(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_AGE_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AGE_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetDateOfBirth(st_ble_uds_date_of_birth_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DATE_OF_BIRTH_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_date_of_birth(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetDateOfBirth(const st_ble_uds_date_of_birth_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_DATE_OF_BIRTH_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_DATE_OF_BIRTH_LEN,
    };

    ret = encode_date_of_birth(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DATE_OF_BIRTH_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetGender(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_GENDER_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetGender(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_GENDER_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_GENDER_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetWeight(uint16_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_WEIGHT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_2_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetWeight(uint16_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_WEIGHT_LEN] = { 0 };

    BT_PACK_LE_2_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_WEIGHT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetHeight(uint16_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HEIGHT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_2_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetHeight(uint16_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_HEIGHT_LEN] = { 0 };

    BT_PACK_LE_2_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HEIGHT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetVo2Max(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_VO2_MAX_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetVo2Max(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_VO2_MAX_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_VO2_MAX_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetHeartRateMax(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HEART_RATE_MAX_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetHeartRateMax(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_HEART_RATE_MAX_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HEART_RATE_MAX_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetRestingHeartRate(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_RESTING_HEART_RATE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetRestingHeartRate(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_RESTING_HEART_RATE_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_RESTING_HEART_RATE_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetMaximumRecommendedHeartRate(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_MAX_RECOMMENDED_HEART_RATE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetMaximumRecommendedHeartRate(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_MAX_RECOMMENDED_HEART_RATE_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_MAX_RECOMMENDED_HEART_RATE_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAerobicThreshold(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_THRESHOLD_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAerobicThreshold(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_AEROBIC_THRESHOLD_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_THRESHOLD_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAnaerobicThreshold(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_THRESHOLD_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAnaerobicThreshold(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_ANAEROBIC_THRESHOLD_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_THRESHOLD_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetSportTypeForAerobicAndAnaerobicThresholds(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetSportTypeForAerobicAndAnaerobicThresholds(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetDateOfThresholdAssessment(st_ble_uds_date_of_threshold_assessment_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_date_of_threshold_assessment(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetDateOfThresholdAssessment(const st_ble_uds_date_of_threshold_assessment_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_LEN,
    };

    ret = encode_date_of_threshold_assessment(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetWaistCircumference(uint16_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_WAIST_CIRCUMFERENCE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_2_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetWaistCircumference(uint16_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_WAIST_CIRCUMFERENCE_LEN] = { 0 };

    BT_PACK_LE_2_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_WAIST_CIRCUMFERENCE_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetHipCircumference(uint16_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HIP_CIRCUMFERENCE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_2_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetHipCircumference(uint16_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_HIP_CIRCUMFERENCE_LEN] = { 0 };

    BT_PACK_LE_2_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_HIP_CIRCUMFERENCE_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetFatBurnHeartRateLowerLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FAT_BURN_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetFatBurnHeartRateLowerLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_FAT_BURN_HEART_RATE_LOWER_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FAT_BURN_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetFatBurnHeartRateUpperLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FAT_BURN_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetFatBurnHeartRateUpperLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_FAT_BURN_HEART_RATE_UPPER_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FAT_BURN_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAerobicHeartRateLowerLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAerobicHeartRateLowerLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_AEROBIC_HEART_RATE_LOWER_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAerobicHeartRateUpperLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAerobicHeartRateUpperLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_AEROBIC_HEART_RATE_UPPER_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_AEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAnaerobicHeartRateLowerLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAnaerobicHeartRateLowerLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_ANAEROBIC_HEART_RATE_LOWER_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_HEART_RATE_LOWER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetAnaerobicHeartRateUpperLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetAnaerobicHeartRateUpperLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_ANAEROBIC_HEART_RATE_UPPER_LIMIT_LEN] = { 0 };


    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_ANAEROBIC_HEART_RATE_UPPER_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetFiveZoneHeartRateLimits(st_ble_uds_five_zone_heart_rate_limits_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_five_zone_heart_rate_limits(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetFiveZoneHeartRateLimits(const st_ble_uds_five_zone_heart_rate_limits_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_LEN,
    };

    ret = encode_five_zone_heart_rate_limits(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_FIVE_ZONE_HEART_RATE_LIMITS_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetThreeZoneHeartRateLimits(st_ble_uds_three_zone_heart_rate_limits_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_three_zone_heart_rate_limits(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetThreeZoneHeartRateLimits(const st_ble_uds_three_zone_heart_rate_limits_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_LEN,
    };

    ret = encode_three_zone_heart_rate_limits(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_THREE_ZONE_HEART_RATE_LIMITS_VAL_HDL, &gatt_value);

    return ret;
}

ble_status_t R_BLE_UDS_GetTwoZoneHeartRateLimit(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_TWO_ZONE_HEART_RATE_LIMIT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetTwoZoneHeartRateLimit(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_TWO_ZONE_HEART_RATE_LIMIT_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_TWO_ZONE_HEART_RATE_LIMIT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetDatabaseChangeIncrement(uint32_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DB_CHANGE_INCREMENT_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_4_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetDatabaseChangeIncrement(uint32_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_DB_CHANGE_INCREMENT_LEN] = { 0 };

    BT_PACK_LE_4_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_DB_CHANGE_INCREMENT_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_NotifyDatabaseChangeIncrement(uint16_t conn_hdl, uint32_t app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint16_t cli_cnfg;

    ret = get_cli_cnfg(conn_hdl, BLE_UDS_DB_CHANGE_INCREMENT_CLI_CNFG_DESC_HDL, &cli_cnfg);

    if (BLE_SUCCESS != ret)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if ((cli_cnfg & BLE_GATTS_CLI_CNFG_NOTIFICATION) == 0x0000)
    {
        return BLE_ERR_INVALID_STATE;
    }

    uint8_t byte_value[BLE_UDS_DB_CHANGE_INCREMENT_LEN] = { 0 };

    BT_PACK_LE_4_BYTE(byte_value, &app_value);

    st_ble_gatt_hdl_value_pair_t ntf_data =
    {
        .attr_hdl = BLE_UDS_DB_CHANGE_INCREMENT_VAL_HDL,
        .value.p_value = byte_value,
        .value.value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_Notification(conn_hdl, &ntf_data);
}

ble_status_t R_BLE_UDS_GetUserIndex(uint8_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_USER_INDEX_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        BT_UNPACK_LE_1_BYTE(p_app_value, gatt_value.p_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetUserIndex(uint8_t app_value) // @suppress("API function naming")
{
    uint8_t byte_value[BLE_UDS_USER_INDEX_LEN] = { 0 };

    BT_PACK_LE_1_BYTE(byte_value, &app_value);

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = sizeof(app_value),
    };

    return R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_USER_INDEX_VAL_HDL, &gatt_value);
}

ble_status_t R_BLE_UDS_GetLanguage(st_ble_uds_language_t *p_app_value) // @suppress("API function naming")
{
    st_ble_gatt_value_t gatt_value;
    ble_status_t ret;

    if (NULL == p_app_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    ret = R_BLE_GATTS_GetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_LANGUAGE_VAL_HDL, &gatt_value);

    if (BLE_SUCCESS == ret)
    {
        ret = decode_language(p_app_value, &gatt_value);
    }

    return ret;
}

ble_status_t R_BLE_UDS_SetLanguage(const st_ble_uds_language_t *p_app_value) // @suppress("API function naming")
{
    ble_status_t ret;
    uint8_t byte_value[BLE_UDS_LANGUAGE_LEN] = { 0 };

    st_ble_gatt_value_t gatt_value =
    {
        .p_value = byte_value,
        .value_len = BLE_UDS_LANGUAGE_LEN,
    };

    ret = encode_language(p_app_value, &gatt_value);
    ret = R_BLE_GATTS_SetAttr(BLE_GAP_INVALID_CONN_HDL, BLE_UDS_LANGUAGE_VAL_HDL, &gatt_value);
    
    return ret;
}

uint32_t R_BLE_UDS_GetVersion(void) // @suppress("API function naming")
{
    uint32_t version;

    version = ((BLE_UDS_PRV_VERSION_MAJOR << 16) | (BLE_UDS_PRV_VERSION_MINOR << 8));

    return version;
}
