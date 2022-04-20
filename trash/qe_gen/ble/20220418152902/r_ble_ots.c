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
 * File Name   : r_ble_ots.c
 * Description : The source file for Object Transfer Service Server.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include "r_ble_ots.h"
#include "r_ble_ots_record.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef UNUSED_ARG
#define UNUSED_ARG(arg)         (void)(arg)
#endif /* UNUSED_ARG */

st_ble_ots_object_t  *gp_current_object = NULL;
st_ble_ots_uuid_t g_obj_type;
uint8_t g_obj_name[BLE_OTS_OBJ_NAME_LEN - 2];

static ble_status_t ots_send_obj_list_ctrl_pt_resp(uint16_t conn_hdl, uint8_t op_code, uint8_t req_op_code, uint8_t resp_code, uint32_t param);
static ble_status_t ots_send_object_action_ctrl_pt_resp(uint16_t conn_hdl, uint8_t op_code, uint8_t req_op_code, uint8_t resp_code, uint16_t param);

/*static uint16_t      gs_index;*/
static uint16_t      gs_conn_hdl;
static bool          gs_is_olcp_in_progress;
static bool          gs_is_oacp_in_progress;
/*static uint16_t      gs_current_obj_idx;*/

static bool gs_write_long_obj_filter_0_flag = false;
static bool gs_write_long_obj_filter_1_flag = false;
static bool gs_write_long_obj_filter_2_flag = false;

/* Set Object Name */
static char obj_name[BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN];
static st_ble_seq_data_t object_name_string = {
    .data = (uint8_t*)obj_name,
    .len = BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN,
};


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief OACP Create Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_CREATE_OP_CODE_SUPPORTED                                                       (1  << 0)

/*******************************************************************************************************************//**
 * @brief OACP Delete Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_DELETE_OP_CODE_SUPPORTED                                                       (1 << 1)

/*******************************************************************************************************************//**
 * @brief OACP Calculate Checksum Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_CALCULATE_CHECKSUM_OP_CODE_SUPPORTED                                           (1 << 2)

/*******************************************************************************************************************//**
 * @brief OACP Execute Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_EXECUTE_OP_CODE_SUPPORTED                                                      (1 << 3)

/*******************************************************************************************************************//**
 * @brief OACP Read Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_READ_OP_CODE_SUPPORTED                                                         (1 << 4)

/*******************************************************************************************************************//**
 * @brief OACP Write Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_WRITE_OP_CODE_SUPPORTED                                                        (1 << 5)

/*******************************************************************************************************************//**
 * @brief Appending Additional Data to Objects Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_APPENDING_ADDITIONAL_DATA_TO_OBJECTS_SUPPORTED                                      (1 << 6)

/*******************************************************************************************************************//**
 * @brief Truncation of Objects Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_TRUNCATION_OF_OBJECTS_SUPPORTED                                                     (1 << 7)

/*******************************************************************************************************************//**
 * @brief Patching of Objects Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_PATCHING_OF_OBJECTS_SUPPORTED                                                       (1 << 8)

/*******************************************************************************************************************//**
 * @brief OACP Abort Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OACP_ABORT_OP_CODE_SUPPORTED                                                        (1 << 9)

/*******************************************************************************************************************//**
 * @brief OLCP Go To Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OLCP_GO_TO_OP_CODE_SUPPORTED                                                        (1 << 10)

/*******************************************************************************************************************//**
 * @brief OLCP Order Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OLCP_ORDER_OP_CODE_SUPPORTED                                                        (1 << 11)

/*******************************************************************************************************************//**
 * @brief OLCP Request Number of Objects Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OLCP_REQUEST_NUMBER_OF_OBJECTS_OP_CODE_SUPPORTED                                    (1 << 12)

/*******************************************************************************************************************//**
 * @brief OLCP Clear Marking Op Code Supported bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_OLCP_CLEAR_MARKING_OP_CODE_SUPPORTED                                                (1 << 13)

/*******************************************************************************************************************//**
 * @brief Deletion of this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_DELETION_OF_THIS_OBJECT_IS_PERMITTED                                                (1 << 0)

/*******************************************************************************************************************//**
 * @brief Execution of this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_EXECUTION_OF_THIS_OBJECT_IS_PERMITTED                                               (1 << 1)

/*******************************************************************************************************************//**
 * @brief Reading this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_READING_THIS_OBJECT_IS_PERMITTED                                                    (1 << 2)

/*******************************************************************************************************************//**
 * @brief Writing data to this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_WRITING_DATA_TO_THIS_OBJECT_IS_PERMITTED                                            (1 << 3)

/*******************************************************************************************************************//**
 * @brief Appending data to this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_APPENDING_DATA_THAT_INCREASES_OBJ_ALLOCATED_SIZE_IS_PERMITTED                       (1 << 4)

/*******************************************************************************************************************//**
 * @brief Truncation of this object is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_TRUNCATION_OF_THIS_OBJECT_IS_PERMITTED                                              (1 << 5)

/*******************************************************************************************************************//**
 * @brief Patching this object by overwriting is permitted bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_PATCHING_BY_OVERWRITING_OBJECT_EXISTING_CONTENTS_IS_PERMITTED                       (1 << 6)

/*******************************************************************************************************************//**
 * @brief This object is a marked object bit
***********************************************************************************************************************/
#define BLE_OTS_PRV_THIS_OBJECT_IS_A_MARKED_OBJECT                                                      (1 << 7)

/*******************************************************************************************************************//**
 * @brief Source of Change
***********************************************************************************************************************/
#define BLE_OTS_PRV_SOURCE_OF_CHANGE                                                                    (1 << 0)

/*******************************************************************************************************************//**
 * @brief Change occurred to the object contents
***********************************************************************************************************************/
#define BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_CONTENTS                                              (1 << 1)

/*******************************************************************************************************************//**
 * @brief Change occurred to the object metadata
***********************************************************************************************************************/
#define BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_METADATA                                              (1 << 2)

/*******************************************************************************************************************//**
 * @brief Object Creation
***********************************************************************************************************************/
#define BLE_OTS_PRV_OBJECT_CREATION                                                                     (1 << 3)

/*******************************************************************************************************************//**
 * @brief Object Deletion
***********************************************************************************************************************/
#define BLE_OTS_PRV_OBJECT_DELETION                                                                     (1 << 4)


static st_ble_servs_info_t gs_servs_info;

/*----------------------------------------------------------------------------------------------------------------------
    OTS Feature characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_feat_t(st_ble_ots_feat_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert byte sequence to app data. */
    uint32_t ots_oacp_features = 0;
    uint32_t ots_olcp_features = 0;

    if (BLE_OTS_FEAT_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the feature bit flags */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_feat_t));

    BT_UNPACK_LE_4_BYTE(&ots_oacp_features, &p_gatt_value->p_value[0]);
    BT_UNPACK_LE_4_BYTE(&ots_olcp_features, &p_gatt_value->p_value[4]);

    /* Create Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_CREATE_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_create_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_create_op_code_supported = false;
    }

    /* Delete Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_DELETE_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_delete_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_delete_op_code_supported = false;
    }

    /* OACP Calculate Checksum Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_CALCULATE_CHECKSUM_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_calculate_checksum_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_calculate_checksum_op_code_supported = false;
    }

    /* OACP Execute Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_EXECUTE_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_execute_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_execute_op_code_supported = false;
    }

    /* OACP Read Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_READ_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_read_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_read_op_code_supported = false;
    }

    /* OACP Write Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_WRITE_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_write_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_write_op_code_supported = false;
    }

    /* Appending Additional Data to Objects Supported */
    if (ots_oacp_features & BLE_OTS_PRV_APPENDING_ADDITIONAL_DATA_TO_OBJECTS_SUPPORTED)
    {
        p_app_value->oacp_features.is_appending_additional_data_to_objects_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_appending_additional_data_to_objects_supported = false;
    }

    /* Truncation of Objects Supported */
    if (ots_oacp_features & BLE_OTS_PRV_TRUNCATION_OF_OBJECTS_SUPPORTED)
    {
        p_app_value->oacp_features.is_truncation_of_objects_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_truncation_of_objects_supported = false;
    }

    /* Patching of Objects Supported */
    if (ots_oacp_features & BLE_OTS_PRV_PATCHING_OF_OBJECTS_SUPPORTED)
    {
        p_app_value->oacp_features.is_patching_of_objects_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_patching_of_objects_supported = false;
    }

    /* OACP Abort Op Code Supported */
    if (ots_oacp_features & BLE_OTS_PRV_OACP_ABORT_OP_CODE_SUPPORTED)
    {
        p_app_value->oacp_features.is_oacp_abort_op_code_supported = true;
    }
    else
    {
        p_app_value->oacp_features.is_oacp_abort_op_code_supported = false;
    }

    /* OLCP Go To Op Code Supported */
    if (ots_olcp_features & BLE_OTS_PRV_OLCP_GO_TO_OP_CODE_SUPPORTED)
    {
        p_app_value->olcp_features.is_olcp_go_to_op_code_supported = true;
    }
    else
    {
        p_app_value->olcp_features.is_olcp_go_to_op_code_supported = false;
    }

    /* OLCP Order Op Code Supported */
    if (ots_olcp_features & BLE_OTS_PRV_OLCP_ORDER_OP_CODE_SUPPORTED)
    {
        p_app_value->olcp_features.is_olcp_order_op_code_supported = true;
    }
    else
    {
        p_app_value->olcp_features.is_olcp_order_op_code_supported = false;
    }

    /* OLCP Request Number of Objects Op Code Supported */
    if (ots_olcp_features & BLE_OTS_PRV_OLCP_REQUEST_NUMBER_OF_OBJECTS_OP_CODE_SUPPORTED)
    {
        p_app_value->olcp_features.is_olcp_request_number_of_objects_op_code_supported = true;
    }
    else
    {
        p_app_value->olcp_features.is_olcp_request_number_of_objects_op_code_supported = false;
    }

    /* OLCP Clear Marking Op Code Supported */
    if (ots_olcp_features & BLE_OTS_PRV_OLCP_CLEAR_MARKING_OP_CODE_SUPPORTED)
    {
        p_app_value->olcp_features.is_olcp_clear_marking_op_code_supported = true;
    }
    else
    {
        p_app_value->olcp_features.is_olcp_clear_marking_op_code_supported = false;
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_feat_t(const st_ble_ots_feat_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos                = 0;
    uint32_t oacp_features_flag = 0;
    uint32_t olcp_features_flag = 0;

    /* Clear the byte array */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_FEAT_LEN);

    /* Create Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_create_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_CREATE_OP_CODE_SUPPORTED;
    }

    /* OACP Delete Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_delete_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_DELETE_OP_CODE_SUPPORTED;
    }

    /* OACP Calculate Checksum Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_calculate_checksum_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_CALCULATE_CHECKSUM_OP_CODE_SUPPORTED;
    }

    /* OACP Execute Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_execute_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_EXECUTE_OP_CODE_SUPPORTED;
    }

    /* OACP Read Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_read_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_READ_OP_CODE_SUPPORTED;
    }

    /* OACP Write Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_write_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_WRITE_OP_CODE_SUPPORTED;
    }

    /* Appending Additional Data to Objects Supported */
    if (p_app_value->oacp_features.is_appending_additional_data_to_objects_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_APPENDING_ADDITIONAL_DATA_TO_OBJECTS_SUPPORTED;
    }

    /* Truncation of Objects Supported */
    if (p_app_value->oacp_features.is_truncation_of_objects_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_TRUNCATION_OF_OBJECTS_SUPPORTED;
    }

    /* Patching of Objects Supported */
    if (p_app_value->oacp_features.is_patching_of_objects_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_PATCHING_OF_OBJECTS_SUPPORTED;
    }

    /* OACP Abort Op Code Supported */
    if (p_app_value->oacp_features.is_oacp_abort_op_code_supported)
    {
        /* Set the bit */
        oacp_features_flag |= BLE_OTS_PRV_OACP_ABORT_OP_CODE_SUPPORTED;
    }

    /* OLCP Go To Op Code Supported */
    if (p_app_value->olcp_features.is_olcp_go_to_op_code_supported)
    {
        /* Set the bit */
        olcp_features_flag |= BLE_OTS_PRV_OLCP_GO_TO_OP_CODE_SUPPORTED;
    }

    /* OLCP Order Op Code Supported */
    if (p_app_value->olcp_features.is_olcp_order_op_code_supported)
    {
        /* Set the bit */
        olcp_features_flag |= BLE_OTS_PRV_OLCP_ORDER_OP_CODE_SUPPORTED;
    }

    /* OLCP Request Number of Objects Op Code Supported */
    if (p_app_value->olcp_features.is_olcp_request_number_of_objects_op_code_supported)
    {
        /* Set the bit */
        olcp_features_flag |= BLE_OTS_PRV_OLCP_REQUEST_NUMBER_OF_OBJECTS_OP_CODE_SUPPORTED;
    }

    /* OLCP Clear Marking Op Code Supported */
    if (p_app_value->olcp_features.is_olcp_clear_marking_op_code_supported)
    {
        /* Set the bit */
        olcp_features_flag |= BLE_OTS_PRV_OLCP_CLEAR_MARKING_OP_CODE_SUPPORTED;
    }

    BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &oacp_features_flag);
    pos += (sizeof(oacp_features_flag));

    BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &olcp_features_flag);
    pos += (sizeof(olcp_features_flag));

    p_gatt_value->value_len = BLE_OTS_FEAT_LEN;

    return BLE_SUCCESS;
}

/* OTS Feature characteristic definition */
static const st_ble_servs_char_info_t gs_feat_char = {
    .start_hdl    = BLE_OTS_FEAT_DECL_HDL,
    .end_hdl      = BLE_OTS_FEAT_VAL_HDL,
    .char_idx     = BLE_OTS_FEAT_IDX,
    .app_size     = sizeof(st_ble_ots_feat_t),
    .db_size      = BLE_OTS_FEAT_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_feat_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_feat_t,
};

ble_status_t R_BLE_OTS_SetFeat(const st_ble_ots_feat_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_feat_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetFeat(st_ble_ots_feat_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_feat_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Name characteristic
----------------------------------------------------------------------------------------------------------------------*/

static void write_req_obj_name(const void *p_attr, uint16_t conn_hdl, ble_status_t result, st_ble_seq_data_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    if (gp_current_object)
    {
        if (strncmp((const char*)gp_current_object->obj_name, (const char*)p_app_value->data, 8) == 0)
        {

            R_BLE_GATTS_SendErrRsp(BLE_OTS_OBJECT_NAME_ALREADY_EXISTS_ERROR);
            return;
        }
        else
        {
            R_BLE_GATTS_SendErrRsp(BLE_OTS_OBJECT_NOT_SELECTED_ERROR);
            return;
        }
    }
}

/* Object Name characteristic definition */
static const st_ble_servs_char_info_t gs_obj_name_char = {
    .start_hdl    = BLE_OTS_OBJ_NAME_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_NAME_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_NAME_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_OTS_OBJ_NAME_LEN,
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_obj_name,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_OTS_SetObjName(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_name_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjName(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_name_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Type characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_uuid_t(st_ble_ots_uuid_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert byte sequence to app data. */
    uint32_t pos = 0;
    uint8_t  i   = 0;

    if (BLE_OTS_OBJ_TYPE_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_uuid_t));

    if (2 == p_gatt_value->value_len)
    {
        p_app_value->uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;

        BT_UNPACK_LE_2_BYTE(&(p_app_value->uuid.uuid_16), &(p_gatt_value->p_value[pos]));
        pos += 2;
    }
    else
    {
        p_app_value->uuid_type = BLE_GATT_128_BIT_UUID_FORMAT;

        for (i = 0; i < 16; i++)
        {
            p_app_value->uuid.uuid_128[i] = p_gatt_value->p_value[pos++];
        }
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_uuid_t(const st_ble_ots_uuid_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    /* Clear the byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    if (BLE_GATT_16_BIT_UUID_FORMAT == p_app_value->uuid_type)
    {
        p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->uuid.uuid_16 & 0xFF;
        p_gatt_value->p_value[pos++] = (uint8_t)((p_app_value->uuid.uuid_16 >> 8) & 0xFF);
    }
    else //if (BLE_GATT_128_BIT_UUID_FORMAT == p_app_value->uuid_type)
    {
        for (i = 0; i < 16; i++)
        {
            p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->uuid.uuid_128[i];
        }
    }

    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Object Type characteristic definition */
static const st_ble_servs_char_info_t gs_obj_type_char = {
    .start_hdl    = BLE_OTS_OBJ_TYPE_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_TYPE_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_TYPE_IDX,
    .app_size     = sizeof(st_ble_ots_uuid_t),
    .db_size      = BLE_OTS_OBJ_TYPE_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_uuid_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_uuid_t,
};

ble_status_t R_BLE_OTS_SetObjType(const st_ble_ots_uuid_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_type_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjType(st_ble_ots_uuid_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_type_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Size characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_size_t(st_ble_ots_obj_size_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
     /* Convert byte sequence to app data. */
    uint32_t pos = 0;

    if (BLE_OTS_OBJ_SIZE_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_size_t));

    BT_UNPACK_LE_4_BYTE(&(p_app_value->current_size), &(p_gatt_value->p_value[pos]));
    pos += (sizeof(p_app_value->current_size));

    BT_UNPACK_LE_4_BYTE(&(p_app_value->allocated_size), &(p_gatt_value->p_value[pos]));
    pos += (sizeof(p_app_value->allocated_size));
    
    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_obj_size_t(const st_ble_ots_obj_size_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert app data to byte sequence. */
    uint32_t  pos = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_OBJ_SIZE_LEN);

    BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->current_size));
    pos += (sizeof(p_app_value->current_size));

    BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->allocated_size));
    pos += (sizeof(p_app_value->allocated_size));

    /* Update Length */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Object Size characteristic definition */
static const st_ble_servs_char_info_t gs_obj_size_char = {
    .start_hdl    = BLE_OTS_OBJ_SIZE_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_SIZE_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_SIZE_IDX,
    .app_size     = sizeof(st_ble_ots_obj_size_t),
    .db_size      = BLE_OTS_OBJ_SIZE_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_size_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_size_t,
};

ble_status_t R_BLE_OTS_SetObjSize(const st_ble_ots_obj_size_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_size_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjSize(st_ble_ots_obj_size_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_size_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object First-Created characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_date_time_t(st_ble_date_time_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert byte sequence to app data. */
    uint32_t pos = 0;

    if (BLE_OTS_OBJ_FIRST_CREATED_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_date_time_t));

    BT_UNPACK_LE_2_BYTE(&(p_app_value->year), &(p_gatt_value->p_value[pos]));
    pos += 2;

    BT_UNPACK_LE_1_BYTE(&(p_app_value->month), &(p_gatt_value->p_value[pos++]));
    BT_UNPACK_LE_1_BYTE(&(p_app_value->day), &(p_gatt_value->p_value[pos++]));
    BT_UNPACK_LE_1_BYTE(&(p_app_value->hours), &(p_gatt_value->p_value[pos++]));
    BT_UNPACK_LE_1_BYTE(&(p_app_value->minutes), &(p_gatt_value->p_value[pos++]));
    BT_UNPACK_LE_1_BYTE(&(p_app_value->seconds), &(p_gatt_value->p_value[pos++]));

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_date_time_t(const st_ble_date_time_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x00, BLE_OTS_OBJ_FIRST_CREATED_LEN);

    BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->year));
    pos += (sizeof(p_app_value->year));

    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->month));
    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->day));
    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->hours));
    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->minutes));
    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->seconds));

    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Object First-Created characteristic definition */
static const st_ble_servs_char_info_t gs_obj_first_created_char = {
    .start_hdl    = BLE_OTS_OBJ_FIRST_CREATED_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_FIRST_CREATED_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_FIRST_CREATED_IDX,
    .app_size     = sizeof(st_ble_date_time_t),
    .db_size      = BLE_OTS_OBJ_FIRST_CREATED_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_date_time_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_date_time_t,
};

ble_status_t R_BLE_OTS_SetObjFirstCreated(const st_ble_date_time_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_first_created_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjFirstCreated(st_ble_date_time_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_first_created_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Last-Modified characteristic
----------------------------------------------------------------------------------------------------------------------*/
/* Object Last-Modified characteristic definition */
static const st_ble_servs_char_info_t gs_obj_last_modified_char = {
    .start_hdl    = BLE_OTS_OBJ_LAST_MODIFIED_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_LAST_MODIFIED_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_LAST_MODIFIED_IDX,
    .app_size     = sizeof(st_ble_date_time_t),
    .db_size      = BLE_OTS_OBJ_LAST_MODIFIED_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_date_time_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_date_time_t,
};

ble_status_t R_BLE_OTS_SetObjLastModified(const st_ble_date_time_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_last_modified_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjLastModified(st_ble_date_time_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_last_modified_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object ID characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Object ID characteristic definition */
static const st_ble_servs_char_info_t gs_obj_id_char = {
    .start_hdl    = BLE_OTS_OBJ_ID_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_ID_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_ID_IDX,
    .app_size     = sizeof(uint8_t) * 6,
    .db_size      = BLE_OTS_OBJ_ID_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_allcopy,
    .encode       = (ble_servs_attr_encode_t)encode_allcopy,
};

ble_status_t R_BLE_OTS_SetObjId(const uint8_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_id_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjId(uint8_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_id_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Properties characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_prop_t(st_ble_ots_obj_prop_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert byte sequence to app data. */
    uint32_t pos                    = 0;
    uint32_t ots_object_properties  = 0;
    /*uint16_t ret;*/

    if (BLE_OTS_OBJ_PROP_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_prop_t));

    BT_UNPACK_LE_4_BYTE(&ots_object_properties, &p_gatt_value->p_value[pos]);
    pos += 4;

    /* Deletion of this object */
    if (ots_object_properties & BLE_OTS_PRV_DELETION_OF_THIS_OBJECT_IS_PERMITTED)
    {
        p_app_value->is_deletion_of_this_object_is_permitted = true;
    }
    else
    {
        p_app_value->is_deletion_of_this_object_is_permitted = false;
    }

    /* Execution of this object */
    if (ots_object_properties & BLE_OTS_PRV_EXECUTION_OF_THIS_OBJECT_IS_PERMITTED)
    {
        p_app_value->is_execution_of_this_object_is_permitted = true;
    }
    else
    {
        p_app_value->is_execution_of_this_object_is_permitted = false;
    }

    /* Reading this object */
    if (ots_object_properties & BLE_OTS_PRV_READING_THIS_OBJECT_IS_PERMITTED)
    {
        p_app_value->is_reading_this_object_is_permitted = true;
    }
    else
    {
        p_app_value->is_reading_this_object_is_permitted = false;
    }

    /* Writing data to this object */
    if (ots_object_properties & BLE_OTS_PRV_WRITING_DATA_TO_THIS_OBJECT_IS_PERMITTED)
    {
        p_app_value->is_writing_data_to_this_object_is_permitted = true;
    }
    else
    {
        p_app_value->is_writing_data_to_this_object_is_permitted = false;
    }

    /* Appending data to this object */
    if (ots_object_properties & BLE_OTS_PRV_APPENDING_DATA_THAT_INCREASES_OBJ_ALLOCATED_SIZE_IS_PERMITTED)
    {
        p_app_value->is_appending_data_to_this_object_that_increases_its_allocated_size_is_permitted = true;
    }
    else
    {
        p_app_value->is_appending_data_to_this_object_that_increases_its_allocated_size_is_permitted = false;
    }

    /* Truncation of this object */
    if (ots_object_properties & BLE_OTS_PRV_TRUNCATION_OF_THIS_OBJECT_IS_PERMITTED)
    {
        p_app_value->is_truncation_of_this_object_is_permitted = true;
    }
    else
    {
        p_app_value->is_truncation_of_this_object_is_permitted = false;
    }

    /* Patching this object by overwriting */
    if (ots_object_properties & BLE_OTS_PRV_PATCHING_BY_OVERWRITING_OBJECT_EXISTING_CONTENTS_IS_PERMITTED)
    {
        p_app_value->is_patching_this_object_by_overwriting_some_of_the_object_s_existing_contents_is_permitted = true;
    }
    else
    {
        p_app_value->is_patching_this_object_by_overwriting_some_of_the_object_s_existing_contents_is_permitted = false;
    }

    /* object is a marked */
    if (ots_object_properties & BLE_OTS_PRV_THIS_OBJECT_IS_A_MARKED_OBJECT)
    {
        p_app_value->is_this_object_is_a_marked_object = true;
    }
    else
    {
        p_app_value->is_this_object_is_a_marked_object = false;
    }

    if (ots_object_properties > BLE_OTS_PRV_THIS_OBJECT_IS_A_MARKED_OBJECT)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return BLE_ERR_INVALID_DATA;
    }
    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_obj_prop_t(const st_ble_ots_obj_prop_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert app data to byte sequence. */
    uint32_t pos                    = 0;
    uint32_t ots_object_properties  = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_OBJ_PROP_LEN);


    /* Deletion of this object */
    if (p_app_value->is_deletion_of_this_object_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_DELETION_OF_THIS_OBJECT_IS_PERMITTED;
    }

    /* Execution of this object */
    if (p_app_value->is_execution_of_this_object_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_EXECUTION_OF_THIS_OBJECT_IS_PERMITTED;
    }

    /* Reading this object */
    if (p_app_value->is_reading_this_object_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_READING_THIS_OBJECT_IS_PERMITTED;
    }

    /* Writing data to this object */
    if (p_app_value->is_writing_data_to_this_object_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_WRITING_DATA_TO_THIS_OBJECT_IS_PERMITTED;
    }

    /* Appending data to this object */
    if (p_app_value->is_appending_data_to_this_object_that_increases_its_allocated_size_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_APPENDING_DATA_THAT_INCREASES_OBJ_ALLOCATED_SIZE_IS_PERMITTED;
    }

    /* Truncation of this object */
    if (p_app_value->is_truncation_of_this_object_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_TRUNCATION_OF_THIS_OBJECT_IS_PERMITTED;
    }

    /* Patching this object by overwriting */
    if (p_app_value->is_patching_this_object_by_overwriting_some_of_the_object_s_existing_contents_is_permitted)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_PATCHING_BY_OVERWRITING_OBJECT_EXISTING_CONTENTS_IS_PERMITTED;
    }

    /* object is a marked */
    if (p_app_value->is_this_object_is_a_marked_object)
    {
        /* Set the bit */
        ots_object_properties |= BLE_OTS_PRV_THIS_OBJECT_IS_A_MARKED_OBJECT;
    }

    BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &ots_object_properties);

    /* Update Length */
    p_gatt_value->value_len = BLE_OTS_OBJ_PROP_LEN;
    return BLE_SUCCESS;
}

/* Object Properties characteristic definition */
static const st_ble_servs_char_info_t gs_obj_prop_char = {
    .start_hdl    = BLE_OTS_OBJ_PROP_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_PROP_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_PROP_IDX,
    .app_size     = sizeof(st_ble_ots_obj_prop_t),
    .db_size      = BLE_OTS_OBJ_PROP_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_prop_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_prop_t,
};

ble_status_t R_BLE_OTS_SetObjProp(const st_ble_ots_obj_prop_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_prop_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjProp(st_ble_ots_obj_prop_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_prop_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Action Control Point Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_obj_action_cp_cli_cnfg = {
    .attr_hdl = BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_IDX,
    .db_size  = BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_OTS_SetObjActionCpCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_obj_action_cp_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjActionCpCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_obj_action_cp_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Action Control Point characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_action_cp_t(st_ble_ots_obj_action_cp_t *p_app_value, 
                                                                    const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    /*uint8_t i   = 0;*/
    /*uint8_t value_len;*/

    if (BLE_OTS_OBJ_ACTION_CP_LEN <= p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear app_value structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_action_cp_t));

    BT_UNPACK_LE_1_BYTE(&p_app_value->op_code, &p_gatt_value->p_value[pos]);
    pos += 1;

    if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_CREATE == p_app_value->op_code)
    {
        BT_UNPACK_LE_4_BYTE(&p_app_value->size, &p_gatt_value->p_value[pos]);
        pos += 4;

        if (p_gatt_value->value_len - pos == 2)
        {
            p_app_value->type.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            BT_UNPACK_LE_2_BYTE(&p_app_value->type.uuid.uuid_16, &(p_gatt_value->p_value[pos]));
            pos += 2;
        }
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_DELETE == p_app_value->op_code)
    {
        /* Do Nothing */
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_CALCULATE_CHECKSUM == p_app_value->op_code)
    {
        BT_UNPACK_LE_4_BYTE(&p_app_value->offset, &p_gatt_value->p_value[pos]);
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&p_app_value->length, &p_gatt_value->p_value[pos]);
        pos += 4;
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_EXECUTE == p_app_value->op_code)
    {
        /* Do Nothing */
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_READ == p_app_value->op_code)
    {
        BT_UNPACK_LE_4_BYTE(&p_app_value->offset, &p_gatt_value->p_value[pos]);
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&p_app_value->length, &p_gatt_value->p_value[pos]);
        pos += 4;
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_WRITE == p_app_value->op_code)
    {
        BT_UNPACK_LE_4_BYTE(&p_app_value->offset, &p_gatt_value->p_value[pos]);
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&p_app_value->length, &p_gatt_value->p_value[pos]);
        pos += 4;

        BT_UNPACK_LE_1_BYTE(&p_app_value->mode, &p_gatt_value->p_value[pos]);
        pos += 1;
    }
    else if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_ABORT == p_app_value->op_code)
    {
        /* Do Nothing */
    }
    else
    {
        /* Do Nothing */
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_obj_action_cp_t(const st_ble_ots_obj_action_cp_t *p_app_value, 
                                                                        st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert app data to byte sequence. */
    uint32_t pos = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_OBJ_ACTION_CP_LEN);

    /* opcode */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->op_code);

    /* request opcode */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->request_op_code);

    /* result code */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->result_code);

    /* response parameter */
    if ((BLE_OTS_OBJ_ACTION_CP_OP_CODE_CALCULATE_CHECKSUM == p_app_value->request_op_code)
        && (BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS == p_app_value->result_code))
    {
        BT_PACK_LE_4_BYTE(&p_gatt_value->p_value[pos], &p_app_value->response_parameter);
        pos += 4;
    }

    /* Update Length */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Object Action Control Point characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_obj_action_cp_descs[] = {
    &gs_obj_action_cp_cli_cnfg,
};

/***********************************************************************************************************************
 * Function Name: write_req_obj_action_ctrl_pt
 * Description  : This function handles write request for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl - Connection handle value
                  p_attr - Pointer to the attribute value
                  result - Result of type ble_status_t
 * Return Value : None
 **********************************************************************************************************************/
static void write_req_obj_action_ctrl_pt(const void *p_attr, uint16_t conn_hdl, 
                                            ble_status_t result, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(result);
    UNUSED_ARG(p_app_value);

    if (gs_is_oacp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }

    uint16_t cli_cnfg;
    R_BLE_OTS_GetObjActionCpCliCnfg(conn_hdl, &cli_cnfg);
    if (BLE_GATTS_CLI_CNFG_INDICATION != cli_cnfg)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_CLI_CNFG_IMPROPERLY_CONFIGURED_ERROR);
        return;
    }
}

/***********************************************************************************************************************
 * Function Name: ots_create_new_object
 * Description  : This function creates new object for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_create_new_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    st_ble_ots_object_t *p_object   = NULL;
    uint16_t             object_idx = 0;
    st_ble_ots_obj_size_t object_size = { 0 };
    
    object_size.current_size = 0;
    object_size.allocated_size = p_app_value->size;
    ots_db_create_object(&object_size, &p_app_value->type);
    object_idx = ots_db_get_newest_index();

    p_object = ots_db_get_object(object_idx);

    /* p_object is never NULL since ots_db_create_object always creates a new object or overwrites the 
     oldest object. */
    /*if (NULL == p_object)
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT,
            0);
    }
    */

    p_object->obj_size.allocated_size = p_app_value->size;
    p_object->obj_type.uuid_type = p_app_value->type.uuid_type;
    p_object->obj_type.uuid.uuid_16 = p_app_value->type.uuid.uuid_16;
    gp_current_object = p_object;

    st_ble_ots_obj_prop_t obj_prop;
    g_obj_type.uuid.uuid_16 = p_app_value->type.uuid.uuid_16;
    g_obj_type.uuid_type    = BLE_GATT_16_BIT_UUID_FORMAT;

    R_BLE_OTS_SetObjType(&p_app_value->type);

    obj_prop.is_writing_data_to_this_object_is_permitted = true;

    R_BLE_OTS_SetObjProp(&obj_prop);

    if (g_obj_type.uuid.uuid_16 != 0x7FB1)
    {
        g_obj_type.uuid.uuid_16 = 0x7FB1;
        g_obj_type.uuid_type    = BLE_GATT_16_BIT_UUID_FORMAT;
        R_BLE_OTS_SetObjType(&g_obj_type);

        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
                                            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                            p_app_value->op_code,
                                            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_UNSUPPORTED_TYPE,
                                            0);
    }
    else
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_delete_object
 * Description  : This function deletes object for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_delete_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*st_ble_ots_obj_prop_t obj_prop;*/
    st_ble_ots_object_t *p_object = NULL;
    uint16_t index = ots_db_get_oldest_index();
    p_object = ots_db_get_object(index);
    
    if ((NULL != p_object) && (BLE_OTS_DB_VALID_OBJECT == p_object->valid))
    {
        ots_db_delete_object(p_object);
        ots_db_delete_object(p_object);

        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS,
            0);
    }
    else
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: e2e_crc_calculation
 * Description  : This function handles CRC calculation for OACP.
 * Arguments    : offset  - Pointer to offset value
 *                length  - Length value
 * Return Value : uint32_t - CRC value
 **********************************************************************************************************************/
static uint32_t e2e_crc_calculation(uint32_t* offset, uint32_t length)
{
    uint32_t i = 0;
    uint32_t crc = 0xFFFFFFFF;

    while (length--)
    {
        crc ^= ((uint32_t)*offset++ << 8);

        for (i = 0; i < 8; ++i)
        {
            crc = (crc << 1) ^ ((crc & 0x8000U) ? 0x1021U : 0U);
        }
    }
    return(crc & 0xFFFFFFFF);
}

/***********************************************************************************************************************
 * Function Name: ots_calculate_chksum
 * Description  : This function handles Checksum calculation for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_calculate_chksum(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint32_t crc = 0;
    crc = e2e_crc_calculation(&p_app_value->offset, p_app_value->length);
    
    ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS,
            (uint16_t)crc);
}

/***********************************************************************************************************************
 * Function Name: ots_execute_object
 * Description  : This function handles Execute opcode for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_execute_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint16_t             index = 0;
    st_ble_ots_object_t *ots_object;

    ots_object = ots_db_get_object(index);

    if (NULL == ots_object)
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_read_object
 * Description  : This function handles Read opcode for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_read_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*uint8_t index;*/
    /*int16_t obj_offset = 0;*/
    /*int16_t obj_length = 0;*/
    st_ble_ots_object_t *ots_object;

    /*
    obj_offset = p_app_value->offset;
    obj_length = p_app_value->length;
    */

    ots_object = gp_current_object;

    if (NULL == ots_object)
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT,
            0);
    }
    else
    {
        /* TODO: Check the L2CAP channel number which is open */
        /*ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_CHANNEL_UNAVAILABLE,
            0);*/
       /* ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS,
            0); */
    }
}

/***********************************************************************************************************************
 * Function Name: ots_write_to_object
 * Description  : This function handles Write opcode for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_write_to_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*uint8_t             index;*/
    st_ble_ots_object_t *p_ots_object;

    p_ots_object = gp_current_object;

    if (NULL == p_ots_object)
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT,
            0);
    }
    else
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_CHANNEL_UNAVAILABLE,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_abort_object
 * Description  : This function handles Abort opcode for OACP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_abort_object(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*uint8_t index;*/
    /*st_ble_ots_object_t *p_ots_object;*/

    /*p_ots_object = gp_current_object;*/

    if (gs_is_oacp_in_progress)
    {
        ots_send_object_action_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OPERATION_FAILED,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_oacp_process_operation
 * Description  : This function is call back for characteristic process operation.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
                  started      - Boolean variable for operation status
 * Return Value : None
 **********************************************************************************************************************/
static void ots_oacp_process_operation(uint16_t conn_hdl, st_ble_ots_obj_action_cp_t *p_app_value, bool started)
{
    UNUSED_ARG(started);

    st_ble_ots_feat_t     features_supported;
    st_ble_ots_obj_prop_t object_properties;

    /* Read the Features characteristic value */
    R_BLE_OTS_GetFeat(&features_supported);
    R_BLE_OTS_GetObjProp(&object_properties);

    /* Unsupported Op Code */
    if ((BLE_OTS_OBJ_ACTION_CP_OP_CODE_CREATE > p_app_value->op_code) ||
        (BLE_OTS_OBJ_ACTION_CP_OP_CODE_ABORT < p_app_value->op_code))
    {
        ots_send_object_action_ctrl_pt_resp(conn_hdl,
                                            BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                            p_app_value->op_code,
                                            BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OP_CODE_NOT_SUPPORTED,
                                            0);
        return;

    }

    if ((BLE_OTS_OBJ_ACTION_CP_OP_CODE_CREATE == p_app_value->op_code) && 
        (features_supported.oacp_features.is_oacp_create_op_code_supported))
    {
        ots_create_new_object(conn_hdl, p_app_value);
    }

    if(BLE_OTS_OBJ_ACTION_CP_OP_CODE_DELETE == p_app_value->op_code)
    {
        if(!(object_properties.is_deletion_of_this_object_is_permitted))
        {
            ots_send_object_action_ctrl_pt_resp(conn_hdl,
                                                BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                                p_app_value->op_code,
                                                BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_PROCEDURE_NOT_PERMITTED,
                                                0);
            return;
        }
        else if(features_supported.oacp_features.is_oacp_delete_op_code_supported)
        {
            ots_delete_object(conn_hdl, p_app_value);
        }
    }

    if ((BLE_OTS_OBJ_ACTION_CP_OP_CODE_CALCULATE_CHECKSUM == p_app_value->op_code) && 
        features_supported.oacp_features.is_oacp_calculate_checksum_op_code_supported)
    {
        ots_calculate_chksum(conn_hdl, p_app_value);
    }

    if(BLE_OTS_OBJ_ACTION_CP_OP_CODE_EXECUTE == p_app_value->op_code)
    {
        if (!(object_properties.is_execution_of_this_object_is_permitted))
        {
            ots_send_object_action_ctrl_pt_resp(conn_hdl,
                                               BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                               p_app_value->op_code,
                                               BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_PROCEDURE_NOT_PERMITTED,
                                               0);
            return;
        }
        else if(features_supported.oacp_features.is_oacp_execute_op_code_supported)
        {
            ots_execute_object(conn_hdl, p_app_value);
        }
    }

    if(BLE_OTS_OBJ_ACTION_CP_OP_CODE_READ == p_app_value->op_code)
    {
        if(!(object_properties.is_reading_this_object_is_permitted))
        {
            ots_send_object_action_ctrl_pt_resp(conn_hdl,
                                                BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                                p_app_value->op_code,
                                                BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_PROCEDURE_NOT_PERMITTED,
                                                0);
            return;
        }
        else if(features_supported.oacp_features.is_oacp_read_op_code_supported)
        {
            ots_read_object(conn_hdl, p_app_value);
        }
    }

    if(BLE_OTS_OBJ_ACTION_CP_OP_CODE_WRITE == p_app_value->op_code)
    {
        if(!(object_properties.is_writing_data_to_this_object_is_permitted))
        {
            ots_send_object_action_ctrl_pt_resp(conn_hdl,
                                                BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE,
                                                p_app_value->op_code,
                                                BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_PROCEDURE_NOT_PERMITTED,
                                                0);
            return;
        }
        else if(features_supported.oacp_features.is_oacp_write_op_code_supported)
        {
            ots_write_to_object(conn_hdl, p_app_value);
        }
    }

    if ((BLE_OTS_OBJ_ACTION_CP_OP_CODE_ABORT == p_app_value->op_code) && 
        features_supported.oacp_features.is_oacp_abort_op_code_supported)
    {
        ots_abort_object(conn_hdl, p_app_value);
    }
    else
    {
        /* Nothing */
    }
}

/***********************************************************************************************************************
 * Function Name: write_comp_obj_action_ctrl_pt
 * Description  : This function is call back for characteristic write complete.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
                  p_attr       - Attribute value
 *                conn_hdl     - Connection handle value
                  result      -  Result value of type ble_status_t
 * Return Value : None
 **********************************************************************************************************************/
static void write_comp_obj_action_ctrl_pt(const void *p_attr,
                                          uint16_t conn_hdl,
                                          ble_status_t result,
                                          st_ble_ots_obj_action_cp_t *p_app_value)
{
    UNUSED_ARG(result);

    gs_is_oacp_in_progress = true;
    gs_conn_hdl            = conn_hdl;
    ots_oacp_process_operation(conn_hdl, p_app_value, true);

    st_ble_servs_char_info_t p_char = *(st_ble_servs_char_info_t *)p_attr;

    st_ble_servs_evt_data_t evt_data = {
        .conn_hdl = conn_hdl,
        .param_len = sizeof(st_ble_ots_obj_action_cp_t),
        .p_param = p_app_value,
    };

    gs_servs_info.cb(BLE_SERVS_MULTI_ATTR_EVENT(p_char.char_idx, p_char.inst_idx, BLE_SERVS_WRITE_COMP), BLE_SUCCESS, &evt_data);
}

/***********************************************************************************************************************
 * Function Name: flow_ctrl_obj_action_ctrl_pt
 * Description  : This function  handles characteristic flow control callback.
 * Arguments    : p_attr - Attribute value
 * Return Value : None
 **********************************************************************************************************************/
static void flow_ctrl_obj_action_ctrl_pt(const void *p_attr)
{
    if (gs_is_oacp_in_progress)
    {
        st_ble_servs_char_info_t   *p_char_attr = (st_ble_servs_char_info_t *)p_attr;
        st_ble_ots_obj_action_cp_t app_value;
        st_ble_gatt_value_t        gatt_value;
        R_BLE_GATTS_GetAttr(gs_conn_hdl, (uint16_t)(p_char_attr->start_hdl + 1), &gatt_value);
        decode_st_ble_ots_obj_action_cp_t(&app_value, &gatt_value);
        ots_oacp_process_operation(gs_conn_hdl, &app_value, false);
    }
}

/* Object Action Control Point characteristic definition */
static const st_ble_servs_char_info_t gs_obj_action_cp_char = {
    .start_hdl     = BLE_OTS_OBJ_ACTION_CP_DECL_HDL,
    .end_hdl       = BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_DESC_HDL,
    .char_idx      = BLE_OTS_OBJ_ACTION_CP_IDX,
    .app_size      = sizeof(st_ble_ots_obj_action_cp_t),
    .db_size       = BLE_OTS_OBJ_ACTION_CP_LEN,
    .write_req_cb  = (ble_servs_attr_write_req_t)write_req_obj_action_ctrl_pt,
    .write_comp_cb = (ble_servs_attr_write_comp_t)write_comp_obj_action_ctrl_pt,
    .flow_ctrl_cb  = (ble_servs_attr_flow_ctrl_t)flow_ctrl_obj_action_ctrl_pt,
    .decode        = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_action_cp_t,
    .encode        = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_action_cp_t,
    .pp_descs      = gspp_obj_action_cp_descs,
    .num_of_descs = ARRAY_SIZE(gspp_obj_action_cp_descs),
};

ble_status_t R_BLE_OTS_IndicateObjActionCp(uint16_t conn_hdl, const st_ble_ots_obj_action_cp_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_obj_action_cp_char, conn_hdl, (const void *)p_value, false);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object List Control Point Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_obj_list_cp_cli_cnfg = {
    .attr_hdl = BLE_OTS_OBJ_LIST_CP_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_OTS_OBJ_LIST_CP_CLI_CNFG_IDX,
    .db_size  = BLE_OTS_OBJ_LIST_CP_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_OTS_SetObjListCpCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_obj_list_cp_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjListCpCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_obj_list_cp_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object List Control Point characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_list_cp_t(st_ble_ots_obj_list_cp_t *p_app_value, 
                                                                const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t i = 0;

    if (BLE_OTS_OBJ_LIST_CP_LEN <= p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }
    
    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_list_cp_t));

    BT_UNPACK_LE_1_BYTE(&p_app_value->op_code, &p_gatt_value->p_value[pos]);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_CP_OP_CODE_GO_TO == p_app_value->op_code)
    {
        for (i = 0; i < 6; i++)
        {
            p_app_value->parameter[i] = p_gatt_value->p_value[pos++];
        }
    }
    else if (BLE_OTS_OBJ_LIST_CP_OP_CODE_ORDER == p_app_value->op_code)
    {
        BT_UNPACK_LE_1_BYTE(&p_app_value->list_sort_order, &p_gatt_value->p_value[pos]);
        pos += 1;
    }
    else
    {
        /* Do Nothing */
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_obj_list_cp_t(const st_ble_ots_obj_list_cp_t *p_app_value, 
                                                                        st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert app data to byte sequence. */
    uint32_t pos = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_OBJ_LIST_CP_LEN);

    /* opcode */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->op_code);

    /* request opcode */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->request_op_code);

    /* result code */
    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos++], &p_app_value->result_code);

    /* response parameter */
    if (BLE_OTS_OBJ_LIST_CP_OP_CODE_REQUEST_NUMBER_OF_OBJECTS == p_app_value->request_op_code)
    {
        /* response parameter - Total Number of objects */
        BT_PACK_LE_4_BYTE(&p_gatt_value->p_value[pos], &p_app_value->response_parameter);
        pos += 4;
    }

    /* Update Length */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Object List Control Point characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_obj_list_cp_descs[] = {
    &gs_obj_list_cp_cli_cnfg,
};

/***********************************************************************************************************************
 * Function Name: write_req_obj_list_ctrl_pt
 * Description  : This function handles write request for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl - Connection handle value
                  p_attr - Pointer to the attribute value
                  result - Result of type ble_status_t
 * Return Value : None
 **********************************************************************************************************************/
static void write_req_obj_list_ctrl_pt(const void *p_attr, uint16_t conn_hdl,
                                        ble_status_t result,st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(result);
    UNUSED_ARG(p_app_value);

    uint16_t cli_cnfg;

    if (gs_is_olcp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }

    R_BLE_OTS_GetObjListCpCliCnfg(conn_hdl, &cli_cnfg);
    if (BLE_GATTS_CLI_CNFG_INDICATION != cli_cnfg)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_CLI_CNFG_IMPROPERLY_CONFIGURED_ERROR);
        return;
    }
}

/***********************************************************************************************************************
 * Function Name: ots_make_first_obj_curr_obj
 * Description  : This function handles First opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_make_first_obj_curr_obj(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint16_t            object_idx = 0;
    st_ble_ots_object_t *p_object;

    object_idx = ots_db_get_oldest_index();
    p_object   = ots_db_get_object(object_idx);

    if ((NULL != p_object) && p_object->valid)
    {
        strncpy((char*)object_name_string.data, (const char*)p_object->obj_name, BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN);
        R_BLE_OTS_SetObjName(&object_name_string);

        R_BLE_OTS_SetObjId(p_object->obj_id);

        R_BLE_OTS_SetObjType(&p_object->obj_type);

        R_BLE_OTS_SetObjSize(&p_object->obj_size);

        gp_current_object = p_object;

        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
            0);
    }
    else
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_NO_OBJECT,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_make_last_obj_curr_obj
 * Description  : This function handles First opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_make_last_obj_curr_obj(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint16_t            object_idx;
    st_ble_ots_object_t *p_object;

    object_idx = ots_db_get_newest_index();
    p_object   = ots_db_get_object(object_idx);

    if (NULL != p_object)
    {
        gp_current_object = p_object;

        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
            0);
    }
    else
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_NO_OBJECT,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_make_prev_obj_curr_obj
 * Description  : This function handles Previous opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_make_prev_obj_curr_obj(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint16_t            object_idx;
    st_ble_ots_object_t *p_object;

    object_idx = gp_current_object->prev_index;
    p_object   = ots_db_get_object(object_idx);

    if (NULL != p_object)
    {
        gp_current_object = p_object;

        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
            0);
    }
    else
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OUT_OF_BOUNDS,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_make_next_obj_curr_obj
 * Description  : This function handles Next opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_make_next_obj_curr_obj(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint16_t            object_idx;
    st_ble_ots_object_t *p_object;

    object_idx = gp_current_object->next_index;
    p_object   = ots_db_get_object(object_idx);

    if ((NULL != p_object) && (p_object->valid))
    {
        gp_current_object = p_object;

        strncpy((char*)object_name_string.data, (const char*)p_object->obj_name, BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN);
        R_BLE_OTS_SetObjName(&object_name_string);

        R_BLE_OTS_SetObjId(p_object->obj_id);
        R_BLE_OTS_SetObjType(&p_object->obj_type);
        R_BLE_OTS_SetObjSize(&p_object->obj_size);

        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
        BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
        p_app_value->op_code,
        BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
        0);
    }
    else
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OUT_OF_BOUNDS,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_make_obj_current_obj
 * Description  : This function handles Current opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_make_obj_current_obj(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    st_ble_ots_object_t *p_object = NULL;

    p_object = ots_db_get_object_id(p_app_value);
    if (NULL != p_object)
    {
        gp_current_object = p_object;

        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
            0);
    }
    else
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OBJECT_ID_NOT_FOUND,
            0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_order_objects
 * Description  : This function handles Sort opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_order_objects(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*uint8_t             index;*/
    /*st_ble_ots_object_t *p_ots_object;*/

    /*p_ots_object = gp_current_object;*/

    if ((BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_NAME__ASCENDING > p_app_value->list_sort_order) ||
        (BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_LAST_MODIFIED_TIMESTAMP__DESCENDING < p_app_value->list_sort_order))
    {
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_INVALID_PARAMETER,
            0);
    }
    else
    {
        ots_sort_object_names();
        ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
                                       BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
                                       p_app_value->op_code,
                                       BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
                                       0);
    }
}

/***********************************************************************************************************************
 * Function Name: ots_req_num_of_objects
 * Description  : This function handles Number of Objects opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_req_num_of_objects(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    uint32_t num_of_objects = 0;

    num_of_objects = ots_db_num_of_objects();

    ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
        BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
        p_app_value->op_code,
        (0 == num_of_objects) ? BLE_OTS_OBJ_LIST_CP_RESULT_CODE_NO_OBJECT : BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
        num_of_objects);
}

/***********************************************************************************************************************
 * Function Name: ots_clear_marking
 * Description  : This function handles Clear Marking opcode for OLCP.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
 * Return Value : None
 **********************************************************************************************************************/
static void ots_clear_marking(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(conn_hdl);

    /*uint16_t object_idx = ots_db_get_oldest_index();*/

    ots_db_clear_marking();

    ots_send_obj_list_ctrl_pt_resp(gs_conn_hdl,
        BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
        p_app_value->op_code,
        BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS,
        0);
}

/***********************************************************************************************************************
 * Function Name: ots_olcp_process_operation
 * Description  : This function handles characteristic process operation callback.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
 *                conn_hdl     - Connection handle value
                  started      - Boolean variable for operation status
 * Return Value : None
 **********************************************************************************************************************/
static void ots_olcp_process_operation(uint16_t conn_hdl, st_ble_ots_obj_list_cp_t *p_app_value, bool started)
{
    UNUSED_ARG(started);

    st_ble_ots_feat_t features_supported;
    
    /* Read the features */
    R_BLE_OTS_GetFeat(&features_supported);

    /* Unsupported Op Code */
    if ((BLE_OTS_OBJ_LIST_CP_OP_CODE_FIRST > p_app_value->op_code) ||
        (BLE_OTS_OBJ_LIST_CP_OP_CODE_CLEAR_MARKING < p_app_value->op_code))
    {
        ots_send_obj_list_ctrl_pt_resp(conn_hdl,
            BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE,
            p_app_value->op_code,
            BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OP_CODE_NOT_SUPPORTED,
            0);
        return;
    }

    if (BLE_OTS_OBJ_LIST_CP_OP_CODE_FIRST == p_app_value->op_code)
    {
        ots_make_first_obj_curr_obj(conn_hdl, p_app_value);
    }
    else if (BLE_OTS_OBJ_LIST_CP_OP_CODE_LAST == p_app_value->op_code)
    {
        ots_make_last_obj_curr_obj(conn_hdl, p_app_value);
    }
    else if (BLE_OTS_OBJ_LIST_CP_OP_CODE_PREVIOUS == p_app_value->op_code)
    {
        ots_make_prev_obj_curr_obj(conn_hdl, p_app_value);
    }
    else if (BLE_OTS_OBJ_LIST_CP_OP_CODE_NEXT == p_app_value->op_code)
    {
        ots_make_next_obj_curr_obj(conn_hdl, p_app_value);
    }
    else if ((BLE_OTS_OBJ_LIST_CP_OP_CODE_GO_TO == p_app_value->op_code) &&
                            features_supported.olcp_features.is_olcp_go_to_op_code_supported)
    {
        ots_make_obj_current_obj(conn_hdl, p_app_value);
    }
    else if ((BLE_OTS_OBJ_LIST_CP_OP_CODE_ORDER == p_app_value->op_code) &&
                            features_supported.olcp_features.is_olcp_order_op_code_supported)
    {
        ots_order_objects(conn_hdl, p_app_value);
    }
    else if ((BLE_OTS_OBJ_LIST_CP_OP_CODE_REQUEST_NUMBER_OF_OBJECTS == p_app_value->op_code) &&
                            features_supported.olcp_features.is_olcp_request_number_of_objects_op_code_supported)
    {
        ots_req_num_of_objects(conn_hdl, p_app_value);
    }
    else if ((BLE_OTS_OBJ_LIST_CP_OP_CODE_CLEAR_MARKING == p_app_value->op_code) &&
                            features_supported.olcp_features.is_olcp_clear_marking_op_code_supported)
    {
        ots_clear_marking(conn_hdl, p_app_value);
    }
    else
    {
        /* Nothing */
    }
}

/***********************************************************************************************************************
 * Function Name: write_comp_obj_list_ctrl_pt
 * Description  : This function is call back for characteristic write complete.
 * Arguments    : p_app_value  - Pointer to the control point value in the application layer
                  p_attr       - Attribute value
 *                conn_hdl     - Connection handle value
                  result      -  Result value of type ble_status_t
 * Return Value : None
 **********************************************************************************************************************/
static void write_comp_obj_list_ctrl_pt (const void *p_attr, uint16_t conn_hdl, ble_status_t result,
        st_ble_ots_obj_list_cp_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(result);

    gs_is_olcp_in_progress = true;
    gs_conn_hdl            = conn_hdl;
    ots_olcp_process_operation(conn_hdl, p_app_value, true);
}

/***********************************************************************************************************************
 * Function Name: flow_ctrl_obj_list_ctrl_pt
 * Description  : This function  handles characteristic flow control callback.
 * Arguments    : p_attr - Attribute value
 * Return Value : None
 **********************************************************************************************************************/
static void flow_ctrl_obj_list_ctrl_pt(const void *p_attr)
{
    if (gs_is_olcp_in_progress)
    {
        st_ble_servs_char_info_t *p_char_attr = (st_ble_servs_char_info_t *)p_attr;
        st_ble_ots_obj_list_cp_t app_value;
        st_ble_gatt_value_t gatt_value;
        R_BLE_GATTS_GetAttr(gs_conn_hdl, (uint16_t)(p_char_attr->start_hdl + 1), &gatt_value);
        decode_st_ble_ots_obj_list_cp_t(&app_value, &gatt_value);
        ots_olcp_process_operation(gs_conn_hdl, &app_value, false);
    }
}

/* Object List Control Point characteristic definition */
static const st_ble_servs_char_info_t gs_obj_list_cp_char = {
    .start_hdl     = BLE_OTS_OBJ_LIST_CP_DECL_HDL,
    .end_hdl       = BLE_OTS_OBJ_LIST_CP_CLI_CNFG_DESC_HDL,
    .char_idx      = BLE_OTS_OBJ_LIST_CP_IDX,
    .app_size      = sizeof(st_ble_ots_obj_list_cp_t),
    .db_size       = BLE_OTS_OBJ_LIST_CP_LEN,
    .write_req_cb  = (ble_servs_attr_write_req_t)write_req_obj_list_ctrl_pt,
    .write_comp_cb = (ble_servs_attr_write_comp_t)write_comp_obj_list_ctrl_pt,
    .flow_ctrl_cb  = (ble_servs_attr_flow_ctrl_t)flow_ctrl_obj_list_ctrl_pt,
    .decode        = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_list_cp_t,
    .encode        = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_list_cp_t,
    .pp_descs      = gspp_obj_list_cp_descs,
    .num_of_descs  = ARRAY_SIZE(gspp_obj_list_cp_descs),
};

ble_status_t R_BLE_OTS_IndicateObjListCp(uint16_t conn_hdl, const st_ble_ots_obj_list_cp_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_obj_list_cp_char, conn_hdl, (const void *)p_value, false);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 0 characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_list_filter_0_t(st_ble_ots_obj_list_filter_0_t *p_app_value, 
                                                                    const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    if (BLE_OTS_OBJ_LIST_FILTER_0_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_list_filter_0_t));
    BT_UNPACK_LE_1_BYTE(&p_app_value->filter, &p_gatt_value->p_value[pos]);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NO_FILTER == p_app_value->filter)
    {
        /* Do Nothing */
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        if (BLE_GATT_16_BIT_UUID_SIZE == (p_gatt_value->value_len - 1))
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;

            BT_UNPACK_LE_2_BYTE(&p_app_value->uuid.uuid.uuid_16, &(p_gatt_value->p_value[pos]));
            pos += 2;
        }
        else
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            for (i = 0; i < 16; i++)
            {
                p_app_value->uuid.uuid.uuid_128[i] = p_gatt_value->p_value[pos++];
            }
        }
    }
    else if ((BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CREATED_BETWEEN == p_app_value->filter) || 
            (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_MODIFIED_BETWEEN == p_app_value->filter))
    {
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp1.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.seconds), &(p_gatt_value->p_value[pos++]));
        
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp2.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.seconds), &(p_gatt_value->p_value[pos++]));
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }

    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }
    else
    {
        /* Do Nothing */
    }

    return BLE_SUCCESS;

}

static ble_status_t encode_st_ble_ots_obj_list_filter_0_t(const st_ble_ots_obj_list_filter_0_t *p_app_value, 
                                                                                st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    /* Clear the byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos], &p_app_value->filter);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        /* value_len used to infer uuid type - send only the uuid */
        if (BLE_GATT_16_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->uuid.uuid.uuid_16);
            pos += 2;
        }
        else if (BLE_GATT_128_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            for (i = 0; i < 16; i++)
            {
                p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->uuid.uuid.uuid_128[i];
            }
        }
        else
        {
            /* Nothing */
        }
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CREATED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_MODIFIED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    p_gatt_value->value_len = (uint16_t)pos;
    return BLE_SUCCESS;
}

static void write_req_obj_list0(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                                st_ble_ots_obj_list_filter_0_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    /*st_ble_ots_obj_list_filter_0_t filter_value = { 0 };*/

    if (gs_is_olcp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OBJECT_LOCKED);
        return;
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        if (p_app_value->size1 > p_app_value->size2)
        {
            R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
            return;
        }
    }
    else if ((
        #if BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NO_FILTER != 0
        (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NO_FILTER > p_app_value->filter) ||
        #endif
        (BLE_OTS_OBJ_LIST_FILTER_0_FILTER_MARKED_OBJECTS < p_app_value->filter)) &&
             (false == gs_write_long_obj_filter_0_flag))
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }
    else
    {
        /* Nothing */
    }

    ots_object_list_filter_cb(p_app_value);
}

static void write_comp_obj_list0(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                                void *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    uint16_t check_long = *((uint16_t *)p_app_value);
    if(BLE_GATTS_EVENT_PREPARE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_0_flag = true;
    }
    else if(BLE_GATTS_EVENT_EXE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_0_flag = false;
    }
}

/*static void write_comp_obj_list0(const void *p_attr,
    uint16_t conn_hdl,
    ble_status_t result,
    st_ble_ots_obj_list_cp_t *p_app_value)
{
    gs_is_olcp_in_progress = true;
    gs_conn_hdl            = conn_hdl;
    ots_olcp_process_operation(conn_hdl, p_app_value, true);
}*/

/* Object List Filter 0 characteristic definition */
static const st_ble_servs_char_info_t gs_obj_list_filter_0_char = {
    .start_hdl = BLE_OTS_OBJ_LIST_FILTER_0_DECL_HDL,
    .end_hdl = BLE_OTS_OBJ_LIST_FILTER_0_VAL_HDL,
    .char_idx = BLE_OTS_OBJ_LIST_FILTER_0_IDX,
    .app_size = sizeof(st_ble_ots_obj_list_filter_0_t),
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_obj_list0,
    .write_comp_cb= (ble_servs_attr_write_comp_t)write_comp_obj_list0,
    .db_size      = BLE_OTS_OBJ_LIST_FILTER_0_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_list_filter_0_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_list_filter_0_t,
};

ble_status_t R_BLE_OTS_SetObjListFilter0(const st_ble_ots_obj_list_filter_0_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_list_filter_0_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjListFilter0(st_ble_ots_obj_list_filter_0_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_list_filter_0_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 1 characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_list_filter_1_t(st_ble_ots_obj_list_filter_1_t *p_app_value, 
                                                                        const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    if (BLE_OTS_OBJ_LIST_FILTER_1_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_list_filter_1_t));

    BT_UNPACK_LE_1_BYTE(&p_app_value->filter, &p_gatt_value->p_value[pos]);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        if (BLE_GATT_16_BIT_UUID_SIZE == (p_gatt_value->value_len - 1))
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;

            BT_UNPACK_LE_2_BYTE(&p_app_value->uuid.uuid.uuid_16, &(p_gatt_value->p_value[pos]));
            pos += 2;
        }
        else
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            for (i = 0; i < 16; i++)
            {
                p_app_value->uuid.uuid.uuid_128[i] = p_gatt_value->p_value[pos++];
            }
        }
    }
    else if ((BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CREATED_BETWEEN == p_app_value->filter) || 
            (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_MODIFIED_BETWEEN == p_app_value->filter))
    {
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp1.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.seconds), &(p_gatt_value->p_value[pos++]));
        
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp2.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.seconds), &(p_gatt_value->p_value[pos++]));
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }
    else
    {
        /* Do Nothing */
    }

    return BLE_SUCCESS;

}

static ble_status_t encode_st_ble_ots_obj_list_filter_1_t(const st_ble_ots_obj_list_filter_1_t *p_app_value, 
                                                                                st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    /* Clear the byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos], &p_app_value->filter);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        /* value_len is used to infer uuid type..only uuid sent */
        if (BLE_GATT_16_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->uuid.uuid.uuid_16);
            pos += 2;
        }
        else if (BLE_GATT_128_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            for (i = 0; i < 16; i++)
            {
                p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->uuid.uuid.uuid_128[i];
            }
        }
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CREATED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_MODIFIED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    p_gatt_value->value_len = (uint16_t)pos;
    return BLE_SUCCESS;
}

static void write_req_obj_list1(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                        st_ble_ots_obj_list_filter_0_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    /*st_ble_ots_obj_list_filter_1_t filter_value = { 0 };*/
    if (gs_is_olcp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OBJECT_LOCKED);
        return;
    }
    else if ((
        #if BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NO_FILTER != 0
        (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NO_FILTER > p_app_value->filter) ||
        #endif
        (BLE_OTS_OBJ_LIST_FILTER_1_FILTER_MARKED_OBJECTS < p_app_value->filter)) &&
             (false == gs_write_long_obj_filter_1_flag))
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }
    else
    {
        /* Nothing */
    }

    ots_object_list_filter_cb(p_app_value);
}

static void write_comp_obj_list1(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                                void *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    uint16_t check_long = *((uint16_t *)p_app_value);
    if(BLE_GATTS_EVENT_PREPARE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_1_flag = true;
    }
    else if(BLE_GATTS_EVENT_EXE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_1_flag = false;
    }
}

/* Object List Filter 1 characteristic definition */
static const st_ble_servs_char_info_t gs_obj_list_filter_1_char = {
    .start_hdl    = BLE_OTS_OBJ_LIST_FILTER_1_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_LIST_FILTER_1_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_LIST_FILTER_1_IDX,
    .app_size     = sizeof(st_ble_ots_obj_list_filter_1_t),
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_obj_list1,
    .write_comp_cb= (ble_servs_attr_write_comp_t)write_comp_obj_list1,
    .db_size      = BLE_OTS_OBJ_LIST_FILTER_1_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_list_filter_1_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_list_filter_1_t,
};

ble_status_t R_BLE_OTS_SetObjListFilter1(const st_ble_ots_obj_list_filter_1_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_list_filter_1_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjListFilter1(st_ble_ots_obj_list_filter_1_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_list_filter_1_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 2 characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_list_filter_2_t(st_ble_ots_obj_list_filter_2_t *p_app_value, 
                                                                            const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    if (BLE_OTS_OBJ_LIST_FILTER_2_LEN < p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_list_filter_2_t));

    BT_UNPACK_LE_1_BYTE(&p_app_value->filter, &p_gatt_value->p_value[pos]);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        p_app_value->string.data = &g_obj_name[0];
        memcpy(p_app_value->string.data, &p_gatt_value->p_value[pos++], (size_t)(p_gatt_value->value_len - 1));
        p_app_value->string.len = (uint16_t)(p_gatt_value->value_len - 1);
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        if (BLE_GATT_16_BIT_UUID_SIZE == (p_gatt_value->value_len - 1))
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;

            BT_UNPACK_LE_2_BYTE(&p_app_value->uuid.uuid.uuid_16, &(p_gatt_value->p_value[pos]));
            pos += 2;
        }
        else
        {
            p_app_value->uuid.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            for (i = 0; i < 16; i++)
            {
                p_app_value->uuid.uuid.uuid_128[i] = p_gatt_value->p_value[pos++];
            }
        }
    }
    else if ((BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CREATED_BETWEEN == p_app_value->filter) || 
            (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_MODIFIED_BETWEEN == p_app_value->filter))
    {
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp1.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp1.seconds), &(p_gatt_value->p_value[pos++]));
        
        BT_UNPACK_LE_2_BYTE(&(p_app_value->timestamp2.year), &(p_gatt_value->p_value[pos]));
        pos += 2;

        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.month), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.day), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.hours), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.minutes), &(p_gatt_value->p_value[pos++]));
        BT_UNPACK_LE_1_BYTE(&(p_app_value->timestamp2.seconds), &(p_gatt_value->p_value[pos++]));
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }
    else if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_UNPACK_LE_4_BYTE(&(p_app_value->size1), &(p_gatt_value->p_value[pos]));
        pos += 4;

        BT_UNPACK_LE_4_BYTE(&(p_app_value->size2), &(p_gatt_value->p_value[pos]));
        pos += 4;
    }
    else
    {
        /* Do Nothing */
    }

    return BLE_SUCCESS;

}

static ble_status_t encode_st_ble_ots_obj_list_filter_2_t(const st_ble_ots_obj_list_filter_2_t *p_app_value, 
                                                                            st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;
    uint8_t  i   = 0;

    /* Clear the byte array */
    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    BT_PACK_LE_1_BYTE(&p_gatt_value->p_value[pos], &p_app_value->filter);
    pos += 1;

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_STARTS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_ENDS_WITH == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_CONTAINS == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_IS_EXACTLY == p_app_value->filter)
    {
        memcpy(&p_gatt_value->p_value[pos], p_app_value->string.data, p_app_value->string.len);
        pos += p_app_value->string.len;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_OBJECT_TYPE == p_app_value->filter)
    {
        /* value_len is used to infer uuid type..only uuid sent */
        if (BLE_GATT_16_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->uuid.uuid.uuid_16);
            pos += 2;
        }
        else if (BLE_GATT_128_BIT_UUID_FORMAT == p_app_value->uuid.uuid_type)
        {
            for (i = 0; i < 16; i++)
            {
                p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->uuid.uuid.uuid_128[i];
            }
        }
        else
        {
            /* Nothing */
        }
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CREATED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_MODIFIED_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp1.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp1.seconds));

        BT_PACK_LE_2_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->timestamp2.year));
        pos += 2;

        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.month));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.day));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.hours));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.minutes));
        BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[pos++]), &(p_app_value->timestamp2.seconds));
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CURRENT_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    if (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_ALLOCATED_SIZE_BETWEEN == p_app_value->filter)
    {
        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size1));
        pos += 4;

        BT_PACK_LE_4_BYTE(&(p_gatt_value->p_value[pos]), &(p_app_value->size2));
        pos += 4;
    }

    p_gatt_value->value_len = (uint16_t)pos;
    return BLE_SUCCESS;
}

static void write_req_obj_list2(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                        st_ble_ots_obj_list_filter_0_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    /*st_ble_ots_obj_list_filter_2_t filter_value = { 0 };*/
    if (gs_is_olcp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OBJECT_LOCKED);
        return;
    }

    else if ((
        #if BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NO_FILTER != 0
        (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NO_FILTER > p_app_value->filter) ||
        #endif
        (BLE_OTS_OBJ_LIST_FILTER_2_FILTER_MARKED_OBJECTS < p_app_value->filter)) &&
             (false == gs_write_long_obj_filter_2_flag))
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }
    else
    {
        /* Nothing */
    }
    
    ots_object_list_filter_cb(p_app_value);
}

static void write_comp_obj_list2(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                                void *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    uint16_t check_long = *((uint16_t *)p_app_value);
    if(BLE_GATTS_EVENT_PREPARE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_2_flag = true;
    }
    else if(BLE_GATTS_EVENT_EXE_WRITE_RSP_COMP == check_long)
    {
        gs_write_long_obj_filter_2_flag = false;
    }
}

/* Object List Filter 2 characteristic definition */
static const st_ble_servs_char_info_t gs_obj_list_filter_2_char = {
    .start_hdl    = BLE_OTS_OBJ_LIST_FILTER_2_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_LIST_FILTER_2_VAL_HDL,
    .char_idx     = BLE_OTS_OBJ_LIST_FILTER_2_IDX,
    .app_size     = sizeof(st_ble_ots_obj_list_filter_2_t),
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_obj_list2,
    .write_comp_cb= (ble_servs_attr_write_comp_t)write_comp_obj_list2,
    .db_size      = BLE_OTS_OBJ_LIST_FILTER_2_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_list_filter_2_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_list_filter_2_t,
};

ble_status_t R_BLE_OTS_SetObjListFilter2(const st_ble_ots_obj_list_filter_2_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_obj_list_filter_2_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjListFilter2(st_ble_ots_obj_list_filter_2_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_obj_list_filter_2_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Changed Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_obj_changed_cli_cnfg = {
    .attr_hdl = BLE_OTS_OBJ_CHANGED_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_OTS_OBJ_CHANGED_CLI_CNFG_IDX,
    .db_size  = BLE_OTS_OBJ_CHANGED_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_OTS_SetObjChangedCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_obj_changed_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_OTS_GetObjChangedCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_obj_changed_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Changed characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_ots_obj_changed_t(st_ble_ots_obj_changed_t *p_app_value, 
                                                            const st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert byte sequence to app data. */
    /*uint8_t pos            = 2;*/
    uint8_t ots_obj_change = 0;

    if (BLE_OTS_OBJ_CHANGED_LEN != p_gatt_value->value_len)
    {
        return BLE_ERR_INVALID_DATA;
    }

    BT_UNPACK_LE_2_BYTE(&ots_obj_change, &p_gatt_value->p_value[0]);

    /* Clear the application structure */
    memset(p_app_value, 0x0, sizeof(st_ble_ots_obj_changed_t));

    if (ots_obj_change & BLE_OTS_PRV_SOURCE_OF_CHANGE)
    {
        p_app_value->flags.is_source_of_change = true;
    }
    else
    {
        p_app_value->flags.is_source_of_change = false;
    }

    if (ots_obj_change & BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_CONTENTS)
    {
        p_app_value->flags.is_change_occurred_to_the_object_contents = true;
    }
    else
    {
        p_app_value->flags.is_change_occurred_to_the_object_contents = false;
    }

    if (ots_obj_change & BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_METADATA)
    {
        p_app_value->flags.is_change_occurred_to_the_object_metadata = true;
    }
    else
    {
        p_app_value->flags.is_change_occurred_to_the_object_metadata = false;
    }

    if (ots_obj_change & BLE_OTS_PRV_OBJECT_CREATION)
    {
        p_app_value->flags.is_object_creation = true;
    }
    else
    {
        p_app_value->flags.is_object_creation = false;
    }

    if (ots_obj_change & BLE_OTS_PRV_OBJECT_DELETION)
    {
        p_app_value->flags.is_object_deletion = true;
    }
    else
    {
        p_app_value->flags.is_object_deletion = false;
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_ots_obj_changed_t(const st_ble_ots_obj_changed_t *p_app_value, 
                                                                    st_ble_gatt_value_t *p_gatt_value)
{
    /* Convert app data to byte sequence. */
    uint32_t pos           = 0;
    /*uint8_t  i             = 0;*/
    uint8_t ots_obj_change = 0;

    /* Clear the gatt structure */
    memset(p_gatt_value->p_value, 0x0, BLE_OTS_OBJ_CHANGED_LEN);

    if (p_app_value->flags.is_source_of_change)
    {
        ots_obj_change |= BLE_OTS_PRV_SOURCE_OF_CHANGE;
    }

    if (p_app_value->flags.is_change_occurred_to_the_object_contents)
    {
        ots_obj_change |= BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_CONTENTS;
    }

    if (p_app_value->flags.is_change_occurred_to_the_object_metadata)
    {
        ots_obj_change |= BLE_OTS_PRV_CHANGE_OCCURRED_TO_THE_OBJECT_METADATA;
    }

    if (p_app_value->flags.is_object_creation)
    {
        ots_obj_change |= BLE_OTS_PRV_OBJECT_CREATION;
    }

    if (p_app_value->flags.is_object_deletion)
    {
        ots_obj_change |= BLE_OTS_PRV_OBJECT_DELETION;
    }

    BT_PACK_LE_1_BYTE(&(p_gatt_value->p_value[0]), &ots_obj_change);
    pos += 1;

    memcpy(&p_gatt_value->p_value[pos], &p_app_value->object_id[0], 6);
    pos += 6;

    /* Update Length */
    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

static void write_req_obj_changed(const void *p_attr, uint16_t conn_hdl, ble_status_t result, 
                                                                st_ble_ots_obj_changed_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(result);
    UNUSED_ARG(p_app_value);

    if (gs_is_olcp_in_progress)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_WRITE_REQUEST_REJECTED_ERROR);
        return;
    }

    uint16_t cli_cnfg;
    R_BLE_OTS_GetObjChangedCliCnfg(conn_hdl, &cli_cnfg);
    if (BLE_GATTS_CLI_CNFG_INDICATION != cli_cnfg)
    {
        R_BLE_GATTS_SendErrRsp(BLE_OTS_CLI_CNFG_IMPROPERLY_CONFIGURED_ERROR);
        return;
    }
}


/* Object Changed characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_obj_changed_descs[] = {
    &gs_obj_changed_cli_cnfg,
};

/* Object Changed characteristic definition */
static const st_ble_servs_char_info_t gs_obj_changed_char = {
    .start_hdl    = BLE_OTS_OBJ_CHANGED_DECL_HDL,
    .end_hdl      = BLE_OTS_OBJ_CHANGED_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_OTS_OBJ_CHANGED_IDX,
    .app_size     = sizeof(st_ble_ots_obj_changed_t),
    .db_size      = BLE_OTS_OBJ_CHANGED_LEN,
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_obj_changed,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_ots_obj_changed_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_ots_obj_changed_t,
    .pp_descs     = gspp_obj_changed_descs,
    .num_of_descs = ARRAY_SIZE(gspp_obj_changed_descs),
};

ble_status_t R_BLE_OTS_IndicateObjChanged(uint16_t conn_hdl, const st_ble_ots_obj_changed_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_obj_changed_char, conn_hdl, (const void *)p_value, false);
}

/*----------------------------------------------------------------------------------------------------------------------
    Object Transfer Service server
----------------------------------------------------------------------------------------------------------------------*/

/* Object Transfer Service characteristics definition */
static const st_ble_servs_char_info_t *gspp_chars[] = {
    &gs_feat_char,
    &gs_obj_name_char,
    &gs_obj_type_char,
    &gs_obj_size_char,
    &gs_obj_first_created_char,
    &gs_obj_last_modified_char,
    &gs_obj_id_char,
    &gs_obj_prop_char,
    &gs_obj_action_cp_char,
    &gs_obj_list_cp_char,
    &gs_obj_list_filter_0_char,
    &gs_obj_list_filter_1_char,
    &gs_obj_list_filter_2_char,
    &gs_obj_changed_char,
};

/* Object Transfer Service Server definition */
static st_ble_servs_info_t gs_servs_info = {
    .pp_chars     = gspp_chars,
    .num_of_chars = ARRAY_SIZE(gspp_chars),
};

ble_status_t R_BLE_OTS_Init(ble_servs_app_cb_t cb)
{
    if (NULL == cb)
    {
        return BLE_ERR_INVALID_PTR;
    }

    gs_servs_info.cb = cb;

    ots_db_init();
    return R_BLE_SERVS_RegisterServer(&gs_servs_info);
}

static ble_status_t ots_send_object_action_ctrl_pt_resp(uint16_t conn_hdl, uint8_t op_code, 
                                                        uint8_t req_op_code, uint8_t resp_code, uint16_t param)
{
    st_ble_ots_obj_action_cp_t value = 
    {
        .op_code         = op_code,
        .request_op_code = req_op_code,
        .result_code     = resp_code,
    };


    if (BLE_OTS_OBJ_ACTION_CP_OP_CODE_CALCULATE_CHECKSUM == req_op_code)
    {
        BT_PACK_LE_2_BYTE(&value.response_parameter, &param);
    }

    R_BLE_OTS_IndicateObjActionCp(conn_hdl, &value);

    gs_is_oacp_in_progress = false;

    return BLE_SUCCESS;
}

static ble_status_t ots_send_obj_list_ctrl_pt_resp(uint16_t conn_hdl, uint8_t op_code, 
                                                    uint8_t req_op_code, uint8_t resp_code, uint32_t param)
{
    
    st_ble_ots_obj_list_cp_t value =
    {
        .op_code         = op_code,
        .request_op_code = req_op_code,
        .result_code     = resp_code,
    };

    if (BLE_OTS_OBJ_LIST_CP_OP_CODE_REQUEST_NUMBER_OF_OBJECTS == req_op_code)
    {
        value.response_parameter = param;
    }

    R_BLE_OTS_IndicateObjListCp(conn_hdl, &value);

    gs_is_olcp_in_progress = false;

    return BLE_SUCCESS;
}
