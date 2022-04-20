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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name  : r_ble_ots_object.h
 * Description: This file provides APIs to interface Object Transfer Service Object Records
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 * @defgroup ots Object Transfer Service Server
 * @{
 * @ingroup profile
 * @brief   This file provides APIs to interface Object Transfer Service Object Records.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "r_ble_ots.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#ifndef R_BLE_OTS_OBJECT_H
#define R_BLE_OTS_OBJECT_H

#define BLE_OTS_DB_MAX_NUM_OF_OBJECTS (10)
#define BLE_OTS_DB_INVALID_INDEX      (0xFFFF)

typedef enum
{
    BLE_OTS_DB_INVALID_OBJECT = 0xFF,
    BLE_OTS_DB_VALID_OBJECT = 0x00,
    BLE_OTS_DB_FILTERED_0 = 0x01,
    BLE_OTS_DB_FILTERED_1 = 0x02,
    BLE_OTS_DB_FILTERED_2 = 0x04,
    BLE_OTS_DB_WILL_DELETE,
} e_ble_ots_gls_db_valid_t;

/* Object Structure */
typedef struct
{
    uint8_t                valid;
    uint16_t               next_index;
    uint16_t               prev_index;

    uint8_t                obj_name[BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN];
    uint8_t                data[1028];
    st_ble_ots_uuid_t      obj_type;
    st_ble_date_time_t     obj_first_created;
    st_ble_date_time_t     obj_last_modified;
    st_ble_ots_obj_size_t  obj_size;
    uint8_t                obj_id[6];
    st_ble_ots_obj_prop_t  obj_properties;
    bool                   is_valid;
    bool                   is_locked;
} st_ble_ots_object_t;


/***********************************************************************************************************************
 * Function Name: gls_db_init
 * Description  : This function initializes the OTS object database.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void ots_db_init(void);

/***********************************************************************************************************************
 * Function Name: gls_db_get_oldest_index
 * Description  : This function returns the oldest object index.
 * Arguments    : none
 * Return Value : oldest object index
 **********************************************************************************************************************/
uint16_t ots_db_get_oldest_index(void);

/***********************************************************************************************************************
 * Function Name: gls_db_get_newest_index
 * Description  : This function returns the newest object index.
 * Arguments    : none
 * Return Value : newest object index
 **********************************************************************************************************************/
uint16_t ots_db_get_newest_index(void);

/***********************************************************************************************************************
 * Function Name: gls_db_get_next_index
 * Description  : This function returns the next object index of the specified index.
 * Arguments    : index - object index
 * Return Value : next object index
 **********************************************************************************************************************/
uint16_t ots_db_get_next_index(uint16_t index);

/***********************************************************************************************************************
 * Function Name: gls_db_get_object
 * Description  : This function returns the object at the given index.
 * Arguments    : index - object index in the database
 * Return Value : Pointer to st_ble_ots_object_t
 **********************************************************************************************************************/
st_ble_ots_object_t *ots_db_get_object(uint16_t index);

/***********************************************************************************************************************
 * Function Name: ots_db_mark_delete_object
 * Description  : This function marks delete objects at the given index from the database.
 * Arguments    : index - object index to delete
 * Return Value : none
 **********************************************************************************************************************/
void ots_db_mark_delete_object(uint16_t index);

/***********************************************************************************************************************
 * Function Name: gls_db_delete_object
 * Description  : This function delete objects at the given index from the database.
 * Arguments    : index - object index to delete
 * Return Value : none
 **********************************************************************************************************************/
void ots_db_delete_object(st_ble_ots_object_t *p_object);

/***********************************************************************************************************************
 * Function Name: ots_db_create_object
 * Description  : This function creates objects
 * Arguments    : index - object index to delete
 * Return Value : sPointer to st_ble_ots_object_t
 **********************************************************************************************************************/
void ots_db_create_object(st_ble_ots_obj_size_t *obj_size, st_ble_ots_uuid_t *obj_type);

/***********************************************************************************************************************
 * Function Name: ots_db_store_object
 * Description  : This function stores objects
 * Arguments    : obj_size  Pointer to Object size
                  obj_type  Pointer to Object type
 * Return Value : sPointer to st_ble_ots_object_t
 **********************************************************************************************************************/
void ots_db_store_object(uint16_t index, st_ble_ots_obj_size_t *obj_size, st_ble_ots_uuid_t *obj_type);

/***********************************************************************************************************************
 * Function Name: object_list_cb
 * Description  : Callback to Object List Filter Characteristic
 * Arguments    : Pointer to p_app_value
 * Return Value : Nothing
 **********************************************************************************************************************/
void object_list_cb(st_ble_ots_obj_list_filter_0_t *p_app_value);

/***********************************************************************************************************************
 * Function Name: ots_db_num_of_objects
 * Description  : This function returns number of objects
 * Arguments    : Nothing
 * Return Value : Returns number of objects
 **********************************************************************************************************************/
uint16_t ots_db_num_of_objects();

/***********************************************************************************************************************
 * Function Name: ots_db_get_object_id
 * Description  : This function returns objects
 * Arguments    : Pointer to p_app_value
 * Return Value : Pointer to st_ble_ots_object_t
 **********************************************************************************************************************/
st_ble_ots_object_t * ots_db_get_object_id(st_ble_ots_obj_list_cp_t *p_app_value);

/***********************************************************************************************************************
 * Function Name: ots_db_clear_marking
 * Description  : This function clears the marked property of objects
 * Arguments    : Nothing
 * Return Value : Nothing
 **********************************************************************************************************************/
void ots_db_clear_marking(void);

/***********************************************************************************************************************
 * Function Name: ots_sort_object_names
 * Description  : This function sorts the objects by object name
 * Arguments    : Nothing
 * Return Value : Nothing
 **********************************************************************************************************************/
void ots_sort_object_names(void);

/***********************************************************************************************************************
 * Function Name: ots_object_list_filter_cb
 * Description  : This function is callback function for the object list filter 
 * Arguments    : p_app_value  Pointer to the object list filter data structure
 * Return Value : Nothing
 **********************************************************************************************************************/
void ots_object_list_filter_cb(st_ble_ots_obj_list_filter_0_t *p_app_value);

#endif /* R_BLE_OTS_OBJECT_H */

/** @} */
