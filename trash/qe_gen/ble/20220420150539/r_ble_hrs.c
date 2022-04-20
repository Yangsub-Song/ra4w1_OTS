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
 * File Name: r_ble_hrs.c
 * Description : The source file for Heart Rate service.
 **********************************************************************************************************************/

#include <string.h>
#include "r_ble_hrs.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef UNUSED_ARG
#define UNUSED_ARG(arg)         (void)(arg)
#endif /* UNUSED_ARG */

static st_ble_servs_info_t gs_servs_info;

static uint16_t gs_energy_expended;

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Measurement Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_meas_cli_cnfg = {
    .attr_hdl = BLE_HRS_MEAS_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_HRS_MEAS_CLI_CNFG_IDX,
    .db_size  = BLE_HRS_MEAS_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_HRS_SetMeasCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_meas_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_HRS_GetMeasCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_meas_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Measurement characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_hrs_meas_t(st_ble_hrs_meas_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    p_app_value->flags.is_heart_rate_value_16_bit          = !!(p_gatt_value->p_value[pos] & 0x01);
    p_app_value->flags.is_sensor_contact_detected          = !!(p_gatt_value->p_value[pos] & 0x02);
    p_app_value->flags.is_sensor_contact_feature_supported = !!(p_gatt_value->p_value[pos] & 0x04);
    p_app_value->flags.is_energy_expended_included         = !!(p_gatt_value->p_value[pos] & 0x08);
    p_app_value->flags.is_rr_interval_included             = !!(p_gatt_value->p_value[pos] & 0x10);
    pos++;

    if (p_app_value->flags.is_heart_rate_value_16_bit)
    {
        BT_UNPACK_LE_2_BYTE(&p_app_value->heart_rate_measurement_value, &p_gatt_value->p_value[pos]);
        pos += 2;
    }

    if (p_app_value->flags.is_energy_expended_included)
    {
        BT_UNPACK_LE_2_BYTE(&gs_energy_expended, &p_gatt_value->p_value[pos]);
        pos += 2;
    }

    if (p_app_value->flags.is_rr_interval_included)
    {
        p_app_value->number_of_rr_interval = (uint8_t)((p_gatt_value->value_len - pos) / 2);

        for (uint8_t i = 0; i < p_app_value->number_of_rr_interval; i++)
        {
            BT_UNPACK_LE_2_BYTE(&p_app_value->rr_interval[i], &p_gatt_value->p_value[pos]);
            pos += 2;
        }
    }

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_hrs_meas_t(const st_ble_hrs_meas_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    memset(p_gatt_value->p_value, 0x00, p_gatt_value->value_len);

    p_gatt_value->p_value[pos] = (uint8_t)(p_app_value->flags.is_heart_rate_value_16_bit ? 0x01 : 0x00);
    if (p_app_value->flags.is_sensor_contact_feature_supported)
    {
        p_gatt_value->p_value[pos] = (uint8_t)(p_gatt_value->p_value[pos]
                                   | (p_app_value->flags.is_sensor_contact_detected ? 0x02 : 0x00)
                                   | (p_app_value->flags.is_sensor_contact_feature_supported ? 0x04 : 0x00));
    }
    p_gatt_value->p_value[pos] = (uint8_t)(p_gatt_value->p_value[pos]
                               | (p_app_value->flags.is_energy_expended_included ? 0x08 : 0x00)
                               | (p_app_value->flags.is_rr_interval_included ? 0x10 : 0x00));
    pos++;

    if (p_app_value->flags.is_heart_rate_value_16_bit)
    {
        BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->heart_rate_measurement_value);
        pos += 2;
    }
    else
    {
        p_gatt_value->p_value[pos++] = (uint8_t)p_app_value->heart_rate_measurement_value;
    }

    if (p_app_value->flags.is_energy_expended_included)
    {
        BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &gs_energy_expended);
        pos += 2;
    }

    if (p_app_value->flags.is_rr_interval_included)
    {
        for (uint8_t i = 0; i < p_app_value->number_of_rr_interval; i++)
        {
            BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->rr_interval[i]);
            pos += 2;
        }
    }

    p_gatt_value->value_len = (uint16_t)pos;

    return BLE_SUCCESS;
}

/* Heart Rate Measurement characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_meas_descs[] = {
    &gs_meas_cli_cnfg,
};

/* Heart Rate Measurement characteristic definition */
static const st_ble_servs_char_info_t gs_meas_char = {
    .start_hdl    = BLE_HRS_MEAS_DECL_HDL,
    .end_hdl      = BLE_HRS_MEAS_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_HRS_MEAS_IDX,
    .app_size     = sizeof(st_ble_hrs_meas_t),
    .db_size      = BLE_HRS_MEAS_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_hrs_meas_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_hrs_meas_t,
    .pp_descs     = gspp_meas_descs,
    .num_of_descs = ARRAY_SIZE(gspp_meas_descs),
};

ble_status_t R_BLE_HRS_NotifyMeas(uint16_t conn_hdl, const st_ble_hrs_meas_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_meas_char, conn_hdl, (const void *)p_value, true);
}

ble_status_t R_BLE_HRS_AddEnergyExpended(uint16_t *p_value)
{
    if (NULL == p_value)
    {
        return BLE_ERR_INVALID_PTR;
    }

    if (gs_energy_expended + *p_value > gs_energy_expended)
    {
        gs_energy_expended = (uint16_t)(gs_energy_expended + *p_value);
    }
    else
    {
        gs_energy_expended = 0xFFFF;
    }

    return BLE_SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------
    Body Sensor Location characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Body Sensor Location characteristic definition */
static const st_ble_servs_char_info_t gs_body_sensor_location_char = {
    .start_hdl    = BLE_HRS_BODY_SENSOR_LOCATION_DECL_HDL,
    .end_hdl      = BLE_HRS_BODY_SENSOR_LOCATION_VAL_HDL,
    .char_idx     = BLE_HRS_BODY_SENSOR_LOCATION_IDX,
    .app_size     = sizeof(uint8_t),
    .db_size      = BLE_HRS_BODY_SENSOR_LOCATION_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_uint8_t,
    .encode       = (ble_servs_attr_encode_t)encode_uint8_t,
};

ble_status_t R_BLE_HRS_SetBodySensorLocation(const uint8_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_body_sensor_location_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_HRS_GetBodySensorLocation(uint8_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_body_sensor_location_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Control Point characteristic
----------------------------------------------------------------------------------------------------------------------*/

static void write_req_heart_rate_cp(const void *p_attr, uint16_t conn_hdl, ble_status_t result, uint8_t *p_app_value)
{
    UNUSED_ARG(p_attr);
    UNUSED_ARG(conn_hdl);
    UNUSED_ARG(result);

    uint8_t op_code = *p_app_value;

    if (BLE_HRS_HEART_RATE_CP_HEART_RATE_CONTROL_POINT_RESET_ENERGY_EXPENDED == op_code)
    {
        gs_energy_expended = 0;
    }
    else
    {
        R_BLE_GATTS_SendErrRsp(BLE_HRS_HEART_RATE_CONTROL_POINT_NOT_SUPPORTED_ERROR);
    }
}

/* Heart Rate Control Point characteristic definition */
static const st_ble_servs_char_info_t gs_heart_rate_cp_char = {
    .start_hdl    = BLE_HRS_HEART_RATE_CP_DECL_HDL,
    .end_hdl      = BLE_HRS_HEART_RATE_CP_VAL_HDL,
    .char_idx     = BLE_HRS_HEART_RATE_CP_IDX,
    .app_size     = sizeof(uint8_t),
    .db_size      = BLE_HRS_HEART_RATE_CP_LEN,
    .write_req_cb = (ble_servs_attr_write_req_t)write_req_heart_rate_cp,
    .decode       = (ble_servs_attr_decode_t)decode_uint8_t,
    .encode       = (ble_servs_attr_encode_t)encode_uint8_t,
};

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate server
----------------------------------------------------------------------------------------------------------------------*/

/* Heart Rate characteristics definition */
static const st_ble_servs_char_info_t *gspp_chars[] = {
    &gs_meas_char,
    &gs_body_sensor_location_char,
    &gs_heart_rate_cp_char,
};

/* Heart Rate service definition */
static st_ble_servs_info_t gs_servs_info = {
    .pp_chars     = gspp_chars,
    .num_of_chars = ARRAY_SIZE(gspp_chars),
};

ble_status_t R_BLE_HRS_Init(ble_servs_app_cb_t cb)
{
    if (NULL == cb)
    {
        return BLE_ERR_INVALID_PTR;
    }

    gs_servs_info.cb = cb;

    return R_BLE_SERVS_RegisterServer(&gs_servs_info);
}
