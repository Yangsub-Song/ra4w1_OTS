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
 * File Name   : r_ble_ots.h
 * Description : The header file for Object Transfer Service Server.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 * @defgroup ots Object Transfer Service Server
 * @{
 * @ingroup profile
 * @brief   This service provides management and control features supporting bulk data transfers which occur via a separate L2CAP connection oriented channel. The Client is enabled to create and delete objects and to execute an action using the currently selected object. The selected object can be written, updated or read via an Object Transfer Channel opened by the Client. The generation of a checksum covering a part or the whole of the object contents is included as an optional feature. This service provides a general method for a Client to select and initiate the transfer of any type of object.
 **********************************************************************************************************************/
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef R_BLE_OTS_H
#define R_BLE_OTS_H

/***************************************************************************//**
 * @brief uuid structure
*******************************************************************************/
typedef struct {
    uint16_t uuid_16;
    uint8_t uuid_128[16];
}st_ble_ots_uuid_value_t;

typedef struct {
    uint8_t uuid_type;
    st_ble_ots_uuid_value_t uuid;
}st_ble_ots_uuid_t;

/*----------------------------------------------------------------------------------------------------------------------
    OTS Feature Characteristic
----------------------------------------------------------------------------------------------------------------------*/
      
      
/***************************************************************************//**
 * @brief OTS Feature OACP Features value structure.
*******************************************************************************/
typedef struct {
    bool is_oacp_create_op_code_supported; /**< OACP Create Op Code Supported */
    bool is_oacp_delete_op_code_supported; /**< OACP Delete Op Code Supported */
    bool is_oacp_calculate_checksum_op_code_supported; /**< OACP Calculate Checksum Op Code Supported */
    bool is_oacp_execute_op_code_supported; /**< OACP Execute Op Code Supported */
    bool is_oacp_read_op_code_supported; /**< OACP Read Op Code Supported */
    bool is_oacp_write_op_code_supported; /**< OACP Write Op Code Supported */
    bool is_appending_additional_data_to_objects_supported; /**< Appending Additional Data to Objects Supported */
    bool is_truncation_of_objects_supported; /**< Truncation of Objects Supported */
    bool is_patching_of_objects_supported; /**< Patching of Objects Supported */
    bool is_oacp_abort_op_code_supported; /**< OACP Abort Op Code Supported */
} st_ble_lns_feat_oacp_features_t;

/***************************************************************************//**
 * @brief OTS Feature OLCP Features value structure.
*******************************************************************************/
typedef struct {
    bool is_olcp_go_to_op_code_supported; /**< OLCP Go To Op Code Supported */
    bool is_olcp_order_op_code_supported; /**< OLCP Order Op Code Supported */
    bool is_olcp_request_number_of_objects_op_code_supported; /**< OLCP Request Number of Objects Op Code Supported */
    bool is_olcp_clear_marking_op_code_supported; /**< OLCP Clear Marking Op Code Supported */
} st_ble_lns_feat_olcp_features_t;

/***************************************************************************//**
 * @brief OTS Feature value structure.
*******************************************************************************/
typedef struct {
    st_ble_lns_feat_oacp_features_t oacp_features; /**< OACP Features */
    st_ble_lns_feat_olcp_features_t olcp_features; /**< OLCP Features */
} st_ble_ots_feat_t;

/***************************************************************************//**
 * @brief     Set OTS Feature characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetFeat(const st_ble_ots_feat_t *p_value);

/***************************************************************************//**
 * @brief     Get OTS Feature characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetFeat(st_ble_ots_feat_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Name Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/** Object Name Length */
#define BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN (512)

/***************************************************************************//**
 * @brief     Set Object Name characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjName(const st_ble_seq_data_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Name characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjName(st_ble_seq_data_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Type Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Object Type characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjType(const st_ble_ots_uuid_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Type characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjType(st_ble_ots_uuid_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Size Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object Size value structure.
*******************************************************************************/
typedef struct {
    uint32_t current_size; /**< Current Size */
    uint32_t allocated_size; /**< Allocated Size */
} st_ble_ots_obj_size_t;

/***************************************************************************//**
 * @brief     Set Object Size characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjSize(const st_ble_ots_obj_size_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Size characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjSize(st_ble_ots_obj_size_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object First-Created Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Object First-Created characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjFirstCreated(const st_ble_date_time_t *p_value);

/***************************************************************************//**
 * @brief     Get Object First-Created characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjFirstCreated(st_ble_date_time_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Last-Modified Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief     Set Object Last-Modified characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjLastModified(const st_ble_date_time_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Last-Modified characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjLastModified(st_ble_date_time_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object ID Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object ID Object ID enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_ID_OBJECT_ID_RESERVED_FOR_THE_DIRECTORY_LISTING_OBJECT = 0, /**< Reserved for the Directory Listing Object */
} e_ble_ots_obj_id_object_id_t;

/***************************************************************************//**
 * @brief     Set Object ID characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjId(const uint8_t *p_value);

/***************************************************************************//**
 * @brief     Get Object ID characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjId(uint8_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Properties Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object Properties value structure.
*******************************************************************************/
typedef struct {
    bool is_deletion_of_this_object_is_permitted; /**< Deletion of this object is permitted */
    bool is_execution_of_this_object_is_permitted; /**< Execution of this object is permitted */
    bool is_reading_this_object_is_permitted; /**< Reading this object is permitted */
    bool is_writing_data_to_this_object_is_permitted; /**< Writing data to this object is permitted */
    bool is_appending_data_to_this_object_that_increases_its_allocated_size_is_permitted; /**< Appending data to this object that increases its Allocated Size is permitted */
    bool is_truncation_of_this_object_is_permitted; /**< Truncation of this object is permitted */
    bool is_patching_this_object_by_overwriting_some_of_the_object_s_existing_contents_is_permitted; /**< Patching this object by overwriting some of the object's existing contents is permitted */
    bool is_this_object_is_a_marked_object; /**< This object is a marked object */
} st_ble_ots_obj_prop_t;

/***************************************************************************//**
 * @brief     Set Object Properties characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjProp(const st_ble_ots_obj_prop_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Properties characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjProp(st_ble_ots_obj_prop_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Action Control Point Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/** Parameter Length */
#define BLE_OTS_OBJ_ACTION_CP_PARAMETER_LEN (20)

/***************************************************************************//**
 * @brief Object Action Control Point Op Code enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_CREATE = 1, /**< Create a new, empty object */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_DELETE = 2, /**< Delete the Current Object */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_CALCULATE_CHECKSUM = 3, /**< Compare a checksum */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_EXECUTE = 4, /**< Use the Current Object to perform an operation */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_READ = 5, /**< Send object data through Object Transfer Channel */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_WRITE = 6, /**< Open the Current Object for writing and prepare to accept data */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_ABORT = 7, /**< Cease sending previously requested data through Object Transfer Channel */
    BLE_OTS_OBJ_ACTION_CP_OP_CODE_RESPONSE_CODE = 96, /**< Used to identify the response to this Control Point */
} e_ble_ots_obj_action_cp_op_code_t;

/***************************************************************************//**
 * @brief Object Action Control Point Result Code enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_SUCCESS = 1, /**< Response for successful operation */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OP_CODE_NOT_SUPPORTED = 2, /**< Response if unsupported Op Code is received */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_PARAMETER = 3, /**< Parameter received does not meet the requirements */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INSUFFICIENT_RESOURCES = 4, /**< Size parameter exceeds the available memory */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_INVALID_OBJECT = 5, /**< Current Object is an Invalid Object */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_CHANNEL_UNAVAILABLE = 6, /**< Object Transfer Channel was not available for use */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_UNSUPPORTED_TYPE = 7, /**< OACP procedure Type parameter is not supported by the Server */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_PROCEDURE_NOT_PERMITTED = 8, /**< Requested procedure is not permitted */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OBJECT_LOCKED = 9, /**< Current Object is temporarily locked */
    BLE_OTS_OBJ_ACTION_CP_RESULT_CODE_OPERATION_FAILED = 10, /**< Operation Failed */
} e_ble_ots_obj_action_cp_result_code_t;

/***************************************************************************//**
 * @brief Object Action Control Point value structure.
*******************************************************************************/
typedef struct {
    uint8_t op_code; /**< Op Code */
    uint32_t size; /**< size */
    st_ble_ots_uuid_t type; /**< type */
    uint32_t offset; /**< offset */
    uint32_t length; /**< length */
    uint8_t mode; /**< mode */
    uint8_t request_op_code; /**< Request Op Code */
    uint8_t result_code; /**< Result Code */
    uint32_t response_parameter; /**< Response Parameter */
} st_ble_ots_obj_action_cp_t;

/***************************************************************************//**
 * @brief     Send indication of  Object Action Control Point characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_IndicateObjActionCp(uint16_t conn_hdl, const st_ble_ots_obj_action_cp_t *p_value);

/***************************************************************************//**
 * @brief     Set Object Action Control Point cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjActionCpCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Action Control Point cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjActionCpCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object List Control Point Characteristic
----------------------------------------------------------------------------------------------------------------------*/

/** Parameter Length */
#define BLE_OTS_OBJ_LIST_CP_PARAMETER_LEN (6)

/***************************************************************************//**
 * @brief Object List Control Point Op Code enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_CP_OP_CODE_FIRST = 1, /**< Make the first object in the list become the Current Object */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_LAST = 2, /**< Make the last object in the list become the Current Object */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_PREVIOUS = 3, /**< Make the object immediately preceding the Current Object in the list become the Current Object */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_NEXT = 4, /**< Make the object immediately after the Current Object in the list become the Current Object */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_GO_TO = 5, /**< Select an object by specifying its Object ID */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_ORDER = 6, /**< Arrange the list of objects in order */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_REQUEST_NUMBER_OF_OBJECTS = 7, /**< Report the total number of objects */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_CLEAR_MARKING = 8, /**< Update the marking of objects */
    BLE_OTS_OBJ_LIST_CP_OP_CODE_RESPONSE_CODE = 112, /**< Used to identify the response to this Control Point */
} e_ble_ots_obj_list_cp_op_code_t;

/***************************************************************************//**
 * @brief Object List Control Point Result Code enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_SUCCESS = 1, /**< Response for successful operation */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OP_CODE_NOT_SUPPORTED = 2, /**< Response if unsupported Op Code is received */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_INVALID_PARAMETER = 3, /**< Parameter received does not meet the requirements */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OPERATION_FAILED = 4, /**< Requested procedure failed */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OUT_OF_BOUNDS = 5, /**< Selected object is beyond the first or last object in the current list */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_TOO_MANY_OBJECTS = 6, /**< Procedure failed due to too many objects */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_NO_OBJECT = 7, /**< Procedure failed due to there being zero objects in the current list */
    BLE_OTS_OBJ_LIST_CP_RESULT_CODE_OBJECT_ID_NOT_FOUND = 8, /**< Procedure failed due to there being no object with the requested Object ID */
} e_ble_ots_obj_list_cp_result_code_t;

/***************************************************************************//**
 * @brief Object List Control Point List Sort Order enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_NAME__ASCENDING = 1, /**< Order the list by object name, ascending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_TYPE__ASCENDING = 2, /**< Order the list by object name, ascending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_CURRENT_SIZE__ASCENDING = 3, /**< Order the list by object current size, ascending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_FIRST_CREATED_TIMESTAMP__ASCENDING = 4, /**< Order the list by object first-created timestamp, ascending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_LAST_MODIFIED_TIMESTAMP__ASCENDING = 5, /**< Order the list by object last-modified timestamp, ascending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_NAME__DESCENDING = 17, /**< Order the list by object name, descending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_TYPE__DESCENDING = 18, /**< Order the list by object type, descending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_CURRENT_SIZE__DESCENDING = 19, /**< Order the list by object current size, descending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_FIRST_CREATED_TIMESTAMP__DESCENDING = 20, /**< Order the list by object first-created timestamp, descending */
    BLE_OTS_OBJ_LIST_CP_LIST_SORT_ORDER_ORDER_THE_LIST_BY_OBJECT_LAST_MODIFIED_TIMESTAMP__DESCENDING = 21, /**< Order the list by object last-modified timestamp, descending */
} e_ble_ots_obj_list_cp_list_sort_order_t;

/***************************************************************************//**
 * @brief Object List Control Point value structure.
*******************************************************************************/
typedef struct {
    uint8_t op_code; /**< Op Code */
    uint8_t parameter[BLE_OTS_OBJ_LIST_CP_PARAMETER_LEN]; /**< Parameter */
    uint8_t request_op_code; /**< Request Op Code */
    uint8_t result_code; /**< Result Code */
    uint32_t response_parameter; /**< Response Parameter */
    uint8_t list_sort_order; /**< List Sort Order */
} st_ble_ots_obj_list_cp_t;

/***************************************************************************//**
 * @brief     Send indication of  Object List Control Point characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_IndicateObjListCp(uint16_t conn_hdl, const st_ble_ots_obj_list_cp_t *p_value);

/***************************************************************************//**
 * @brief     Set Object List Control Point cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjListCpCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Object List Control Point cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjListCpCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 0 Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object List Filter 0 Filter enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NO_FILTER = 0, /**< No Filter */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_STARTS_WITH = 1, /**< Name Starts With */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_ENDS_WITH = 2, /**< Name Ends With */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_CONTAINS = 3, /**< Name Contains */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_IS_EXACTLY = 4, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_OBJECT_TYPE = 5, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CREATED_BETWEEN = 6, /**< Created between */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_MODIFIED_BETWEEN = 7, /**< Modified between */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CURRENT_SIZE_BETWEEN = 8, /**< Current Size between */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_ALLOCATED_SIZE_BETWEEN = 9, /**< Allocated Size between */
    BLE_OTS_OBJ_LIST_FILTER_0_FILTER_MARKED_OBJECTS = 10, /**< Marked Objects */
} e_ble_ots_obj_list_filter_0_filter_t;

/***************************************************************************//**
 * @brief Object List Filter 0 value structure.
*******************************************************************************/
typedef struct {
    uint8_t filter; /**< Filter */
    st_ble_seq_data_t string; /**< string */
    st_ble_ots_uuid_t uuid; /**< uuid */
    st_ble_date_time_t timestamp1; /**< timestamp1 */
    st_ble_date_time_t timestamp2; /**< timestamp2 */
    uint32_t size1; /**< size1 */
    uint32_t size2; /**< size2 */
} st_ble_ots_obj_list_filter_0_t;

/***************************************************************************//**
 * @brief     Set Object List Filter 0 characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjListFilter0(const st_ble_ots_obj_list_filter_0_t *p_value);

/***************************************************************************//**
 * @brief     Get Object List Filter 0 characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjListFilter0(st_ble_ots_obj_list_filter_0_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 1 Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object List Filter 1 Filter enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NO_FILTER = 0, /**< No Filter */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_STARTS_WITH = 1, /**< Name Starts With */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_ENDS_WITH = 2, /**< Name Ends With */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_CONTAINS = 3, /**< Name Contains */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_NAME_IS_EXACTLY = 4, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_OBJECT_TYPE = 5, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CREATED_BETWEEN = 6, /**< Created between */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_MODIFIED_BETWEEN = 7, /**< Modified between */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_CURRENT_SIZE_BETWEEN = 8, /**< Current Size between */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_ALLOCATED_SIZE_BETWEEN = 9, /**< Allocated Size between */
    BLE_OTS_OBJ_LIST_FILTER_1_FILTER_MARKED_OBJECTS = 10, /**< Marked Objects */
} e_ble_ots_obj_list_filter_1_filter_t;

/***************************************************************************//**
 * @brief Object List Filter 1 value structure.
*******************************************************************************/
typedef struct {
    uint8_t filter; /**< Filter */
    st_ble_seq_data_t string; /**< string */
    st_ble_ots_uuid_t uuid; /**< uuid */
    st_ble_date_time_t timestamp1; /**< timestamp1 */
    st_ble_date_time_t timestamp2; /**< timestamp2 */
    uint32_t size1; /**< size1 */
    uint32_t size2; /**< size2 */
} st_ble_ots_obj_list_filter_1_t;

/***************************************************************************//**
 * @brief     Set Object List Filter 1 characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjListFilter1(const st_ble_ots_obj_list_filter_1_t *p_value);

/***************************************************************************//**
 * @brief     Get Object List Filter 1 characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjListFilter1(st_ble_ots_obj_list_filter_1_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object List Filter 2 Characteristic
----------------------------------------------------------------------------------------------------------------------*/
/***************************************************************************//**
 * @brief Object List Filter 2 Filter enumeration.
*******************************************************************************/
typedef enum {
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NO_FILTER = 0, /**< No Filter */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_STARTS_WITH = 1, /**< Name Starts With */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_ENDS_WITH = 2, /**< Name Ends With */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_CONTAINS = 3, /**< Name Contains */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_NAME_IS_EXACTLY = 4, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_OBJECT_TYPE = 5, /**< Name is Exactly */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CREATED_BETWEEN = 6, /**< Created between */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_MODIFIED_BETWEEN = 7, /**< Modified between */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_CURRENT_SIZE_BETWEEN = 8, /**< Current Size between */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_ALLOCATED_SIZE_BETWEEN = 9, /**< Allocated Size between */
    BLE_OTS_OBJ_LIST_FILTER_2_FILTER_MARKED_OBJECTS = 10, /**< Marked Objects */
} e_ble_ots_obj_list_filter_2_filter_t;

/***************************************************************************//**
 * @brief Object List Filter 2 value structure.
*******************************************************************************/
typedef struct {
    uint8_t filter; /**< Filter */
    st_ble_seq_data_t string; /**< string */
    st_ble_ots_uuid_t uuid; /**< uuid */
    st_ble_date_time_t timestamp1; /**< timestamp1 */
    st_ble_date_time_t timestamp2; /**< timestamp2 */
    uint32_t size1; /**< size1 */
    uint32_t size2; /**< size2 */
} st_ble_ots_obj_list_filter_2_t;

/***************************************************************************//**
 * @brief     Set Object List Filter 2 characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjListFilter2(const st_ble_ots_obj_list_filter_2_t *p_value);

/***************************************************************************//**
 * @brief     Get Object List Filter 2 characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjListFilter2(st_ble_ots_obj_list_filter_2_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Changed Characteristic
----------------------------------------------------------------------------------------------------------------------*/

      
/***************************************************************************//**
 * @brief Object Changed Flags value structure.
*******************************************************************************/
typedef struct {
    bool is_source_of_change; /**< Source of Change */
    bool is_change_occurred_to_the_object_contents; /**< Change occurred to the object contents */
    bool is_change_occurred_to_the_object_metadata; /**< Change occurred to the object metadata */
    bool is_object_creation; /**< Object Creation */
    bool is_object_deletion; /**< Object Deletion */
} st_ble_lns_obj_changed_flags_t;

/***************************************************************************//**
 * @brief Object Changed value structure.
*******************************************************************************/
typedef struct {
    st_ble_lns_obj_changed_flags_t flags; /**< Flags */
    uint8_t object_id[6]; /**< Object ID */
} st_ble_ots_obj_changed_t;

/***************************************************************************//**
 * @brief     Send indication of  Object Changed characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_IndicateObjChanged(uint16_t conn_hdl, const st_ble_ots_obj_changed_t *p_value);

/***************************************************************************//**
 * @brief     Set Object Changed cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle. 
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_SetObjChangedCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Object Changed cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle. 
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_GetObjChangedCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Object Transfer Service Server
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief An attempt was made to write a value that is invalid or not supported by this Server for a reason other than the attribute permissions.
*******************************************************************************/
#define BLE_OTS_WRITE_REQUEST_REJECTED_ERROR (BLE_ERR_GROUP_GATT | 0x80)

/***************************************************************************//**
 * @brief An attempt was made to read or write to an Object Metadata characteristic while the Current Object was an Invalid Object (see Section 1.8 in the PDF specification).
*******************************************************************************/
#define BLE_OTS_OBJECT_NOT_SELECTED_ERROR (BLE_ERR_GROUP_GATT | 0x81)

/***************************************************************************//**
 * @brief The Server is unable to service the Read Request or Write Request because it exceeds the concurrency limit of the service.
*******************************************************************************/
#define BLE_OTS_CONCURRENCY_LIMIT_EXCEEDED_ERROR (BLE_ERR_GROUP_GATT | 0x82)

/***************************************************************************//**
 * @brief The requested object name was rejected because the name was already in use by an existing object on the Server.
*******************************************************************************/
#define BLE_OTS_OBJECT_NAME_ALREADY_EXISTS_ERROR (BLE_ERR_GROUP_GATT | 0x83)

/***************************************************************************//**
 * @brief The Client Characteristic Configuration descriptor is not configured according to the requirements of the service.
*******************************************************************************/
#define BLE_OTS_CLI_CNFG_IMPROPERLY_CONFIGURED_ERROR (BLE_ERR_GROUP_GATT | 0xFD)

/***************************************************************************//**
 * @brief Object Transfer Service characteristic Index.
*******************************************************************************/
typedef enum {
    BLE_OTS_FEAT_IDX,
    BLE_OTS_OBJ_NAME_IDX,
    BLE_OTS_OBJ_TYPE_IDX,
    BLE_OTS_OBJ_SIZE_IDX,
    BLE_OTS_OBJ_FIRST_CREATED_IDX,
    BLE_OTS_OBJ_LAST_MODIFIED_IDX,
    BLE_OTS_OBJ_ID_IDX,
    BLE_OTS_OBJ_PROP_IDX,
    BLE_OTS_OBJ_ACTION_CP_IDX,
    BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_IDX,
    BLE_OTS_OBJ_LIST_CP_IDX,
    BLE_OTS_OBJ_LIST_CP_CLI_CNFG_IDX,
    BLE_OTS_OBJ_LIST_FILTER_0_IDX,
    BLE_OTS_OBJ_LIST_FILTER_1_IDX,
    BLE_OTS_OBJ_LIST_FILTER_2_IDX,
    BLE_OTS_OBJ_CHANGED_IDX,
    BLE_OTS_OBJ_CHANGED_CLI_CNFG_IDX,
} e_ble_ots_char_idx_t;

/***************************************************************************//**
 * @brief Object Transfer Service event type.
*******************************************************************************/
typedef enum {
    /* OTS Feature */
    BLE_OTS_EVENT_FEAT_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_FEAT_IDX, BLE_SERVS_READ_REQ),
    /* Object Name */
    BLE_OTS_EVENT_OBJ_NAME_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_NAME_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_NAME_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_NAME_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_NAME_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_NAME_IDX, BLE_SERVS_READ_REQ),
    /* Object Type */
    BLE_OTS_EVENT_OBJ_TYPE_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_TYPE_IDX, BLE_SERVS_READ_REQ),
    /* Object Size */
    BLE_OTS_EVENT_OBJ_SIZE_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_SIZE_IDX, BLE_SERVS_READ_REQ),
    /* Object First-Created */
    BLE_OTS_EVENT_OBJ_FIRST_CREATED_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_FIRST_CREATED_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_FIRST_CREATED_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_FIRST_CREATED_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_FIRST_CREATED_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_FIRST_CREATED_IDX, BLE_SERVS_READ_REQ),
    /* Object Last-Modified */
    BLE_OTS_EVENT_OBJ_LAST_MODIFIED_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LAST_MODIFIED_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LAST_MODIFIED_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LAST_MODIFIED_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LAST_MODIFIED_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LAST_MODIFIED_IDX, BLE_SERVS_READ_REQ),
    /* Object ID */
    BLE_OTS_EVENT_OBJ_ID_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ID_IDX, BLE_SERVS_READ_REQ),
    /* Object Properties */
    BLE_OTS_EVENT_OBJ_PROP_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_PROP_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_PROP_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_PROP_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_PROP_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_PROP_IDX, BLE_SERVS_READ_REQ),
    /* Object Action Control Point */
    BLE_OTS_EVENT_OBJ_ACTION_CP_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_ACTION_CP_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_ACTION_CP_HDL_VAL_CNF = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_IDX, BLE_SERVS_HDL_VAL_CNF),
    BLE_OTS_EVENT_OBJ_ACTION_CP_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_ACTION_CP_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_ACTION_CP_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_ACTION_CP_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
    /* Object List Control Point */
    BLE_OTS_EVENT_OBJ_LIST_CP_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LIST_CP_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LIST_CP_HDL_VAL_CNF = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_IDX, BLE_SERVS_HDL_VAL_CNF),
    BLE_OTS_EVENT_OBJ_LIST_CP_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LIST_CP_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LIST_CP_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_CP_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
    /* Object List Filter 0 */
    BLE_OTS_EVENT_OBJ_LIST_FILTER_0_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_0_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_0_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_0_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_0_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_0_IDX, BLE_SERVS_READ_REQ),
    /* Object List Filter 1 */
    BLE_OTS_EVENT_OBJ_LIST_FILTER_1_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_1_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_1_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_1_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_1_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_1_IDX, BLE_SERVS_READ_REQ),
    /* Object List Filter 2 */
    BLE_OTS_EVENT_OBJ_LIST_FILTER_2_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_2_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_2_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_2_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_LIST_FILTER_2_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_LIST_FILTER_2_IDX, BLE_SERVS_READ_REQ),
    /* Object Changed */
    BLE_OTS_EVENT_OBJ_CHANGED_HDL_VAL_CNF = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_CHANGED_IDX, BLE_SERVS_HDL_VAL_CNF),
    BLE_OTS_EVENT_OBJ_CHANGED_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_CHANGED_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_OTS_EVENT_OBJ_CHANGED_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_CHANGED_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_OTS_EVENT_OBJ_CHANGED_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_OTS_OBJ_CHANGED_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
} e_ble_ots_event_t;

/***************************************************************************//**
 * @brief     Initialize Object Transfer Service Server.
 * @param[in] cb Service callback.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_OTS_Init(ble_servs_app_cb_t cb);

#endif /* R_BLE_OTS_H */

/** @} */
