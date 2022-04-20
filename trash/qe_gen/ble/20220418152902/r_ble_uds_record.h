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

/**********************************************************************************************************************
 * File Name: r_ble_uds_record.h
 * Description: This file provides APIs to interface User Data Service Record
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 * @defgroup gls Glucose Service Server
 * @{
 * @ingroup profile
 * @brief   This file provides APIs to interface Glucose Service Records.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "r_ble_uds.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#ifndef R_BLE_UDS_RECORD_H
#define R_BLE_UDS_RECORD_H

#define BLE_UDS_DB_MAX_NUM_OF_RECORDS (10)
#define BLE_UDS_DB_INVALID_INDEX      (0xFFFF)

typedef enum
{
    BLE_UDS_USER_RECORD_STATUS_FREE   = 0, /**< Record is free to hold new user information */
    BLE_UDS_USER_RECORD_STATUS_USED   = 1, /**< Record is not free, has allocated to user */
    BLE_UDS_USER_RECORD_STATUS_LOCKED = 2, /**< Record is locked for an user */
} e_ble_uds_user_record_status_t;

typedef struct
{
    e_ble_uds_user_record_status_t            status;
    uint8_t                                   user_index;
    uint16_t                                  consent_code;
    uint8_t                                   no_of_attempts;
    st_ble_uds_first_name_t                   first_name;
    st_ble_uds_last_name_t                    last_name;
    st_ble_uds_email_address_t                email_address;
    uint8_t                                   age;
    st_ble_uds_date_of_birth_t                date_of_birth;
    uint8_t                                   gender;
    uint16_t                                  weight;
    uint16_t                                  height;
    uint8_t                                   vo2_max;
    uint8_t                                   heart_rate_max;
    uint8_t                                   resting_heart_rate;
    uint8_t                                   max_recommended_heart_rate;
    uint8_t                                   aerobic_threshold;
    uint8_t                                   anaerobic_threshold;
    uint8_t                                   sport_type_for_aerobic_and_anaerobic_thresholds;
    st_ble_uds_date_of_threshold_assessment_t date_of_threshold_assessment;
    uint16_t                                  waist_circumference;
    uint16_t                                  hip_circumference;
    uint8_t                                   fat_burn_heart_rate_lower_limit;
    uint8_t                                   fat_burn_heart_rate_upper_limit;
    uint8_t                                   aerobic_heart_rate_lower_limit;
    uint8_t                                   aerobic_heart_rate_upper_limit;
    uint8_t                                   anaerobic_heart_rate_lower_limit;
    uint8_t                                   anaerobic_heart_rate_upper_limit;
    st_ble_uds_five_zone_heart_rate_limits_t  five_zone_heart_rate_limits;
    st_ble_uds_three_zone_heart_rate_limits_t three_zone_heart_rate_limits;
    uint8_t                                   two_zone_heart_rate_limit;
    st_ble_uds_language_t                     language;
    uint32_t                                  database_change_increment;
} st_ble_uds_record_t;

void uds_db_init (void);

st_ble_uds_record_t *uds_db_get_record (uint8_t index);

uint8_t uds_db_add_user (uint16_t consent_code);

ble_status_t uds_db_delete_user (uint8_t user_index);

ble_status_t uds_db_set_current_user(uint8_t user_index, uint16_t consent_code);

void uds_db_populate_current_user_data(uint8_t user_index);

void uds_db_update_current_user_data(uint8_t user_index, st_ble_uds_record_t *p_app_user_data);

void uds_db_unlock_user(uint8_t user_index);

#endif /* R_BLE_UDS_RECORD_H */

/** @} */
