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
 * File Name: r_ble_dis.c
 * Description : The source file for Device Information service.
 **********************************************************************************************************************/

#include <string.h>
#include "r_ble_dis.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

static st_ble_servs_info_t gs_servs_info;

/*----------------------------------------------------------------------------------------------------------------------
    Manufacturer Name characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Manufacturer Name characteristic definition */
static const st_ble_servs_char_info_t gs_mfr_name_char = {
    .start_hdl    = BLE_DIS_MFR_NAME_DECL_HDL,
    .end_hdl      = BLE_DIS_MFR_NAME_VAL_HDL,
    .char_idx     = BLE_DIS_MFR_NAME_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_MFR_NAME_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetMfrName(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_mfr_name_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetMfrName(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_mfr_name_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Model Number characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Model Number characteristic definition */
static const st_ble_servs_char_info_t gs_model_num_char = {
    .start_hdl    = BLE_DIS_MODEL_NUM_DECL_HDL,
    .end_hdl      = BLE_DIS_MODEL_NUM_VAL_HDL,
    .char_idx     = BLE_DIS_MODEL_NUM_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_MODEL_NUM_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetModelNum(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_model_num_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetModelNum(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_model_num_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Serial Number characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Serial Number characteristic definition */
static const st_ble_servs_char_info_t gs_ser_num_char = {
    .start_hdl    = BLE_DIS_SER_NUM_DECL_HDL,
    .end_hdl      = BLE_DIS_SER_NUM_VAL_HDL,
    .char_idx     = BLE_DIS_SER_NUM_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_SER_NUM_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetSerNum(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_ser_num_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetSerNum(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_ser_num_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Hardware Revision characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Hardware Revision characteristic definition */
static const st_ble_servs_char_info_t gs_hw_rev_char = {
    .start_hdl    = BLE_DIS_HW_REV_DECL_HDL,
    .end_hdl      = BLE_DIS_HW_REV_VAL_HDL,
    .char_idx     = BLE_DIS_HW_REV_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_HW_REV_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetHwRev(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_hw_rev_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetHwRev(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_hw_rev_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Firmware Revision characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Firmware Revision characteristic definition */
static const st_ble_servs_char_info_t gs_firm_rev_char = {
    .start_hdl    = BLE_DIS_FIRM_REV_DECL_HDL,
    .end_hdl      = BLE_DIS_FIRM_REV_VAL_HDL,
    .char_idx     = BLE_DIS_FIRM_REV_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_FIRM_REV_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetFirmRev(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_firm_rev_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetFirmRev(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_firm_rev_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Software Revision characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Software Revision characteristic definition */
static const st_ble_servs_char_info_t gs_sw_rev_char = {
    .start_hdl    = BLE_DIS_SW_REV_DECL_HDL,
    .end_hdl      = BLE_DIS_SW_REV_VAL_HDL,
    .char_idx     = BLE_DIS_SW_REV_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_SW_REV_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetSwRev(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_sw_rev_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetSwRev(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_sw_rev_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    System ID characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_dis_sys_id_t(st_ble_dis_sys_id_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    memcpy(p_app_value->manufacturer_identifier, &p_gatt_value->p_value[pos], 5);
    pos += 5;

    memcpy(p_app_value->organizationally_unique_identifier, &p_gatt_value->p_value[pos], 3);
    pos += 3;

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_dis_sys_id_t(const st_ble_dis_sys_id_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    memcpy(&p_gatt_value->p_value[pos], p_app_value->manufacturer_identifier, 5);
    pos += 5;

    memcpy(&p_gatt_value->p_value[pos], p_app_value->organizationally_unique_identifier, 3);
    pos += 3;

    return BLE_SUCCESS;
}

/* System ID characteristic definition */
static const st_ble_servs_char_info_t gs_sys_id_char = {
    .start_hdl    = BLE_DIS_SYS_ID_DECL_HDL,
    .end_hdl      = BLE_DIS_SYS_ID_VAL_HDL,
    .char_idx     = BLE_DIS_SYS_ID_IDX,
    .app_size     = sizeof(st_ble_dis_sys_id_t),
    .db_size      = BLE_DIS_SYS_ID_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_dis_sys_id_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_dis_sys_id_t,
};

ble_status_t R_BLE_DIS_SetSysId(const st_ble_dis_sys_id_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_sys_id_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetSysId(st_ble_dis_sys_id_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_sys_id_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    IEEE 11073-20601 Regulatory Certification Data List characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* IEEE 11073-20601 Regulatory Certification Data List characteristic definition */
static const st_ble_servs_char_info_t gs_reg_cer_data_list_char = {
    .start_hdl    = BLE_DIS_REG_CER_DATA_LIST_DECL_HDL,
    .end_hdl      = BLE_DIS_REG_CER_DATA_LIST_VAL_HDL,
    .char_idx     = BLE_DIS_REG_CER_DATA_LIST_IDX,
    .app_size     = sizeof(st_ble_seq_data_t),
    .db_size      = BLE_DIS_REG_CER_DATA_LIST_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_seq_data_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_seq_data_t,
};

ble_status_t R_BLE_DIS_SetRegCerDataList(const st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_reg_cer_data_list_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetRegCerDataList(st_ble_seq_data_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_reg_cer_data_list_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    PnP ID characteristic
----------------------------------------------------------------------------------------------------------------------*/

static ble_status_t decode_st_ble_dis_pnp_id_t(st_ble_dis_pnp_id_t *p_app_value, const st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    p_app_value->vendor_id_source = p_gatt_value->p_value[pos++];

    BT_UNPACK_LE_2_BYTE(&p_app_value->vendor_id, &p_gatt_value->p_value[pos]);
    pos += sizeof(uint16_t);

    BT_UNPACK_LE_2_BYTE(&p_app_value->product_id, &p_gatt_value->p_value[pos]);
    pos += sizeof(uint16_t);

    BT_UNPACK_LE_2_BYTE(&p_app_value->product_version, &p_gatt_value->p_value[pos]);
    pos += sizeof(uint16_t);

    return BLE_SUCCESS;
}

static ble_status_t encode_st_ble_dis_pnp_id_t(const st_ble_dis_pnp_id_t *p_app_value, st_ble_gatt_value_t *p_gatt_value)
{
    uint32_t pos = 0;

    p_gatt_value->p_value[pos++] = p_app_value->vendor_id_source;

    BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->vendor_id);
    pos += sizeof(uint16_t);

    BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->product_id);
    pos += sizeof(uint16_t);

    BT_PACK_LE_2_BYTE(&p_gatt_value->p_value[pos], &p_app_value->product_version);
    pos += sizeof(uint16_t);

    return BLE_SUCCESS;
}

/* PnP ID characteristic definition */
static const st_ble_servs_char_info_t gs_pnp_id_char = {
    .start_hdl    = BLE_DIS_PNP_ID_DECL_HDL,
    .end_hdl      = BLE_DIS_PNP_ID_VAL_HDL,
    .char_idx     = BLE_DIS_PNP_ID_IDX,
    .app_size     = sizeof(st_ble_dis_pnp_id_t),
    .db_size      = BLE_DIS_PNP_ID_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_st_ble_dis_pnp_id_t,
    .encode       = (ble_servs_attr_encode_t)encode_st_ble_dis_pnp_id_t,
};

ble_status_t R_BLE_DIS_SetPnpId(const st_ble_dis_pnp_id_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_pnp_id_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_DIS_GetPnpId(st_ble_dis_pnp_id_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_pnp_id_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Device Information server
----------------------------------------------------------------------------------------------------------------------*/

/* Device Information characteristics definition */
static const st_ble_servs_char_info_t *gspp_chars[] = {
    &gs_mfr_name_char,
    &gs_model_num_char,
    &gs_ser_num_char,
    &gs_hw_rev_char,
    &gs_firm_rev_char,
    &gs_sw_rev_char,
    &gs_sys_id_char,
    &gs_reg_cer_data_list_char,
    &gs_pnp_id_char,
};

/* Device Information service definition */
static st_ble_servs_info_t gs_servs_info = {
    .pp_chars     = gspp_chars,
    .num_of_chars = ARRAY_SIZE(gspp_chars),
};

ble_status_t R_BLE_DIS_Init(ble_servs_app_cb_t cb)
{
    if (NULL == cb)
    {
        return BLE_ERR_INVALID_PTR;
    }

    gs_servs_info.cb = cb;

    return R_BLE_SERVS_RegisterServer(&gs_servs_info);
}
