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
 * File Name: r_ble_uds.h
 * Description : This module implements User Data Service Server.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @file
 * @defgroup uds User Data Service Server 
 * @{
 * @ingroup profile
 * @brief   This service exposes user-related data in the sports and fitness environment. This allows remote access and update of user data by a Client as well as the synchronization of user data between a Server and a Client. 
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "profile_cmn/r_ble_serv_common.h"
#include "gatt_db.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_UDS_H
#define R_BLE_UDS_H

/*******************************************************************************************************************//**
 * @brief User Data Access Not Permitted error code.
***********************************************************************************************************************/
#define BLE_UDS_USER_DATA_ACCESS_NOT_PERMITTED              (BLE_ERR_GROUP_GATT | 0x80)

/*******************************************************************************************************************//**
 * @brief User Data Control Point Opcode Parameter length.
***********************************************************************************************************************/
#define BLE_UDS_CONTROL_POINT_OPCODE_PARAMETER_LEN          (18)

/*******************************************************************************************************************//**
 * @brief User Data Control Point Response Parameter length.
***********************************************************************************************************************/
#define BLE_UDS_CONTROL_POINT_RESPONSE_PARAMETER_LEN        (17)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief User Data Service event data.
***********************************************************************************************************************/
typedef struct
{
    uint16_t  conn_hdl;  /**< Connection handle */
    uint16_t  param_len; /**< Event parameter length */
    void     *p_param;    /**< Event parameter */
} st_ble_uds_evt_data_t;

/*******************************************************************************************************************//**
 * @brief User Data Service event callback.
***********************************************************************************************************************/
typedef void (*ble_uds_app_cb_t)(uint16_t type, ble_status_t result, st_ble_uds_evt_data_t *p_data);

/*******************************************************************************************************************//**
 * @brief User Data Service event type.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_EVENT_USER_CNTRL_PNT_CLI_CNFG_ENABLED, /**< User Control Point characteristic cli cnfg enabled event */
    BLE_UDS_EVENT_USER_CNTRL_PNT_CLI_CNFG_DISABLED, /**< User Control Point characteristic cli cnfg disabled event */
    BLE_UDS_EVENT_USER_CNTRL_PNT_HDL_VAL_CNF, /**< User Control Point characteristic handle value confirmation event */
    BLE_UDS_EVENT_DB_CHANGE_INCREMENT_CLI_CNFG_ENABLED, /**< Database Change Increment characteristic cli cnfg
                                                              enabled event */
    BLE_UDS_EVENT_DB_CHANGE_INCREMENT_CLI_CNFG_DISABLED, /**< Database Change Increment characteristic cli cnfg
                                                               disabled event */
    BLE_UDS_EVENT_FIRST_NAME_WRITE_REQ, /**< First Name characteristic write request event */
    BLE_UDS_EVENT_LAST_NAME_WRITE_REQ, /**< Last Name characteristic write request event */
    BLE_UDS_EVENT_EMAIL_ADDR_WRITE_REQ, /**< Email Address characteristic write request event */
    BLE_UDS_EVENT_AGE_WRITE_REQ, /**< Age characteristic write request event */
    BLE_UDS_EVENT_DATE_OF_BIRTH_WRITE_REQ, /**< Date of Birth characteristic write request event */
    BLE_UDS_EVENT_GENDER_WRITE_REQ, /**< Gender characteristic write request event */
    BLE_UDS_EVENT_WEIGHT_WRITE_REQ, /**< Weight characteristic write request event */
    BLE_UDS_EVENT_HEIGHT_WRITE_REQ, /**< Height characteristic write request event */
    BLE_UDS_EVENT_VO2_MAX_WRITE_REQ, /**< VO2 Max characteristic write request event */
    BLE_UDS_EVENT_HEART_RATE_MAX_WRITE_REQ, /**< Heart Rate Max characteristic write request event */
    BLE_UDS_EVENT_RESTING_HEART_RATE_WRITE_REQ, /**< Resting Heart Rate characteristic write request event */
    BLE_UDS_EVENT_MAX_RECOMMENDED_HEART_RATE_WRITE_REQ, /**< Maximum Recommended Heart Rate characteristic write
                                                            request event */
    BLE_UDS_EVENT_AEROBIC_THRESHOLD_WRITE_REQ, /**< Aerobic Threshold characteristic write request event */
    BLE_UDS_EVENT_ANAEROBIC_THRESHOLD_WRITE_REQ, /**< Anaerobic Threshold characteristic write request event */
    BLE_UDS_EVENT_SPORT_TYPE_FOR_THRESHOLD_WRITE_REQ, /**< Sport Type for Aerobic and Anaerobic Thresholds
                                                                             characteristic write request event */
    BLE_UDS_EVENT_DATE_OF_THRESHOLD_ASSESSMENT_WRITE_REQ, /**< Date of Threshold Assessment characteristic write request event */
    BLE_UDS_EVENT_WAIST_CIRCUMFERENCE_WRITE_REQ, /**< Waist Circumference characteristic write request event */
    BLE_UDS_EVENT_HIP_CIRCUMFERENCE_WRITE_REQ, /**< Hip Circumference characteristic write request event */
    BLE_UDS_EVENT_FAT_BURN_HEART_RATE_LOWER_LIMIT_WRITE_REQ, /**< Fat Burn Heart Rate Lower Limit characteristic write request event */
    BLE_UDS_EVENT_FAT_BURN_HEART_RATE_UPPER_LIMIT_WRITE_REQ, /**< Fat Burn Heart Rate Upper Limit characteristic write request event */
    BLE_UDS_EVENT_AEROBIC_HEART_RATE_LOWER_LIMIT_WRITE_REQ, /**< Aerobic Heart Rate Lower Limit characteristic write request event */
    BLE_UDS_EVENT_AEROBIC_HEART_RATE_UPPER_LIMIT_WRITE_REQ, /**< Aerobic Heart Rate Upper Limit characteristic write request event */
    BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_LOWER_LIMIT_WRITE_REQ, /**< Anaerobic Heart Rate Lower Limit characteristic write request event */
    BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_UPPER_LIMIT_WRITE_REQ, /**< Anaerobic Heart Rate Upper Limit characteristic write request event */
    BLE_UDS_EVENT_FIVE_ZONE_HEART_RATE_LIMITS_WRITE_REQ, /**< Five Zone Heart Rate Limits characteristic write request event */
    BLE_UDS_EVENT_THREE_ZONE_HEART_RATE_LIMITS_WRITE_REQ, /**< Three Zone Heart Rate Limits characteristic write request event */
    BLE_UDS_EVENT_TWO_ZONE_HEART_RATE_LIMIT_WRITE_REQ, /**< Two Zone Heart Rate Limit characteristic write request event */
    BLE_UDS_EVENT_DB_CHANGE_INCREMENT_WRITE_REQ, /**< Database Change Increment characteristic write request event */
    BLE_UDS_EVENT_USER_CNTRL_PNT_WRITE_REQ, /**< User Control Point characteristic write request event */
    BLE_UDS_EVENT_LANGUAGE_WRITE_REQ, /**< Language characteristic write request event */
    BLE_UDS_EVENT_FIRST_NAME_READ_REQ, /**< First Name characteristic read request event */
    BLE_UDS_EVENT_LAST_NAME_READ_REQ, /**< Last Name characteristic read request event */
    BLE_UDS_EVENT_EMAIL_ADDR_READ_REQ, /**< Email Address characteristic read request event */
    BLE_UDS_EVENT_AGE_READ_REQ, /**< Age characteristic read request event */
    BLE_UDS_EVENT_DATE_OF_BIRTH_READ_REQ, /**< Date of Birth characteristic read request event */
    BLE_UDS_EVENT_GENDER_READ_REQ, /**< Gender characteristic read request event */
    BLE_UDS_EVENT_WEIGHT_READ_REQ, /**< Weight characteristic read request event */
    BLE_UDS_EVENT_HEIGHT_READ_REQ, /**< Height characteristic read request event */
    BLE_UDS_EVENT_VO2_MAX_READ_REQ, /**< VO2 Max characteristic read request event */
    BLE_UDS_EVENT_HEART_RATE_MAX_READ_REQ, /**< Heart Rate Max characteristic read request event */
    BLE_UDS_EVENT_RESTING_HEART_RATE_READ_REQ, /**< Resting Heart Rate characteristic read request event */
    BLE_UDS_EVENT_MAX_RECOMMENDED_HEART_RATE_READ_REQ, /**< Maximum Recommended Heart Rate characteristic read request event */
    BLE_UDS_EVENT_AEROBIC_THRESHOLD_READ_REQ, /**< Aerobic Threshold characteristic read request event */
    BLE_UDS_EVENT_ANAEROBIC_THRESHOLD_READ_REQ, /**< Anaerobic Threshold characteristic read request event */
    BLE_UDS_EVENT_SPORT_TYPE_FOR_THRESHOLD_READ_REQ, /**< Sport Type for Aerobic and Anaerobic Thresholds
                                                                            characteristic read request event */
    BLE_UDS_EVENT_DATE_OF_THRESHOLD_ASSESSMENT_READ_REQ, /**< Date of Threshold Assessment characteristic read request event */
    BLE_UDS_EVENT_WAIST_CIRCUMFERENCE_READ_REQ, /**< Waist Circumference characteristic read request event */
    BLE_UDS_EVENT_HIP_CIRCUMFERENCE_READ_REQ, /**< Hip Circumference characteristic read request event */
    BLE_UDS_EVENT_FAT_BURN_HEART_RATE_LOWER_LIMIT_READ_REQ, /**< Fat Burn Heart Rate Lower Limit characteristic read request event */
    BLE_UDS_EVENT_FAT_BURN_HEART_RATE_UPPER_LIMIT_READ_REQ, /**< Fat Burn Heart Rate Upper Limit characteristic read request event */
    BLE_UDS_EVENT_AEROBIC_HEART_RATE_LOWER_LIMIT_READ_REQ, /**< Aerobic Heart Rate Lower Limit characteristic read request event */
    BLE_UDS_EVENT_AEROBIC_HEART_RATE_UPPER_LIMIT_READ_REQ, /**< Aerobic Heart Rate Upper Limit characteristic read request event */
    BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_LOWER_LIMIT_READ_REQ, /**< Anaerobic Heart Rate Lower Limit characteristic read request event */
    BLE_UDS_EVENT_ANAEROBIC_HEART_RATE_UPPER_LIMIT_READ_REQ, /**< Anaerobic Heart Rate Upper Limit characteristic read request event */
    BLE_UDS_EVENT_FIVE_ZONE_HEART_RATE_LIMITS_READ_REQ, /**< Five Zone Heart Rate Limits characteristic read request event */
    BLE_UDS_EVENT_THREE_ZONE_HEART_RATE_LIMITS_READ_REQ, /**< Three Zone Heart Rate Limits characteristic read request event */
    BLE_UDS_EVENT_TWO_ZONE_HEART_RATE_LIMIT_READ_REQ, /**< Two Zone Heart Rate Limit characteristic read request event */
    BLE_UDS_EVENT_DB_CHANGE_INCREMENT_READ_REQ, /**< Database Change Increment characteristic read request event */
    BLE_UDS_EVENT_USER_INDEX_READ_REQ, /**< User Index characteristic read request event */
    BLE_UDS_EVENT_LANGUAGE_READ_REQ, /**< Language characteristic read request event */
} e_ble_uds_event_t;

/*******************************************************************************************************************//**
 * @brief Month enumeration.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_DATE_OF_BIRTH_MONTH_MONTH_IS_NOT_KNOWN = 0, /**< Unknown Month of Birth */
    BLE_UDS_DATE_OF_BIRTH_MONTH_JANUARY            = 1, /**< Birth Month - January */
    BLE_UDS_DATE_OF_BIRTH_MONTH_FEBRUARY           = 2, /**< Birth Month - February*/
    BLE_UDS_DATE_OF_BIRTH_MONTH_MARCH              = 3, /**< Birth Month - March */
    BLE_UDS_DATE_OF_BIRTH_MONTH_APRIL              = 4, /**< Birth Month - April */
    BLE_UDS_DATE_OF_BIRTH_MONTH_MAY                = 5, /**< Birth Month - May */
    BLE_UDS_DATE_OF_BIRTH_MONTH_JUNE               = 6, /**< Birth Month - June */
    BLE_UDS_DATE_OF_BIRTH_MONTH_JULY               = 7, /**< Birth Month - July */
    BLE_UDS_DATE_OF_BIRTH_MONTH_AUGUST             = 8, /**< Birth Month - August */
    BLE_UDS_DATE_OF_BIRTH_MONTH_SEPTEMBER          = 9, /**< Birth Month - September */
    BLE_UDS_DATE_OF_BIRTH_MONTH_OCTOBER            = 10, /**< Birth Month - October */
    BLE_UDS_DATE_OF_BIRTH_MONTH_NOVEMBER           = 11, /**< Birth Month - November */
    BLE_UDS_DATE_OF_BIRTH_MONTH_DECEMBER           = 12, /**< Birth Month - December */
} e_ble_uds_date_of_birth_t;

/*******************************************************************************************************************//**
 * @brief Gender enumeration.
***********************************************************************************************************************/
typedef enum
{
    BLE_UDS_GENDER_GENDER_MALE        = 0, /**< Gender - Male */
    BLE_UDS_GENDER_GENDER_FEMALE      = 1, /**< Gender - Female */
    BLE_UDS_GENDER_GENDER_UNSPECIFIED = 2, /**< Gender - Unspecified */
} e_ble_uds_gender_t;

/*******************************************************************************************************************//**
 * @brief Sport Type for Aerobic and Anaerobic Thresholds enumeration.
***********************************************************************************************************************/
typedef enum
{
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_UNSPECIFIED                 = 0, /**< Threshold Unspecified */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_RUNNING__TREADMILL_         = 1, /**< Running (Treadmill) */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_CYCLING__ERGOMETER_         = 2, /**< Cycling (Ergometer) */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_ROWING__ERGOMETER_          = 3, /**< Rowing (Ergometer) */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_CROSS_TRAINING__ELLIPTICAL_ = 4, /**< Cross Training (Elliptical) */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_CLIMBING                    = 5, /**< Climbing */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_SKIING                      = 6, /**< Skiing */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_SKATING                     = 7, /**< Skating */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_ARM_EXERCISING              = 8, /**< Arm exercising */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_LOWER_BODY_EXERCISING       = 9, /**< Lower body exercising */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_UPPER_BODY_EXERCISING       = 10, /**< Upper body exercising */
    BLE_UDS_SPORT_TYPE_FOR_THRESHOLD_WHOLE_BODY_EXERCISING       = 11, /**< Whole body exercising */
} e_ble_uds_sport_type_for_aerobic_and_anaerobic_thresholds_t;

/*******************************************************************************************************************//**
 * @brief Month enumeration.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_MONTH_IS_NOT_KNOWN = 0, /**< Month is not known */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_JANUARY            = 1, /**< January */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_FEBRUARY           = 2, /**< February */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_MARCH              = 3, /**< March */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_APRIL              = 4, /**< April */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_MAY                = 5, /**< May */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_JUNE               = 6, /**< June */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_JULY               = 7, /**< July */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_AUGUST             = 8, /**< August */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_SEPTEMBER          = 9, /**< September */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_OCTOBER            = 10, /**< October */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_NOVEMBER           = 11, /**< November */
    BLE_UDS_DATE_OF_THRESHOLD_ASSESSMENT_MONTH_DECEMBER           = 12, /**< December */
} e_ble_uds_date_of_threshold_assessment_t;

/*******************************************************************************************************************//**
 * @brief User Index enumeration.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_USER_INDEX_UNKNOWN_USER = 255, /**< Unknown User */
} e_ble_uds_user_index_t;

/*******************************************************************************************************************//**
 * @brief OpCode enumeration.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_USER_CNTRL_PNT_OPCODE_RESERVED_FOR_FUTURE_USE = 0, /**< Reserved for future use */
    BLE_UDS_USER_CNTRL_PNT_OPCODE_REGISTER_NEW_USER       = 1, /**< Register New User */
    BLE_UDS_USER_CNTRL_PNT_OPCODE_CONSENT                 = 2, /**< Consent */
    BLE_UDS_USER_CNTRL_PNT_OPCODE_DELETE_USER_DATA        = 3, /**< Delete User Data */
    BLE_UDS_USER_CNTRL_PNT_OPCODE_RESPONSE_CODE           = 32, /**< Response Code */
} e_ble_uds_user_control_point_opcode_t;

/*******************************************************************************************************************//**
 * @brief Response Value enumeration.
***********************************************************************************************************************/
typedef enum 
{
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_RESERVED_FOR_FUTURE_USE = 0, /**< Reserved for future use */
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_SUCCESS                 = 1, /**< Success */
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OP_CODE_NOT_SUPPORTED   = 2, /**< Op Code not supported */
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_INVALID_PARAMETER       = 3, /**< Invalid Parameter */
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_OPERATION_FAILED        = 4, /**< Operation Failed */
    BLE_UDS_USER_CNTRL_PNT_RESPONSE_VALUE_USER_NOT_AUTHORIZED     = 5, /**< User Not Authorized */
} e_ble_uds_user_control_point_response_value_t;

/*******************************************************************************************************************//**
 * @brief User Data Service initialization parameters.
***********************************************************************************************************************/
typedef struct 
{
    ble_uds_app_cb_t cb; /**< User Data Service event callback */
} st_ble_uds_init_param_t;

/*******************************************************************************************************************//**
 * @brief User Data Service connection parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint16_t database_change_increment_cli_cnfg; /**< Database Change Increment characteristic cli cnfg */
    uint16_t user_control_point_cli_cnfg; /**< User Control Point characteristic cli cnfg */
} st_ble_uds_connect_param_t;

/*******************************************************************************************************************//**
 * @brief User Data Service disconnection parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint16_t database_change_increment_cli_cnfg; /**< Database Change Increment characteristic cli cnfg */
    uint16_t user_control_point_cli_cnfg; /**< User Control Point characteristic cli cnfg */
} st_ble_uds_disconnect_param_t;

/*******************************************************************************************************************//**
 * @brief First Name characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t first_name[BLE_UDS_FIRST_NAME_LEN]; /**< First Name value */
} st_ble_uds_first_name_t;

/*******************************************************************************************************************//**
 * @brief Last Name characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t last_name[BLE_UDS_LAST_NAME_LEN]; /**< Last Name value */
} st_ble_uds_last_name_t;

/*******************************************************************************************************************//**
 * @brief Email Address characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t email_address[BLE_UDS_EMAIL_ADDR_LEN]; /**< Email Address value */
} st_ble_uds_email_address_t;

/*******************************************************************************************************************//**
 * @brief Date of Birth characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint16_t year;  /**< Year value */
    uint8_t  month; /**< Month value */
    uint8_t  day;  /**< Day value */
} st_ble_uds_date_of_birth_t;

/*******************************************************************************************************************//**
 * @brief Date of Threshold Assessment characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint16_t year; /**< Year value */
    uint8_t  month; /**< Month value */
    uint8_t  day; /**< Day value */
} st_ble_uds_date_of_threshold_assessment_t;

/*******************************************************************************************************************//**
 * @brief Five Zone Heart Rate Limits characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t five_zone_heart_rate_limits___very_light___light_limit; /**< Five Zone Heart Rate Limits - Very light / 
                                                                    Light Limit value */
    uint8_t five_zone_heart_rate_limits___light___moderate_limit; /**< Five Zone Heart Rate Limits - Light / Moderate 
                                                                  Limit value */
    uint8_t five_zone_heart_rate_limits___moderate___hard_limit; /**< Five Zone Heart Rate Limits - Moderate / Hard Limit 
                                                                 value */
    uint8_t five_zone_heart_rate_limits___hard___maximum_limit; /**< Five Zone Heart Rate Limits - Hard / Maximum Limit 
                                                                value */
} st_ble_uds_five_zone_heart_rate_limits_t;

/*******************************************************************************************************************//**
 * @brief Three Zone Heart Rate Limits characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t three_zone_heart_rate_limits___light__fat_burn____moderate__aerobic__limit; /**< Three zone Heart Rate Limits
                                                                 - Light (Fat burn) / Moderate (Aerobic) Limit value */
    uint8_t three_zone_heart_rate_limits___moderate__aerobic____hard__anaerobic__limit; /**< Three zone Heart Rate Limits
                                                                    - Moderate (Aerobic) / Hard (Anaerobic) Limit value */
} st_ble_uds_three_zone_heart_rate_limits_t;

/*******************************************************************************************************************//**
 * @brief User Control Point characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t  opcode;                                                           /**< OpCode value */
    uint16_t consent_code;                                                     /**< Consent Code value */
    uint8_t  user_index;                                                       /**< User Index value */
    uint8_t  parameter[BLE_UDS_CONTROL_POINT_OPCODE_PARAMETER_LEN];            /**< Parameter value */
    uint8_t  response_code;                                                    /**< Response Code value */
    uint8_t  request_opcode;                                                   /**< Request OpCode value */
    uint8_t  response_value;                                                   /**< Response Value value */
    uint8_t  response_parameter[BLE_UDS_CONTROL_POINT_RESPONSE_PARAMETER_LEN]; /**< Response Parameter value */
} st_ble_uds_user_control_point_t;

/*******************************************************************************************************************//**
 * @brief Language characteristic parameters.
***********************************************************************************************************************/
typedef struct 
{
    uint8_t language[BLE_UDS_LANGUAGE_LEN]; /**< Language value */
} st_ble_uds_language_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern uint8_t     g_current_user_index;
extern uint16_t    g_uds_conn_hdl;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief     Initialize User Data Service.
 * @details   This function shall be called once at startup.
 * @param[in] p_param Pointer to User Data Service initialization parameters.
 * @return
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_Init(const st_ble_uds_init_param_t *p_param);

/*******************************************************************************************************************//**
 * @brief     Perform User Data Service connection settings.
 * @details   This function shall be called on each connection establishment.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_param    Pointer to Connection parameters.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_Connect(uint16_t conn_hdl, const st_ble_uds_connect_param_t *p_param);

/*******************************************************************************************************************//**
 * @brief     Retrieve User Data Service connection specific settings before disconnection.
 * @details   This function shall be called on each disconnection.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_param  Pointer to Disconnection parameters.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_Disconnect(uint16_t conn_hdl, st_ble_uds_disconnect_param_t *p_param);

/*******************************************************************************************************************//**
 * @brief      Get First Name characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved First Name characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetFirstName(st_ble_uds_first_name_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set First Name characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to First Name characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetFirstName(const st_ble_uds_first_name_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Last Name characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Last Name characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetLastName(st_ble_uds_last_name_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Last Name characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Last Name characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetLastName(const st_ble_uds_last_name_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Email Address characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Email Address characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetEmailAddress(st_ble_uds_email_address_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Email Address characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Email Address characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetEmailAddress(const st_ble_uds_email_address_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Age characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Age characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAge(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Age characteristic value to local GATT database.
 * @param[in] app_value Age characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAge(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Date of Birth characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Date of Birth characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetDateOfBirth(st_ble_uds_date_of_birth_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Date of Birth characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Date of Birth characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetDateOfBirth(const st_ble_uds_date_of_birth_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Gender characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Gender characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetGender(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Gender characteristic value to local GATT database.
 * @param[in] app_value Gender characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetGender(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Weight characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Weight characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetWeight(uint16_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Weight characteristic value to local GATT database.
 * @param[in] app_value Weight characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetWeight(uint16_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Height characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Height characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetHeight(uint16_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Height characteristic value to local GATT database.
 * @param[in] app_value Height characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetHeight(uint16_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get VO2 Max characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved VO2 Max characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetVo2Max(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set VO2 Max characteristic value to local GATT database.
 * @param[in] app_value VO2 Max characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetVo2Max(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Heart Rate Max characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Heart Rate Max characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetHeartRateMax(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Heart Rate Max characteristic value to local GATT database.
 * @param[in] app_value Heart Rate Max characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetHeartRateMax(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Resting Heart Rate characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Resting Heart Rate characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetRestingHeartRate(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Resting Heart Rate characteristic value to local GATT database.
 * @param[in] app_value Resting Heart Rate characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetRestingHeartRate(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Maximum Recommended Heart Rate characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to Retrieved Maximum Recommended Heart Rate characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetMaximumRecommendedHeartRate(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Maximum Recommended Heart Rate characteristic value to local GATT database.
 * @param[in] app_value Maximum Recommended Heart Rate characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetMaximumRecommendedHeartRate(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Aerobic Threshold characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Aerobic Threshold characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAerobicThreshold(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Aerobic Threshold characteristic value to local GATT database.
 * @param[in] app_value Aerobic Threshold characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAerobicThreshold(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Anaerobic Threshold characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Anaerobic Threshold characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAnaerobicThreshold(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Anaerobic Threshold characteristic value to local GATT database.
 * @param[in] app_value Anaerobic Threshold characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAnaerobicThreshold(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Sport Type for Aerobic and Anaerobic Thresholds characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Sport Type for Aerobic and Anaerobic Thresholds characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetSportTypeForAerobicAndAnaerobicThresholds(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Sport Type for Aerobic and Anaerobic Thresholds characteristic value to local GATT database.
 * @param[in] app_value Sport Type for Aerobic and Anaerobic Thresholds characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetSportTypeForAerobicAndAnaerobicThresholds(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Date of Threshold Assessment characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Date of Threshold Assessment characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetDateOfThresholdAssessment(st_ble_uds_date_of_threshold_assessment_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Date of Threshold Assessment characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Date of Threshold Assessment characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetDateOfThresholdAssessment(const st_ble_uds_date_of_threshold_assessment_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Waist Circumference characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Waist Circumference characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetWaistCircumference(uint16_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Waist Circumference characteristic value to local GATT database.
 * @param[in] app_value Waist Circumference characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetWaistCircumference(uint16_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Hip Circumference characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Hip Circumference characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetHipCircumference(uint16_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Hip Circumference characteristic value to local GATT database.
 * @param[in] app_value Hip Circumference characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetHipCircumference(uint16_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Fat Burn Heart Rate Lower Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Fat Burn Heart Rate Lower Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetFatBurnHeartRateLowerLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Fat Burn Heart Rate Lower Limit characteristic value to local GATT database.
 * @param[in] app_value Fat Burn Heart Rate Lower Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetFatBurnHeartRateLowerLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Fat Burn Heart Rate Upper Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Fat Burn Heart Rate Upper Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetFatBurnHeartRateUpperLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Fat Burn Heart Rate Upper Limit characteristic value to local GATT database.
 * @param[in] app_value Fat Burn Heart Rate Upper Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetFatBurnHeartRateUpperLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Aerobic Heart Rate Lower Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Aerobic Heart Rate Lower Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAerobicHeartRateLowerLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Aerobic Heart Rate Lower Limit characteristic value to local GATT database.
 * @param[in] app_value Aerobic Heart Rate Lower Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAerobicHeartRateLowerLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Aerobic Heart Rate Upper Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Aerobic Heart Rate Upper Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAerobicHeartRateUpperLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Aerobic Heart Rate Upper Limit characteristic value to local GATT database.
 * @param[in] app_value Aerobic Heart Rate Upper Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAerobicHeartRateUpperLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Anaerobic Heart Rate Lower Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Anaerobic Heart Rate Lower Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAnaerobicHeartRateLowerLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Anaerobic Heart Rate Lower Limit characteristic value to local GATT database.
 * @param[in] app_value Anaerobic Heart Rate Lower Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAnaerobicHeartRateLowerLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Anaerobic Heart Rate Upper Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Anaerobic Heart Rate Upper Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetAnaerobicHeartRateUpperLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Anaerobic Heart Rate Upper Limit characteristic value to local GATT database.
 * @param[in] app_value Anaerobic Heart Rate Upper Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetAnaerobicHeartRateUpperLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Five Zone Heart Rate Limits characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Five Zone Heart Rate Limits characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetFiveZoneHeartRateLimits(st_ble_uds_five_zone_heart_rate_limits_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Five Zone Heart Rate Limits characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Five Zone Heart Rate Limits characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetFiveZoneHeartRateLimits(const st_ble_uds_five_zone_heart_rate_limits_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Three Zone Heart Rate Limits characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Three Zone Heart Rate Limits characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetThreeZoneHeartRateLimits(st_ble_uds_three_zone_heart_rate_limits_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Three Zone Heart Rate Limits characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Three Zone Heart Rate Limits characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetThreeZoneHeartRateLimits(const st_ble_uds_three_zone_heart_rate_limits_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief      Get Two Zone Heart Rate Limit characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Two Zone Heart Rate Limit characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetTwoZoneHeartRateLimit(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Two Zone Heart Rate Limit characteristic value to local GATT database.
 * @param[in] app_value Two Zone Heart Rate Limit characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetTwoZoneHeartRateLimit(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Database Change Increment characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Database Change Increment characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetDatabaseChangeIncrement(uint32_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Database Change Increment characteristic value to local GATT database.
 * @param[in] app_value Database Change Increment characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetDatabaseChangeIncrement(uint32_t app_value);

/*******************************************************************************************************************//**
 * @brief     Send Database Change Increment notification.
 * @param[in] conn_hdl  Connection handle.
 * @param[in] app_value Database Change Increment value to send.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_NotifyDatabaseChangeIncrement(uint16_t conn_hdl, uint32_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get User Index characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved User Index characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetUserIndex(uint8_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set User Index characteristic value to local GATT database.
 * @param[in] app_value User Index characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetUserIndex(uint8_t app_value);

/*******************************************************************************************************************//**
 * @brief      Get Language characteristic value from local GATT database.
 * @param[out] p_app_value Pointer to retrieved Language characteristic value.
 * @return     @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_GetLanguage(st_ble_uds_language_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Set Language characteristic value to local GATT database.
 * @param[in] p_app_value Pointer to Language characteristic value to set.
 * @return    @ref ble_status_t
***********************************************************************************************************************/
ble_status_t R_BLE_UDS_SetLanguage(const st_ble_uds_language_t *p_app_value);

/*******************************************************************************************************************//**
 * @brief     Return version of the UDC service server.
 * @return    version
***********************************************************************************************************************/
uint32_t R_BLE_UDS_GetVersion(void);

#endif /* R_BLE_UDS_H */

/** @} */
