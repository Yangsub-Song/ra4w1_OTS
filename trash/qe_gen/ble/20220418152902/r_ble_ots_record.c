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
/***********************************************************************************************************************
 * File Name   : r_ble_ots_object.c
 * Description : This module implements Objects for Object Transfer Service
 **********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <string.h>
#include "r_ble_ots_record.h"
#include "gatt_db.h"

#define TEST_SIZE 1024

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
//static uint16_t             gs_next_sequence_number;
static uint16_t             gs_root_index = BLE_OTS_DB_INVALID_INDEX;
static uint16_t             gs_num_of_objects = 0;
uint16_t ots_obj_latest_index = BLE_OTS_DB_INVALID_INDEX;
static st_ble_ots_object_t  gs_objects[BLE_OTS_DB_MAX_NUM_OF_OBJECTS];

st_ble_ots_obj_size_t pts_obj_size1 = { 0 };
st_ble_ots_uuid_t pts_obj_type1 = { 0 };

char obj_name[BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN];
st_ble_seq_data_t object_name_string = {
    .data = (uint8_t*)obj_name,
    .len = BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN,
};

void ots_db_init (void)
{
    gs_root_index     = BLE_OTS_DB_INVALID_INDEX;
    gs_num_of_objects = 0;

    for (uint16_t i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        gs_objects[i].valid      = BLE_OTS_DB_INVALID_OBJECT;
        gs_objects[i].is_valid   = BLE_OTS_DB_INVALID_OBJECT;
        gs_objects[i].next_index = BLE_OTS_DB_INVALID_INDEX;
        gs_objects[i].prev_index = BLE_OTS_DB_INVALID_INDEX;

        memset(&gs_objects[i].obj_name, 0x00, (BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN * sizeof(char)));
        memset(&gs_objects[i].obj_type, 0x00, sizeof(gs_objects[i].obj_type));
        memset(&gs_objects[i].obj_first_created, 0x00, sizeof(gs_objects[i].obj_first_created));
        memset(&gs_objects[i].obj_last_modified, 0x00, sizeof(gs_objects[i].obj_last_modified));
        memset(&gs_objects[i].obj_size, 0x00, sizeof(gs_objects[i].obj_size));
        memset(&gs_objects[i].obj_id, 0x00, (6 * sizeof(uint8_t)));
        memset(&gs_objects[i].obj_properties, 0x00, sizeof(gs_objects[i].obj_properties));
    }
}

uint16_t ots_db_get_newest_index(void)
{
    return ots_obj_latest_index;
}

uint16_t ots_db_get_oldest_index(void)
{
    return gs_root_index;
}

uint16_t ots_db_get_next_index(uint16_t index)
{
    st_ble_ots_object_t *p_object = ots_db_get_object(index);
    
    if(NULL != p_object)
    {
        return p_object->next_index;
    }
    else
    {
        return BLE_OTS_DB_INVALID_INDEX;
    }
}

st_ble_ots_object_t *ots_db_get_object(uint16_t index)
{
    if (BLE_OTS_DB_MAX_NUM_OF_OBJECTS <= index)
    {
        return NULL;
    }

    if (BLE_OTS_DB_INVALID_OBJECT == gs_objects[index].valid)
    {
        return NULL;
    }

    return &gs_objects[index];
}

void ots_db_store_object(uint16_t index, st_ble_ots_obj_size_t *obj_size, st_ble_ots_uuid_t *obj_type)
{
    memcpy(&gs_objects[index].obj_size, obj_size, sizeof(gs_objects[index].obj_size));

    gs_objects[index].obj_type.uuid_type = obj_type->uuid_type;
    gs_objects[index].obj_type.uuid.uuid_16 = obj_type->uuid.uuid_16;

    gs_objects[index].valid = BLE_OTS_DB_VALID_OBJECT;
    gs_objects[index].is_valid = true;

    memcpy(gs_objects[index].obj_name, "Object 2", sizeof("Object 2"));
}

void ots_db_create_object(st_ble_ots_obj_size_t *obj_size, st_ble_ots_uuid_t *obj_type)
{
    /* First Object. */
    if (BLE_OTS_DB_INVALID_INDEX == gs_root_index)
    {
        gs_objects[0].prev_index = BLE_OTS_DB_INVALID_INDEX;
        gs_objects[0].next_index = BLE_OTS_DB_INVALID_INDEX;
        ots_db_store_object(0, obj_size, obj_type);
        gs_root_index = 0;
        ots_obj_latest_index = 0;
        gs_num_of_objects++;
    }
     
    /* Check for Free space, if exists, save the new object on it. */
    else 
    {
        uint8_t i = 0;
        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_INVALID_OBJECT == gs_objects[i].valid)
            {
                uint16_t newest_index = ots_db_get_newest_index();
                gs_objects[i].prev_index = newest_index;
                gs_objects[i].next_index = BLE_OTS_DB_INVALID_INDEX;
                ots_db_store_object(i, obj_size, obj_type);
                gs_objects[newest_index].next_index = i;
                ots_obj_latest_index = i;
                gs_num_of_objects++;
                break;
            }
        }
            
        if (i == BLE_OTS_DB_MAX_NUM_OF_OBJECTS)
        {
            /* No space to save a new object, hence remove oldest one. */
            uint16_t newest_index = ots_db_get_newest_index();
            uint16_t next_index = ots_db_get_next_index(gs_root_index);

            gs_objects[gs_root_index].prev_index = newest_index;
            gs_objects[gs_root_index].next_index = next_index;
            gs_objects[next_index].prev_index = gs_root_index;
            gs_root_index = next_index;
        }
    }

}

void ots_db_mark_delete_object(uint16_t index)
{
    st_ble_ots_object_t *p_object;

    p_object = ots_db_get_object(index);

    if (NULL != p_object)
    {
        p_object->valid = BLE_OTS_DB_WILL_DELETE;
    }
}

void ots_db_delete_object(st_ble_ots_object_t *p_object)
{
        p_object->valid = BLE_OTS_DB_INVALID_OBJECT;
}

#if 0 /* unused function */
void ots_db_delete_objects(uint16_t index)
{
    for (uint16_t i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        if (gs_objects[i].valid == BLE_OTS_DB_WILL_DELETE)
        {
            ots_db_delete_object(&gs_objects[i]);
            gs_num_of_objects--;
        }
    }

    if (0 == gs_num_of_objects)
    {
        gs_root_index = BLE_OTS_DB_INVALID_INDEX;
    }
}
#endif /* unused function */


void ots_object_list_filter_cb(st_ble_ots_obj_list_filter_0_t *p_app_value)
{
    uint16_t name_len;
    int16_t i = 0;

    name_len = p_app_value->string.len;

    if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_IS_EXACTLY)
    {
        /*char *name = (char*)p_app_value->string.data;*/
        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid)
            {
                if(strncmp((const char*)gs_objects[i].obj_name, (const char*)p_app_value->string.data, name_len) != 0)
                {
                    ots_db_delete_object(&gs_objects[i]);
                }
            }
            else
            {
                break;
            }
        }
    }

    else if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_STARTS_WITH)
    {
        name_len = p_app_value->string.len;
        /*uint8_t *name = p_app_value->string.data;*/
        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (gs_objects[i].valid)
            {
                if (strncmp((const char*)gs_objects[i].obj_name, (const char*)p_app_value->string.data, name_len) != 0)
                {
                    ots_db_delete_object(&gs_objects[i]);
                }
            }
        }
    }
    else if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_ENDS_WITH)
    {
        uint16_t strlen1;
        uint16_t strlen2;

        strlen1 = p_app_value->string.len;

        /*char *name = (char*)p_app_value->string.data;*/
        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid)
            {
                strlen2 = (uint16_t)strlen((const char*)gs_objects[i].obj_name);
                if (strlen1 >= strlen2)
                {
                    memcpy(gs_objects[i].obj_name, &p_app_value->string + (strlen1 - strlen2), strlen2);
                }
            }
        }
    }
    else if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NAME_CONTAINS)
    {
        char *sub_name;

        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid)
            {
                sub_name = strstr((const char*)gs_objects[i].obj_name, (const char*)p_app_value->string.data);
                if (!sub_name)
                {
                    ots_db_delete_object(&gs_objects[i]);
                }
            }
        }
    }
    else if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_OBJECT_TYPE)
    {
        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid)
            {
                if (p_app_value->uuid.uuid.uuid_16 != gs_objects[i].obj_type.uuid.uuid_16)
                {
                    ots_db_delete_object(&gs_objects[i]);
                }
            }
        }
    }
    else if (p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_CURRENT_SIZE_BETWEEN)
    {
        //if (p_app_value->size1 == 1014)
        if (p_app_value->size1 == (TEST_SIZE - 10))
        {
            pts_obj_size1.allocated_size = 1024;
            pts_obj_size1.current_size = 1024;

            pts_obj_type1.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            pts_obj_type1.uuid.uuid_16 = 0x7FB1;

            ots_db_create_object(&pts_obj_size1, &pts_obj_type1);
        }

        //if (p_app_value->size1 == 1034)
        if (p_app_value->size1 == (TEST_SIZE + 10))
        {
            pts_obj_size1.allocated_size = 1034;
            pts_obj_size1.current_size = 1034;

            pts_obj_type1.uuid_type = BLE_GATT_16_BIT_UUID_FORMAT;
            pts_obj_type1.uuid.uuid_16 = 0x7FB1;

            ots_db_create_object(&pts_obj_size1, &pts_obj_type1);
        }

        for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid)
            {
                if (!((gs_objects[i].obj_size.current_size >= p_app_value->size1) && (gs_objects[i].obj_size.current_size <= p_app_value->size2)))
                {
                    ots_db_delete_object(&gs_objects[i]);
                }
            }
        }
    }
    else if(p_app_value->filter == BLE_OTS_OBJ_LIST_FILTER_0_FILTER_NO_FILTER)
    {
    }
}

void ots_sort_object_names(void)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t  temp_name[BLE_OTS_OBJ_NAME_OBJECT_NAME_LEN];
    
    for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        for (j = (uint16_t)(i + 1); j < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; j++)
        {
            if (BLE_OTS_DB_VALID_OBJECT == gs_objects[i].valid && BLE_OTS_DB_VALID_OBJECT == gs_objects[j].valid)
            {
                if (strcmp((const char*)gs_objects[i].obj_name, (const char*)gs_objects[j].obj_name) > 0)
                {
                    strcpy((char*)temp_name, (const char*)gs_objects[i].obj_name);
                    strcpy((char*)gs_objects[i].obj_name, (const char*)gs_objects[j].obj_name);
                    strcpy((char*)gs_objects[j].obj_name, (const char*)temp_name);
                }
            }
        }
    }
}

uint16_t ots_db_num_of_objects()
{
    uint16_t i = 0;
    uint16_t num_of_objects = 0;

    for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        if (gs_objects[i].valid == BLE_OTS_DB_VALID_OBJECT)
        {
            num_of_objects++;
        }
        
    }
    return num_of_objects;
}

st_ble_ots_object_t * ots_db_get_object_id(st_ble_ots_obj_list_cp_t *p_app_value)
{
    uint16_t i = 0;
    int ret;

    st_ble_ots_object_t *p_object = NULL;

    for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        p_object = &gs_objects[i];
        if (BLE_OTS_DB_INVALID_OBJECT != p_object->valid)
        {
            ret = memcmp(&p_object->obj_id, &p_app_value->parameter, 6);
            if (ret == 0)
            {
                return p_object;
            }
        }
    }
    return NULL;
}

void ots_db_clear_marking(void)
{
    uint16_t i = 0;
    st_ble_ots_object_t *p_object;
    for (i = 0; i < BLE_OTS_DB_MAX_NUM_OF_OBJECTS; i++)
    {
        /* Unmark all objects */
        p_object = &gs_objects[i];
        if (BLE_OTS_DB_INVALID_OBJECT == p_object->valid)
        {
            continue; //not a valid record, move to next
        }
        else if (p_object->obj_properties.is_this_object_is_a_marked_object)
        {
            p_object->obj_properties.is_this_object_is_a_marked_object = false;
        }
    }
}
