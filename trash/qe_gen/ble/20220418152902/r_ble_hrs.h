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
 * File Name: r_ble_hrs.h
 * Description : The header file for Heart Rate service.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @file
 * @defgroup hrs Heart Rate Service Server
 * @{
 * @ingroup profile
 * @brief   This service exposes heart rate and other data from a Heart Rate Sensor intended for fitness applications.
 **********************************************************************************************************************/

#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef R_BLE_HRS_H
#define R_BLE_HRS_H

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Measurement Characteristic
----------------------------------------------------------------------------------------------------------------------*/

      
/***************************************************************************//**
 * @brief Heart Rate Measurement Flags value structure.
*******************************************************************************/
typedef struct {
    bool is_heart_rate_value_16_bit; /**< Heart Rate Value Format bit */
    bool is_sensor_contact_feature_supported; /**< Sensor Contact Feature Supported */
    bool is_sensor_contact_detected; /**< Sensor Contact Detected */
    bool is_energy_expended_included; /**< Energy Expended Status bit */
    bool is_rr_interval_included; /**< RR-Interval bit */
} st_ble_hrs_meas_flags_t;

/***************************************************************************//**
 * @brief Heart Rate Measurement value structure.
*******************************************************************************/
typedef struct {
    st_ble_hrs_meas_flags_t flags; /**< Flags */
    uint16_t heart_rate_measurement_value; /**< Heart Rate Measurement Value */
    uint16_t rr_interval[9]; /**< RR-Interval */
    uint8_t number_of_rr_interval; /**< Number of RR-Interval */
} st_ble_hrs_meas_t;

/***************************************************************************//**
 * @brief     Send notification of  Heart Rate Measurement characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_NotifyMeas(uint16_t conn_hdl, const st_ble_hrs_meas_t *p_value);

/***************************************************************************//**
 * @brief     Add Energy Expended value.
 * @param[in] p_value  Energy expended value to add.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_AddEnergyExpended(uint16_t *p_value);

/***************************************************************************//**
 * @brief     Set Heart Rate Measurement cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_SetMeasCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Heart Rate Measurement cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_GetMeasCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Body Sensor Location Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Body Sensor Location Body Sensor Location enumeration.
*******************************************************************************/
typedef enum {
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_OTHER = 0, /**< Other */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_CHEST = 1, /**< Chest */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_WRIST = 2, /**< Wrist */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_FINGER = 3, /**< Finger */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_HAND = 4, /**< Hand */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_EAR_LOBE = 5, /**< Ear Lobe */
    BLE_HRS_BODY_SENSOR_LOCATION_BODY_SENSOR_LOCATION_FOOT = 6, /**< Foot */
} e_ble_hrs_body_sensor_location_body_sensor_location_t;

/***************************************************************************//**
 * @brief     Set Body Sensor Location characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_SetBodySensorLocation(const uint8_t *p_value);

/***************************************************************************//**
 * @brief     Get Body Sensor Location characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_GetBodySensorLocation(uint8_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Control Point Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Heart Rate Control Point Heart Rate Control Point enumeration.
*******************************************************************************/
typedef enum {
    BLE_HRS_HEART_RATE_CP_HEART_RATE_CONTROL_POINT_RESET_ENERGY_EXPENDED = 1, /**< Reset Energy Expended: resets the value of the Energy Expended field in the Heart Rate Measurement characteristic to 0 */
} e_ble_hrs_heart_rate_cp_heart_rate_control_point_t;

/*----------------------------------------------------------------------------------------------------------------------
    Heart Rate Service
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief Heart Rate Control Point Not Supported
*******************************************************************************/
#define BLE_HRS_HEART_RATE_CONTROL_POINT_NOT_SUPPORTED_ERROR (BLE_ERR_GROUP_GATT | 0x80)

/***************************************************************************//**
 * @brief Heart Rate characteristic Index.
*******************************************************************************/
typedef enum {
    BLE_HRS_MEAS_IDX,
    BLE_HRS_MEAS_CLI_CNFG_IDX,
    BLE_HRS_BODY_SENSOR_LOCATION_IDX,
    BLE_HRS_HEART_RATE_CP_IDX,
} e_ble_hrs_char_idx_t;

/***************************************************************************//**
 * @brief Heart Rate event type.
*******************************************************************************/
typedef enum {
    /* Heart Rate Measurement */
    /* Body Sensor Location */
    BLE_HRS_EVENT_BODY_SENSOR_LOCATION_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_HRS_BODY_SENSOR_LOCATION_IDX, BLE_SERVS_READ_REQ),
    /* Heart Rate Control Point */
    BLE_HRS_EVENT_HEART_RATE_CP_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_HRS_HEART_RATE_CP_IDX, BLE_SERVS_WRITE_REQ),
    BLE_HRS_EVENT_HEART_RATE_CP_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_HRS_HEART_RATE_CP_IDX, BLE_SERVS_WRITE_COMP),
} e_ble_hrs_event_t;

/***************************************************************************//**
 * @brief     Initialize Heart Rate service.
 * @param[in] cb Service callback.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_HRS_Init(ble_servs_app_cb_t cb);

#endif /* R_BLE_HRS_H */

/** @} */
