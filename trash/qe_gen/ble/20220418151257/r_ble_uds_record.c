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
 * File Name: r_ble_uds_record.c
 * Description : This module implements User Data Service Records
 **********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <string.h>
#include "r_ble_uds_record.h"
#include "gatt_db.h"
#include "r_ble_uds.h"

/***********************************************************************************************************************
 Macro definitions
***********************************************************************************************************************/
#define BLE_UDS_RECORD_PRV_MAX_CONSENT_TRIES      (4)

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
/*static uint16_t             gs_num_of_records;*/
static st_ble_uds_record_t  gs_records[BLE_UDS_DB_MAX_NUM_OF_RECORDS];

/***********************************************************************************************************************
 * Function Name: uds_db_init
 * Description  : This function initializes the UDS record database.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void uds_db_init (void)
{
    /*gs_num_of_records   = 0;*/

    for (uint8_t i = 0; i < BLE_UDS_DB_MAX_NUM_OF_RECORDS; i++)
    {
        gs_records[i].no_of_attempts = 0;

        memset(&gs_records[i], 0x00, sizeof(st_ble_uds_record_t));

        gs_records[i].status = BLE_UDS_USER_RECORD_STATUS_FREE;

        /* Type case enum to uint8_t */
        gs_records[i].user_index = (uint8_t)BLE_UDS_USER_INDEX_UNKNOWN_USER;
    }

}

/***********************************************************************************************************************
 * Function Name: uds_db_get_record
 * Description  : This function returns the record at the given index.
 * Arguments    : index - record index in the database
 * Return Value : record
 **********************************************************************************************************************/
st_ble_uds_record_t *uds_db_get_record (uint8_t index)
{
    if (BLE_UDS_DB_MAX_NUM_OF_RECORDS <= index)
    {
        return NULL;
    }

    if (BLE_UDS_USER_RECORD_STATUS_USED != gs_records[index].status)
    {
        /* record not free or locked */
        return NULL;
    }

    return &gs_records[index];

}

/***********************************************************************************************************************
 * Function Name: uds_db_add_user
 * Description  : This function adds new user data to the database.
 * Arguments    : p_record - Pointer to user data record
 * Return Value : user index
 **********************************************************************************************************************/
uint8_t uds_db_add_user(uint16_t consent_code)
{
    uint8_t i;
    uint8_t new_user_index = 0;

    if (1 == BLE_UDS_DB_MAX_NUM_OF_RECORDS)
    {
        /* Single user - overwrite the user data */
        gs_records[0].status                    = BLE_UDS_USER_RECORD_STATUS_USED;
        gs_records[0].user_index                = 0;
        gs_records[0].database_change_increment = 0;
        gs_records[0].consent_code              = consent_code;
        new_user_index                          = 0;
    }
    else
    {
        for (i = 0; i < BLE_UDS_DB_MAX_NUM_OF_RECORDS; i++)
        {
            if (BLE_UDS_USER_RECORD_STATUS_FREE == gs_records[i].status)
            {
                gs_records[i].status                    = BLE_UDS_USER_RECORD_STATUS_USED;
                gs_records[i].user_index                = i;
                gs_records[i].database_change_increment = 0;
                gs_records[i].consent_code              = consent_code;
                new_user_index                          = i;
                break;
            }
        }

        /* No space to save new record */
        if (BLE_UDS_DB_MAX_NUM_OF_RECORDS == i)
        {
            new_user_index = BLE_UDS_DB_MAX_NUM_OF_RECORDS;
        }
    }

    return new_user_index;
}

/***********************************************************************************************************************
 * Function Name: uds_db_delete_user
 * Description  : This function deletes the record at the given index from the database.
 * Arguments    : user_index - user index
 * Return Value : ble_status_t
 **********************************************************************************************************************/
ble_status_t uds_db_delete_user(uint8_t user_index)
{
    st_ble_uds_record_t *p_record;
    ble_status_t ret = BLE_SUCCESS;

    p_record = uds_db_get_record(user_index);

    if (NULL != p_record)
    {
        p_record->status = BLE_UDS_USER_RECORD_STATUS_FREE;

        /* Type case enum value to uint8_t */
        p_record->user_index = (uint8_t)BLE_UDS_USER_INDEX_UNKNOWN_USER;
    }
    else
    {
        /* User Not Found in the record */
        ret = BLE_ERR_NOT_FOUND;
    }

    return ret;
}

/***********************************************************************************************************************
 * Function Name: uds_db_set_current_user
 * Description  : This function checks the consent code for the given index and sets the current user.
 * Arguments    : user_index - user index
 *                consent_code - consent code for the user
 * Return Value : ble_status_t
 **********************************************************************************************************************/
ble_status_t uds_db_set_current_user(uint8_t user_index, uint16_t consent_code)
{
    st_ble_uds_record_t *p_record;
    ble_status_t ret = BLE_SUCCESS;

    p_record = uds_db_get_record(user_index);

    if (NULL != p_record)
    {
        if (p_record->consent_code != consent_code)
        {
            /* User Not Authorized */
            p_record->no_of_attempts++;
            if (BLE_UDS_RECORD_PRV_MAX_CONSENT_TRIES < p_record->no_of_attempts)
            {
                ret = BLE_ERR_LIMIT_EXCEEDED;

                /* set status flag to LOCKED */
                p_record->status = BLE_UDS_USER_RECORD_STATUS_LOCKED;
            }
            else
            {
                ret = BLE_ERR_INVALID_DATA;
            }
        }
        else
        {
            p_record->no_of_attempts            = 0;
            p_record->database_change_increment = 0;
        }
    }
    else
    {
        /* Not valid User */
        ret = BLE_ERR_NOT_FOUND;
    }

    return ret;
}

/***********************************************************************************************************************
 * Function Name: uds_db_populate_current_user_data
 * Description  : This function sets all the UDS characteristics values with values in the database.
 * Arguments    : user_index - user index
 * Return Value : none
 **********************************************************************************************************************/
void uds_db_populate_current_user_data(uint8_t user_index)
{
    st_ble_uds_record_t *p_record;
    /*ble_status_t ret = BLE_SUCCESS;*/

    p_record = uds_db_get_record(user_index);

    if (NULL != p_record)
    {
        R_BLE_UDS_SetFirstName( &p_record->first_name);
        R_BLE_UDS_SetLastName( &p_record->last_name);
        R_BLE_UDS_SetEmailAddress( &p_record->email_address);
        R_BLE_UDS_SetLanguage( &p_record->language);
        R_BLE_UDS_SetAge(p_record->age);
        R_BLE_UDS_SetDateOfBirth( &p_record->date_of_birth);
        R_BLE_UDS_SetGender(p_record->gender);
        R_BLE_UDS_SetWeight(p_record->weight);
        R_BLE_UDS_SetHeight(p_record->height);
        R_BLE_UDS_SetVo2Max(p_record->vo2_max);
        R_BLE_UDS_SetHeartRateMax(p_record->heart_rate_max);
        R_BLE_UDS_SetRestingHeartRate(p_record->resting_heart_rate);
        R_BLE_UDS_SetMaximumRecommendedHeartRate(p_record->max_recommended_heart_rate);
        R_BLE_UDS_SetAerobicThreshold(p_record->aerobic_threshold);
        R_BLE_UDS_SetAnaerobicThreshold(p_record->anaerobic_threshold);
        R_BLE_UDS_SetSportTypeForAerobicAndAnaerobicThresholds(
                p_record->sport_type_for_aerobic_and_anaerobic_thresholds);
        R_BLE_UDS_SetDateOfThresholdAssessment( &p_record->date_of_threshold_assessment);
        R_BLE_UDS_SetWaistCircumference(p_record->waist_circumference);
        R_BLE_UDS_SetHipCircumference(p_record->hip_circumference);
        R_BLE_UDS_SetFatBurnHeartRateLowerLimit(p_record->fat_burn_heart_rate_lower_limit);
        R_BLE_UDS_SetFatBurnHeartRateUpperLimit(p_record->fat_burn_heart_rate_upper_limit);
        R_BLE_UDS_SetAerobicHeartRateLowerLimit(p_record->aerobic_heart_rate_lower_limit);
        R_BLE_UDS_SetAerobicHeartRateUpperLimit(p_record->aerobic_heart_rate_upper_limit);
        R_BLE_UDS_SetAnaerobicHeartRateLowerLimit(p_record->anaerobic_heart_rate_lower_limit);
        R_BLE_UDS_SetAnaerobicHeartRateUpperLimit(p_record->anaerobic_heart_rate_upper_limit);
        R_BLE_UDS_SetFiveZoneHeartRateLimits( &p_record->five_zone_heart_rate_limits);
        R_BLE_UDS_SetThreeZoneHeartRateLimits( &p_record->three_zone_heart_rate_limits);
        R_BLE_UDS_SetTwoZoneHeartRateLimit(p_record->two_zone_heart_rate_limit);
        R_BLE_UDS_SetDatabaseChangeIncrement(p_record->database_change_increment);
        R_BLE_UDS_SetUserIndex(user_index);
    }
}

/***********************************************************************************************************************
 * Function Name: uds_db_update_current_user_data
 * Description  : This function updates current user data record and all the UDS characteristics values 
                  with values in the database.
 * Arguments    : user_index - user index
 *                p_app_user_data - pointer to the user data from application (UI)
 * Return Value : none
 **********************************************************************************************************************/
void uds_db_update_current_user_data(uint8_t user_index, st_ble_uds_record_t *p_app_user_data)
{
    st_ble_uds_record_t *p_record;
    /*ble_status_t ret = BLE_SUCCESS;*/

    p_record = uds_db_get_record(user_index);

    if ((NULL != p_record) && (NULL != p_app_user_data))
    {
        /* First increment the Database Change Increment value */
        p_app_user_data->database_change_increment = p_record->database_change_increment + 1;
        
        memcpy(p_record, p_app_user_data, sizeof(st_ble_uds_record_t));

        R_BLE_UDS_SetFirstName( &p_record->first_name);
        R_BLE_UDS_SetLastName( &p_record->last_name);
        R_BLE_UDS_SetEmailAddress( &p_record->email_address);
        R_BLE_UDS_SetLanguage( &p_record->language);
        R_BLE_UDS_SetAge(p_record->age);
        R_BLE_UDS_SetDateOfBirth( &p_record->date_of_birth);
        R_BLE_UDS_SetGender(p_record->gender);
        R_BLE_UDS_SetWeight(p_record->weight);
        R_BLE_UDS_SetHeight(p_record->height);
        R_BLE_UDS_SetVo2Max(p_record->vo2_max);
        R_BLE_UDS_SetHeartRateMax(p_record->heart_rate_max);
        R_BLE_UDS_SetRestingHeartRate(p_record->resting_heart_rate);
        R_BLE_UDS_SetMaximumRecommendedHeartRate(p_record->max_recommended_heart_rate);
        R_BLE_UDS_SetAerobicThreshold(p_record->aerobic_threshold);
        R_BLE_UDS_SetAnaerobicThreshold(p_record->anaerobic_threshold);
        R_BLE_UDS_SetSportTypeForAerobicAndAnaerobicThresholds(
                p_record->sport_type_for_aerobic_and_anaerobic_thresholds);
        R_BLE_UDS_SetDateOfThresholdAssessment( &p_record->date_of_threshold_assessment);
        R_BLE_UDS_SetWaistCircumference(p_record->waist_circumference);
        R_BLE_UDS_SetHipCircumference(p_record->hip_circumference);
        R_BLE_UDS_SetFatBurnHeartRateLowerLimit(p_record->fat_burn_heart_rate_lower_limit);
        R_BLE_UDS_SetFatBurnHeartRateUpperLimit(p_record->fat_burn_heart_rate_upper_limit);
        R_BLE_UDS_SetAerobicHeartRateLowerLimit(p_record->aerobic_heart_rate_lower_limit);
        R_BLE_UDS_SetAerobicHeartRateUpperLimit(p_record->aerobic_heart_rate_upper_limit);
        R_BLE_UDS_SetAnaerobicHeartRateLowerLimit(p_record->anaerobic_heart_rate_lower_limit);
        R_BLE_UDS_SetAnaerobicHeartRateUpperLimit(p_record->anaerobic_heart_rate_upper_limit);
        R_BLE_UDS_SetFiveZoneHeartRateLimits( &p_record->five_zone_heart_rate_limits);
        R_BLE_UDS_SetThreeZoneHeartRateLimits( &p_record->three_zone_heart_rate_limits);
        R_BLE_UDS_SetTwoZoneHeartRateLimit(p_record->two_zone_heart_rate_limit);
        R_BLE_UDS_SetDatabaseChangeIncrement(p_record->database_change_increment);

        /* Notify the connected client the database change increment characteristic value */
        R_BLE_UDS_NotifyDatabaseChangeIncrement(g_uds_conn_hdl, p_record->database_change_increment);
    }
}

/***********************************************************************************************************************
 * Function Name: uds_db_unlock_user
 * Description  : This function unlocks the user data in the database.
 * Arguments    : user_index - user index
 * Return Value : none
 **********************************************************************************************************************/
void uds_db_unlock_user(uint8_t user_index)
{
    st_ble_uds_record_t *p_record;
    /*ble_status_t ret = BLE_SUCCESS;*/

    p_record = uds_db_get_record(user_index);

    if (NULL != p_record)
    {
        p_record->status = BLE_UDS_USER_RECORD_STATUS_USED;
    }
}
