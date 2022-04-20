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
 * File Name: r_ble_dis.h
 * Description : The header file for Device Information service.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @file
 * @defgroup dis Device Information Service Server
 * @{
 * @ingroup profile
 * @brief   The Device Information Service exposes manufacturer and/or vendor information about a device.
 **********************************************************************************************************************/
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef R_BLE_DIS_H
#define R_BLE_DIS_H

/*----------------------------------------------------------------------------------------------------------------------
    Manufacturer Name Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Manufacturer Name characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetMfrName(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Manufacturer Name characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetMfrName(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Model Number Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Model Number characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetModelNum(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Model Number characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetModelNum(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Serial Number Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Serial Number characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetSerNum(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Serial Number characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetSerNum(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Hardware Revision Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Hardware Revision characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetHwRev(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Hardware Revision characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetHwRev(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Firmware Revision Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Firmware Revision characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetFirmRev(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Firmware Revision characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetFirmRev(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Software Revision Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Software Revision characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetSwRev(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Software Revision characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetSwRev(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    System ID Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief System ID value structure.
*******************************************************************************/
typedef struct {
    uint8_t manufacturer_identifier[5]; /**< Manufacturer Identifier */
    uint8_t organizationally_unique_identifier[3]; /**< Organizationally Unique Identifier */
} st_ble_dis_sys_id_t;

/***************************************************************************//**
 * @brief     Set System ID characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetSysId(const st_ble_dis_sys_id_t *p_value);

/***************************************************************************//**
 * @brief     Get System ID characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetSysId(st_ble_dis_sys_id_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    IEEE 11073-20601 Regulatory Certification Data List Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set IEEE 11073-20601 Regulatory Certification Data List characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetRegCerDataList(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get IEEE 11073-20601 Regulatory Certification Data List characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetRegCerDataList(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    PnP ID Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief PnP ID Vendor ID Source enumeration.
*******************************************************************************/
typedef enum {
    BLE_DIS_PNP_ID_VENDOR_ID_SOURCE_BLUETOOTH_SIG_ASSIGNED_COMPANY_IDENTIFIER_VALUE_FROM_THE_ASSIGNED_NUMBERS_DOCUMENT = 1, /**< Bluetooth SIG assigned Company Identifier value from the Assigned Numbers document */
    BLE_DIS_PNP_ID_VENDOR_ID_SOURCE_USB_IMPLEMENTER_S_FORUM_ASSIGNED_VENDOR_ID_VALUE = 2, /**< USB Implementer's Forum assigned Vendor ID value */
} e_ble_dis_pnp_id_vendor_id_source_t;

/***************************************************************************//**
 * @brief PnP ID value structure.
*******************************************************************************/
typedef struct {
    uint8_t vendor_id_source; /**< Vendor ID Source */
    uint16_t vendor_id; /**< Vendor ID */
    uint16_t product_id; /**< Product ID */
    uint16_t product_version; /**< Product Version */
} st_ble_dis_pnp_id_t;

/***************************************************************************//**
 * @brief     Set PnP ID characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_SetPnpId(const st_ble_dis_pnp_id_t *p_value);

/***************************************************************************//**
 * @brief     Get PnP ID characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_GetPnpId(st_ble_dis_pnp_id_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Device Information Service
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief Device Information characteristic Index.
*******************************************************************************/
typedef enum {
    BLE_DIS_MFR_NAME_IDX,
    BLE_DIS_MODEL_NUM_IDX,
    BLE_DIS_SER_NUM_IDX,
    BLE_DIS_HW_REV_IDX,
    BLE_DIS_FIRM_REV_IDX,
    BLE_DIS_SW_REV_IDX,
    BLE_DIS_SYS_ID_IDX,
    BLE_DIS_REG_CER_DATA_LIST_IDX,
    BLE_DIS_PNP_ID_IDX,
} st_ble_dis_char_idx_t;

/***************************************************************************//**
 * @brief Device Information event type.
*******************************************************************************/
typedef enum {
    /* Manufacturer Name */
    BLE_DIS_EVENT_MFR_NAME_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_MFR_NAME_IDX, BLE_SERVS_READ_REQ),
    /* Model Number */
    BLE_DIS_EVENT_MODEL_NUM_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_MODEL_NUM_IDX, BLE_SERVS_READ_REQ),
    /* Serial Number */
    BLE_DIS_EVENT_SER_NUM_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_SER_NUM_IDX, BLE_SERVS_READ_REQ),
    /* Hardware Revision */
    BLE_DIS_EVENT_HW_REV_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_HW_REV_IDX, BLE_SERVS_READ_REQ),
    /* Firmware Revision */
    BLE_DIS_EVENT_FIRM_REV_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_FIRM_REV_IDX, BLE_SERVS_READ_REQ),
    /* Software Revision */
    BLE_DIS_EVENT_SW_REV_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_SW_REV_IDX, BLE_SERVS_READ_REQ),
    /* System ID */
    BLE_DIS_EVENT_SYS_ID_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_SYS_ID_IDX, BLE_SERVS_READ_REQ),
    /* IEEE 11073-20601 Regulatory Certification Data List */
    BLE_DIS_EVENT_REG_CER_DATA_LIST_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_REG_CER_DATA_LIST_IDX, BLE_SERVS_READ_REQ),
    /* PnP ID */
    BLE_DIS_EVENT_PNP_ID_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_DIS_PNP_ID_IDX, BLE_SERVS_READ_REQ),
} e_ble_dis_event_t;

/***************************************************************************//**
 * @brief     Initialize Device Information service.
 * @param[in] cb Service callback.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_DIS_Init(ble_servs_app_cb_t cb);

#endif /* R_BLE_DIS_H */

/** @} */
