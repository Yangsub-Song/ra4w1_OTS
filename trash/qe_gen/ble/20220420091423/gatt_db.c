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
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/**
 *  GATT DATABASE QUICK REFERENCE TABLE:
 *  Abbreviations used for PROPERTIES:
 *      BC = Broadcast
 *      RD = Read
 *      WW = Write Without Response
 *      WR = Write
 *      NT = Notification
 *      IN = Indication
 *      RW = Reliable Write
 * 
 *  HANDLE | ATT_TYPE          | PROPERTIES  | ATT_VALUE                        | DEFINITION
 *  ============================================================================================
 *  GAP Service
 *  ============================================================================================
 *  0x0001 | 0x28,0x00         | RD          | 0x00,0x18                        | GAP Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0002 | 0x28,0x03         | RD          | 0x0A,0x03,0x00,0x00,0x2A         | Device Name characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0003 | 0x00,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Device Name characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0004 | 0x28,0x03         | RD          | 0x02,0x05,0x00,0x01,0x2A         | Appearance characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0005 | 0x01,0x2A         | RD          | 0x00,0x00                        | Appearance characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0006 | 0x28,0x03         | RD          | 0x02,0x07,0x00,0x04,0x2A         | Peripheral Preferred Connection Parameters characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0007 | 0x04,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Peripheral Preferred Connection Parameters characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0008 | 0x28,0x03         | RD          | 0x02,0x09,0x00,0xA6,0x2A         | Central Address Resolution characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0009 | 0xA6,0x2A         | RD          | 0x00                             | Central Address Resolution characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x000A | 0x28,0x03         | RD          | 0x02,0x0B,0x00,0xC9,0x2A         | Resolvable Private Address Only characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x000B | 0xC9,0x2A         | RD          | 0x00                             | Resolvable Private Address Only characteristic value
 *  ============================================================================================
 *  GATT Service
 *  ============================================================================================
 *  0x000C | 0x28,0x00         | RD          | 0x01,0x18                        | GATT Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x000D | 0x28,0x03         | RD          | 0x20,0x0E,0x00,0x05,0x2A         | Service Changed characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x000E | 0x05,0x2A         | IN          | 0x00,0x00,0x00,0x00              | Service Changed characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x000F | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  ============================================================================================
 *  LED Switch Service
 *  ============================================================================================
 *  0x0010 | 0x28,0x00         | RD          | 0xE0,0xFC,0x8E,0x8E,0x96,0xB4... | LED Switch Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0011 | 0x28,0x03         | RD          | 0x10,0x12,0x00,0xE0,0xFC,0x8E... | Switch State characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0012 | 0xE0,0xFC,0x8E... | NT          | 0x00                             | Switch State characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0013 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0014 | 0x28,0x03         | RD          | 0x0A,0x15,0x00,0xE0,0xFC,0x8E... | LED Blink Rate characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0015 | 0xE0,0xFC,0x8E... | RD,WR       | 0x00                             | LED Blink Rate characteristic value
 *  ============================================================================================
 *  Heart Rate Service
 *  ============================================================================================
 *  0x0016 | 0x28,0x00         | RD          | 0x0D,0x18                        | Heart Rate Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0017 | 0x28,0x03         | RD          | 0x10,0x18,0x00,0x37,0x2A         | Heart Rate Measurement characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0018 | 0x37,0x2A         | NT          | 0x00,0x00,0x00,0x00,0x00,0x00... | Heart Rate Measurement characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0019 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001A | 0x28,0x03         | RD          | 0x02,0x1B,0x00,0x38,0x2A         | Body Sensor Location characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001B | 0x38,0x2A         | RD          | 0x00                             | Body Sensor Location characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001C | 0x28,0x03         | RD          | 0x08,0x1D,0x00,0x39,0x2A         | Heart Rate Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001D | 0x39,0x2A         | WR          | 0x00                             | Heart Rate Control Point characteristic value
 *  ============================================================================================
 *  Object Transfer Service
 *  ============================================================================================
 *  0x001E | 0x28,0x00         | RD          | 0x25,0x18                        | Object Transfer Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001F | 0x28,0x03         | RD          | 0x02,0x20,0x00,0xBD,0x2A         | OTS Feature characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0020 | 0xBD,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | OTS Feature characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0021 | 0x28,0x03         | RD          | 0x0A,0x22,0x00,0xBE,0x2A         | Object Name characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0022 | 0xBE,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Name characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0023 | 0x28,0x03         | RD          | 0x02,0x24,0x00,0xBF,0x2A         | Object Type characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0024 | 0xBF,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Type characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0025 | 0x28,0x03         | RD          | 0x02,0x26,0x00,0xC0,0x2A         | Object Size characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0026 | 0xC0,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Size characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0027 | 0x28,0x03         | RD          | 0x0A,0x28,0x00,0xC1,0x2A         | Object First-Created characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0028 | 0xC1,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object First-Created characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0029 | 0x28,0x03         | RD          | 0x0A,0x2A,0x00,0xC2,0x2A         | Object Last-Modified characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002A | 0xC2,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Last-Modified characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002B | 0x28,0x03         | RD          | 0x02,0x2C,0x00,0xC3,0x2A         | Object ID characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002C | 0xC3,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00    | Object ID characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002D | 0x28,0x03         | RD          | 0x0A,0x2E,0x00,0xC4,0x2A         | Object Properties characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002E | 0xC4,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00              | Object Properties characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002F | 0x28,0x03         | RD          | 0x28,0x30,0x00,0xC5,0x2A         | Object Action Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0030 | 0xC5,0x2A         | WR,IN       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Action Control Point characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0031 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0032 | 0x28,0x03         | RD          | 0x28,0x33,0x00,0xC6,0x2A         | Object List Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0033 | 0xC6,0x2A         | WR,IN       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Control Point characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0034 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0035 | 0x28,0x03         | RD          | 0x0A,0x36,0x00,0xC7,0x2A         | Object List Filter 0 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0036 | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 0 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0037 | 0x28,0x03         | RD          | 0x0A,0x38,0x00,0xC7,0x2A         | Object List Filter 1 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0038 | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 1 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0039 | 0x28,0x03         | RD          | 0x0A,0x3A,0x00,0xC7,0x2A         | Object List Filter 2 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003A | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 2 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003B | 0x28,0x03         | RD          | 0x20,0x3C,0x00,0xC8,0x2A         | Object Changed characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003C | 0xC8,0x2A         | IN          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Changed characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003D | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  ============================================================================================
 
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include "gatt_db.h"

/*****************************************************************************
* Global definition
******************************************************************************/
static const uint8_t gs_gatt_const_uuid_arr[] =
{
    /* Primary Service Declaration : 0 */
    0x00, 0x28,

    /* Secondary Service Declaration : 2 */
    0x01, 0x28,

    /* Included Service Declaration : 4 */
    0x02, 0x28,

    /* Characteristic Declaration : 6 */
    0x03, 0x28,

    /* GAP Service : 8 */
    0x00, 0x18,

    /* Device Name : 10 */
    0x00, 0x2A,

    /* Appearance : 12 */
    0x01, 0x2A,

    /* Peripheral Preferred Connection Parameters : 14 */
    0x04, 0x2A,

    /* Central Address Resolution : 16 */
    0xA6, 0x2A,

    /* Resolvable Private Address Only : 18 */
    0xC9, 0x2A,

    /* GATT Service : 20 */
    0x01, 0x18,

    /* Service Changed : 22 */
    0x05, 0x2A,

    /* Client Characteristic Configuration : 24 */
    0x02, 0x29,

    /* LED Switch Service : 26 */
    0xE0, 0xFC, 0x8E, 0x8E, 0x96, 0xB4, 0x01, 0xAB, 0x67, 0x42, 0x05, 0x5F, 0x26, 0x19, 0x83, 0x58,

    /* Switch State : 42 */
    0xE0, 0xFC, 0x8E, 0x8E, 0x96, 0xB4, 0x01, 0xAB, 0x67, 0x42, 0x05, 0x5F, 0x57, 0x7F, 0x83, 0x58,

    /* LED Blink Rate : 58 */
    0xE0, 0xFC, 0x8E, 0x8E, 0x96, 0xB4, 0x01, 0xAB, 0x67, 0x42, 0x05, 0x5F, 0x2F, 0xC3, 0x83, 0x58,

    /* Heart Rate Service : 74 */
    0x0D, 0x18,

    /* Heart Rate Measurement : 76 */
    0x37, 0x2A,

    /* Body Sensor Location : 78 */
    0x38, 0x2A,

    /* Heart Rate Control Point : 80 */
    0x39, 0x2A,

    /* Object Transfer Service : 82 */
    0x25, 0x18,

    /* OTS Feature : 84 */
    0xBD, 0x2A,

    /* Object Name : 86 */
    0xBE, 0x2A,

    /* Object Type : 88 */
    0xBF, 0x2A,

    /* Object Size : 90 */
    0xC0, 0x2A,

    /* Object First-Created : 92 */
    0xC1, 0x2A,

    /* Object Last-Modified : 94 */
    0xC2, 0x2A,

    /* Object ID : 96 */
    0xC3, 0x2A,

    /* Object Properties : 98 */
    0xC4, 0x2A,

    /* Object Action Control Point : 100 */
    0xC5, 0x2A,

    /* Object List Control Point : 102 */
    0xC6, 0x2A,

    /* Object List Filter 0 : 104 */
    0xC7, 0x2A,

    /* Object Changed : 106 */
    0xC8, 0x2A,

};

static uint8_t gs_gatt_value_arr[] =
{
    /* Device Name */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Appearance */
    0x00, 0x00,

    /* Peripheral Preferred Connection Parameters */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Central Address Resolution */
    0x00,

    /* Resolvable Private Address Only */
    0x00,

    /* LED Blink Rate */
    0x00,

    /* Body Sensor Location */
    0x00,

    /* Heart Rate Control Point */
    0x00,

    /* OTS Feature */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object Name */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object Type */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object Size */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object First-Created */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object Last-Modified */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object ID */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object Properties */
    0x00, 0x00, 0x00, 0x00,

    /* Object Action Control Point */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object List Control Point */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object List Filter 0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object List Filter 1 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Object List Filter 2 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

static const uint8_t gs_gatt_const_value_arr[] =
{
    /* Device Name */
    0x0A,       // Properties
    0x03, 0x00, // Attr Handle
    0x00, 0x2A, // UUID

    /* Appearance */
    0x02,       // Properties
    0x05, 0x00, // Attr Handle
    0x01, 0x2A, // UUID

    /* Peripheral Preferred Connection Parameters */
    0x02,       // Properties
    0x07, 0x00, // Attr Handle
    0x04, 0x2A, // UUID

    /* Central Address Resolution */
    0x02,       // Properties
    0x09, 0x00, // Attr Handle
    0xA6, 0x2A, // UUID

    /* Resolvable Private Address Only */
    0x02,       // Properties
    0x0B, 0x00, // Attr Handle
    0xC9, 0x2A, // UUID

    /* Service Changed */
    0x20,       // Properties
    0x0E, 0x00, // Attr Handle
    0x05, 0x2A, // UUID

    /* Switch State */
    0x10,       // Properties
    0x12, 0x00, // Attr Handle
    0xE0, 0xFC, 0x8E, 0x8E, 0x96, 0xB4, 0x01, 0xAB, 0x67, 0x42, 0x05, 0x5F, 0x57, 0x7F, 0x83, 0x58, // UUID

    /* LED Blink Rate */
    0x0A,       // Properties
    0x15, 0x00, // Attr Handle
    0xE0, 0xFC, 0x8E, 0x8E, 0x96, 0xB4, 0x01, 0xAB, 0x67, 0x42, 0x05, 0x5F, 0x2F, 0xC3, 0x83, 0x58, // UUID

    /* Heart Rate Measurement */
    0x10,       // Properties
    0x18, 0x00, // Attr Handle
    0x37, 0x2A, // UUID

    /* Body Sensor Location */
    0x02,       // Properties
    0x1B, 0x00, // Attr Handle
    0x38, 0x2A, // UUID

    /* Heart Rate Control Point */
    0x08,       // Properties
    0x1D, 0x00, // Attr Handle
    0x39, 0x2A, // UUID

    /* OTS Feature */
    0x02,       // Properties
    0x20, 0x00, // Attr Handle
    0xBD, 0x2A, // UUID

    /* Object Name */
    0x0A,       // Properties
    0x22, 0x00, // Attr Handle
    0xBE, 0x2A, // UUID

    /* Object Type */
    0x02,       // Properties
    0x24, 0x00, // Attr Handle
    0xBF, 0x2A, // UUID

    /* Object Size */
    0x02,       // Properties
    0x26, 0x00, // Attr Handle
    0xC0, 0x2A, // UUID

    /* Object First-Created */
    0x0A,       // Properties
    0x28, 0x00, // Attr Handle
    0xC1, 0x2A, // UUID

    /* Object Last-Modified */
    0x0A,       // Properties
    0x2A, 0x00, // Attr Handle
    0xC2, 0x2A, // UUID

    /* Object ID */
    0x02,       // Properties
    0x2C, 0x00, // Attr Handle
    0xC3, 0x2A, // UUID

    /* Object Properties */
    0x0A,       // Properties
    0x2E, 0x00, // Attr Handle
    0xC4, 0x2A, // UUID

    /* Object Action Control Point */
    0x28,       // Properties
    0x30, 0x00, // Attr Handle
    0xC5, 0x2A, // UUID

    /* Object List Control Point */
    0x28,       // Properties
    0x33, 0x00, // Attr Handle
    0xC6, 0x2A, // UUID

    /* Object List Filter 0 */
    0x0A,       // Properties
    0x36, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object List Filter 1 */
    0x0A,       // Properties
    0x38, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object List Filter 2 */
    0x0A,       // Properties
    0x3A, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object Changed */
    0x20,       // Properties
    0x3C, 0x00, // Attr Handle
    0xC8, 0x2A, // UUID

};

static const uint8_t gs_gatt_db_const_peer_specific_val_arr[] =
{
    /* Service Changed : Client Characteristic Configuration */
    0x00, 0x00,

    /* Switch State : Client Characteristic Configuration */
    0x00, 0x00,

    /* Heart Rate Measurement : Client Characteristic Configuration */
    0x00, 0x00,

    /* Object Action Control Point : Client Characteristic Configuration */
    0x00, 0x00,

    /* Object List Control Point : Client Characteristic Configuration */
    0x00, 0x00,

    /* Object Changed : Client Characteristic Configuration */
    0x00, 0x00,

};

#ifdef BSP_MCU_GROUP_RA4W1 /* RA4W1 */
static uint8_t gs_gatt_db_peer_specific_val_arr[sizeof(gs_gatt_db_const_peer_specific_val_arr)*(BLE_ABS_CFG_RF_CONNECTION_MAXIMUM+1)];
#else /* RX23W or RE01B */
static uint8_t gs_gatt_db_peer_specific_val_arr[sizeof(gs_gatt_db_const_peer_specific_val_arr)*(BLE_CFG_RF_CONN_MAX+1)];
#endif
static const st_ble_gatts_db_uuid_cfg_t gs_gatt_type_table[] =
{
    /* 0 : Primary Service Declaration */
    {
        /* UUID Offset */
        0,
        /* First Occurrence for Type */
        0x0001,
        /* Last Occurrence for Type */
        0x001E,
    },

    /* 1 : GAP Service */
    {
        /* UUID Offset */
        8,
        /* First Occurrence for Type */
        0x0001,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 2 : Characteristic Declaration */
    {
        /* UUID Offset */
        6,
        /* First Occurrence for Type */
        0x0002,
        /* Last Occurrence for Type */
        0x003B,
    },

    /* 3 : Device Name */
    {
        /* UUID Offset */
        10,
        /* First Occurrence for Type */
        0x0003,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 4 : Appearance */
    {
        /* UUID Offset */
        12,
        /* First Occurrence for Type */
        0x0005,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 5 : Peripheral Preferred Connection Parameters */
    {
        /* UUID Offset */
        14,
        /* First Occurrence for Type */
        0x0007,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 6 : Central Address Resolution */
    {
        /* UUID Offset */
        16,
        /* First Occurrence for Type */
        0x0009,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 7 : Resolvable Private Address Only */
    {
        /* UUID Offset */
        18,
        /* First Occurrence for Type */
        0x000B,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 8 : GATT Service */
    {
        /* UUID Offset */
        20,
        /* First Occurrence for Type */
        0x000C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 9 : Service Changed */
    {
        /* UUID Offset */
        22,
        /* First Occurrence for Type */
        0x000E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 10 : Client Characteristic Configuration */
    {
        /* UUID Offset */
        24,
        /* First Occurrence for Type */
        0x000F,
        /* Last Occurrence for Type */
        0x003D,
    },

    /* 11 : LED Switch Service */
    {
        /* UUID Offset */
        26,
        /* First Occurrence for Type */
        0x0010,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 12 : Switch State */
    {
        /* UUID Offset */
        42,
        /* First Occurrence for Type */
        0x0012,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 13 : LED Blink Rate */
    {
        /* UUID Offset */
        58,
        /* First Occurrence for Type */
        0x0015,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 14 : Heart Rate Service */
    {
        /* UUID Offset */
        74,
        /* First Occurrence for Type */
        0x0016,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 15 : Heart Rate Measurement */
    {
        /* UUID Offset */
        76,
        /* First Occurrence for Type */
        0x0018,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 16 : Body Sensor Location */
    {
        /* UUID Offset */
        78,
        /* First Occurrence for Type */
        0x001B,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 17 : Heart Rate Control Point */
    {
        /* UUID Offset */
        80,
        /* First Occurrence for Type */
        0x001D,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 18 : Object Transfer Service */
    {
        /* UUID Offset */
        82,
        /* First Occurrence for Type */
        0x001E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 19 : OTS Feature */
    {
        /* UUID Offset */
        84,
        /* First Occurrence for Type */
        0x0020,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 20 : Object Name */
    {
        /* UUID Offset */
        86,
        /* First Occurrence for Type */
        0x0022,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 21 : Object Type */
    {
        /* UUID Offset */
        88,
        /* First Occurrence for Type */
        0x0024,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 22 : Object Size */
    {
        /* UUID Offset */
        90,
        /* First Occurrence for Type */
        0x0026,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 23 : Object First-Created */
    {
        /* UUID Offset */
        92,
        /* First Occurrence for Type */
        0x0028,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 24 : Object Last-Modified */
    {
        /* UUID Offset */
        94,
        /* First Occurrence for Type */
        0x002A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 25 : Object ID */
    {
        /* UUID Offset */
        96,
        /* First Occurrence for Type */
        0x002C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 26 : Object Properties */
    {
        /* UUID Offset */
        98,
        /* First Occurrence for Type */
        0x002E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 27 : Object Action Control Point */
    {
        /* UUID Offset */
        100,
        /* First Occurrence for Type */
        0x0030,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 28 : Object List Control Point */
    {
        /* UUID Offset */
        102,
        /* First Occurrence for Type */
        0x0033,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 29 : Object List Filter 0 */
    {
        /* UUID Offset */
        104,
        /* First Occurrence for Type */
        0x0036,
        /* Last Occurrence for Type */
        0x003A,
    },

    /* 30 : Object Changed */
    {
        /* UUID Offset */
        106,
        /* First Occurrence for Type */
        0x003C,
        /* Last Occurrence for Type */
        0x0000,
    },

};

static const st_ble_gatts_db_attr_cfg_t gs_gatt_db_attr_table[] =
{
    /* Handle : 0x0000 */
    /* Blank */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0001,
        /* UUID Offset */
        0,
        /* Value */
        NULL,
    },

    /* Handle : 0x0001 */
    /* GAP Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x000C,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 8),
    },

    /* Handle : 0x0002 */
    /* Device Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0004,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 0),
    },

    /* Handle : 0x0003 */
    /* Device Name */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        128,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        10,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 0),
    },

    /* Handle : 0x0004 */
    /* Appearance : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0006,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 5),
    },

    /* Handle : 0x0005 */
    /* Appearance */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        12,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 128),
    },

    /* Handle : 0x0006 */
    /* Peripheral Preferred Connection Parameters : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0008,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 10),
    },

    /* Handle : 0x0007 */
    /* Peripheral Preferred Connection Parameters */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        8,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        14,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 130),
    },

    /* Handle : 0x0008 */
    /* Central Address Resolution : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x000A,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 15),
    },

    /* Handle : 0x0009 */
    /* Central Address Resolution */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        16,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 138),
    },

    /* Handle : 0x000A */
    /* Resolvable Private Address Only : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x000D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 20),
    },

    /* Handle : 0x000B */
    /* Resolvable Private Address Only */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        18,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 139),
    },

    /* Handle : 0x000C */
    /* GATT Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0010,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 20),
    },

    /* Handle : 0x000D */
    /* Service Changed : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0011,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 25),
    },

    /* Handle : 0x000E */
    /* Service Changed */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        4,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        22,
        /* Value */
        (NULL),
    },

    /* Handle : 0x000F */
    /* Service Changed : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0013,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 0),
    },

    /* Handle : 0x0010 */
    /* LED Switch Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_128_BIT_UUID_FORMAT,
        /* Value Size */
        16,
        /* Next Attribute Type Index */
        0x0016,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 26),
    },

    /* Handle : 0x0011 */
    /* Switch State : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        19,
        /* Next Attribute Type Index */
        0x0014,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 30),
    },

    /* Handle : 0x0012 */
    /* Switch State */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_128_BIT_UUID_FORMAT,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        42,
        /* Value */
        (NULL),
    },

    /* Handle : 0x0013 */
    /* Switch State : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0019,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 2),
    },

    /* Handle : 0x0014 */
    /* LED Blink Rate : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        19,
        /* Next Attribute Type Index */
        0x0017,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 49),
    },

    /* Handle : 0x0015 */
    /* LED Blink Rate */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_128_BIT_UUID_FORMAT,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        58,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 140),
    },

    /* Handle : 0x0016 */
    /* Heart Rate Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x001E,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 74),
    },

    /* Handle : 0x0017 */
    /* Heart Rate Measurement : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x001A,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 68),
    },

    /* Handle : 0x0018 */
    /* Heart Rate Measurement */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        20,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        76,
        /* Value */
        (NULL),
    },

    /* Handle : 0x0019 */
    /* Heart Rate Measurement : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0031,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 4),
    },

    /* Handle : 0x001A */
    /* Body Sensor Location : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x001C,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 73),
    },

    /* Handle : 0x001B */
    /* Body Sensor Location */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        78,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 141),
    },

    /* Handle : 0x001C */
    /* Heart Rate Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x001F,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 78),
    },

    /* Handle : 0x001D */
    /* Heart Rate Control Point */
    {
        /* Properties */
        BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        80,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 142),
    },

    /* Handle : 0x001E */
    /* Object Transfer Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 82),
    },

    /* Handle : 0x001F */
    /* OTS Feature : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0021,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 83),
    },

    /* Handle : 0x0020 */
    /* OTS Feature */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        8,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        84,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 143),
    },

    /* Handle : 0x0021 */
    /* Object Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0023,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 88),
    },

    /* Handle : 0x0022 */
    /* Object Name */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        120,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        86,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 151),
    },

    /* Handle : 0x0023 */
    /* Object Type : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0025,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 93),
    },

    /* Handle : 0x0024 */
    /* Object Type */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        16,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        88,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 271),
    },

    /* Handle : 0x0025 */
    /* Object Size : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0027,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 98),
    },

    /* Handle : 0x0026 */
    /* Object Size */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        8,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        90,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 287),
    },

    /* Handle : 0x0027 */
    /* Object First-Created : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0029,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 103),
    },

    /* Handle : 0x0028 */
    /* Object First-Created */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        7,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        92,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 295),
    },

    /* Handle : 0x0029 */
    /* Object Last-Modified : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x002B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 108),
    },

    /* Handle : 0x002A */
    /* Object Last-Modified */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        7,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        94,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 302),
    },

    /* Handle : 0x002B */
    /* Object ID : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x002D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 113),
    },

    /* Handle : 0x002C */
    /* Object ID */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        6,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        96,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 309),
    },

    /* Handle : 0x002D */
    /* Object Properties : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x002F,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 118),
    },

    /* Handle : 0x002E */
    /* Object Properties */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        4,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        98,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 315),
    },

    /* Handle : 0x002F */
    /* Object Action Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0032,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 123),
    },

    /* Handle : 0x0030 */
    /* Object Action Control Point */
    {
        /* Properties */
        BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        21,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        100,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 319),
    },

    /* Handle : 0x0031 */
    /* Object Action Control Point : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0034,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 6),
    },

    /* Handle : 0x0032 */
    /* Object List Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0035,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 128),
    },

    /* Handle : 0x0033 */
    /* Object List Control Point */
    {
        /* Properties */
        BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        7,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        102,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 340),
    },

    /* Handle : 0x0034 */
    /* Object List Control Point : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x003D,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 8),
    },

    /* Handle : 0x0035 */
    /* Object List Filter 0 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0037,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 133),
    },

    /* Handle : 0x0036 */
    /* Object List Filter 0 */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0038,
        /* UUID Offset */
        104,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 347),
    },

    /* Handle : 0x0037 */
    /* Object List Filter 1 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0039,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 138),
    },

    /* Handle : 0x0038 */
    /* Object List Filter 1 */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x003A,
        /* UUID Offset */
        104,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 447),
    },

    /* Handle : 0x0039 */
    /* Object List Filter 2 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x003B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 143),
    },

    /* Handle : 0x003A */
    /* Object List Filter 2 */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        104,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 547),
    },

    /* Handle : 0x003B */
    /* Object Changed : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 148),
    },

    /* Handle : 0x003C */
    /* Object Changed */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        7,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        106,
        /* Value */
        (NULL),
    },

    /* Handle : 0x003D */
    /* Object Changed : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 10),
    },

};

static const st_ble_gatts_db_char_cfg_t gs_gatt_characteristic[] =
{
    /* 0 : Device Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0002,
        /* Service Index */
        0,
    },

    /* 1 : Appearance */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0004,
        /* Service Index */
        0,
    },

    /* 2 : Peripheral Preferred Connection Parameters */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0006,
        /* Service Index */
        0,
    },

    /* 3 : Central Address Resolution */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0008,
        /* Service Index */
        0,
    },

    /* 4 : Resolvable Private Address Only */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x000A,
        /* Service Index */
        0,
    },

    /* 5 : Service Changed */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x000D,
        /* Service Index */
        1,
    },

    /* 6 : Switch State */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0011,
        /* Service Index */
        2,
    },

    /* 7 : LED Blink Rate */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0014,
        /* Service Index */
        2,
    },

    /* 8 : Heart Rate Measurement */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0017,
        /* Service Index */
        3,
    },

    /* 9 : Body Sensor Location */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001A,
        /* Service Index */
        3,
    },

    /* 10 : Heart Rate Control Point */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001C,
        /* Service Index */
        3,
    },

    /* 11 : OTS Feature */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001F,
        /* Service Index */
        4,
    },

    /* 12 : Object Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0021,
        /* Service Index */
        4,
    },

    /* 13 : Object Type */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0023,
        /* Service Index */
        4,
    },

    /* 14 : Object Size */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0025,
        /* Service Index */
        4,
    },

    /* 15 : Object First-Created */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0027,
        /* Service Index */
        4,
    },

    /* 16 : Object Last-Modified */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0029,
        /* Service Index */
        4,
    },

    /* 17 : Object ID */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x002B,
        /* Service Index */
        4,
    },

    /* 18 : Object Properties */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x002D,
        /* Service Index */
        4,
    },

    /* 19 : Object Action Control Point */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x002F,
        /* Service Index */
        4,
    },

    /* 20 : Object List Control Point */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0032,
        /* Service Index */
        4,
    },

    /* 21 : Object List Filter 0 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0035,
        /* Service Index */
        4,
    },

    /* 22 : Object List Filter 1 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0037,
        /* Service Index */
        4,
    },

    /* 23 : Object List Filter 2 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0039,
        /* Service Index */
        4,
    },

    /* 24 : Object Changed */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x003B,
        /* Service Index */
        4,
    },

};

static const st_ble_gatts_db_serv_cfg_t gs_gatt_service[] =
{
    /* GAP Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x0001,
        /* Service End Handle */
        0x000B,
        /* Characteristic Start Index */
        0,
        /* Characteristic End Index */
        4,
    },

    /* GATT Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x000C,
        /* Service End Handle */
        0x000F,
        /* Characteristic Start Index */
        5,
        /* Characteristic End Index */
        5,
    },

    /* LED Switch Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x0010,
        /* Service End Handle */
        0x0015,
        /* Characteristic Start Index */
        6,
        /* Characteristic End Index */
        7,
    },

    /* Heart Rate Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x0016,
        /* Service End Handle */
        0x001D,
        /* Characteristic Start Index */
        8,
        /* Characteristic End Index */
        10,
    },

    /* Object Transfer Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x001E,
        /* Service End Handle */
        0x003D,
        /* Characteristic Start Index */
        11,
        /* Characteristic End Index */
        24,
    },

};

st_ble_gatts_db_cfg_t g_gatt_db_table =
{
    gs_gatt_const_uuid_arr,
    gs_gatt_value_arr,
    gs_gatt_const_value_arr,
    gs_gatt_db_peer_specific_val_arr,
    gs_gatt_db_const_peer_specific_val_arr,
    gs_gatt_type_table,
    gs_gatt_db_attr_table,
    gs_gatt_characteristic,
    gs_gatt_service,
    ARRAY_SIZE(gs_gatt_service),
    ARRAY_SIZE(gs_gatt_characteristic),
    ARRAY_SIZE(gs_gatt_type_table),
    ARRAY_SIZE(gs_gatt_db_const_peer_specific_val_arr),
};