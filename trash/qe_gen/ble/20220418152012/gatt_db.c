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
 *  User Data Service
 *  ============================================================================================
 *  0x0016 | 0x28,0x00         | RD          | 0x1C,0x18                        | User Data Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0017 | 0x28,0x03         | RD          | 0x0A,0x18,0x00,0x8A,0x2A         | First Name characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0018 | 0x8A,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | First Name characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0019 | 0x28,0x03         | RD          | 0x0A,0x1A,0x00,0x90,0x2A         | Last Name characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001A | 0x90,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Last Name characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001B | 0x28,0x03         | RD          | 0x0A,0x1C,0x00,0x87,0x2A         | Email Address characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001C | 0x87,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Email Address characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001D | 0x28,0x03         | RD          | 0x0A,0x1E,0x00,0x80,0x2A         | Age characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001E | 0x80,0x2A         | RD,WR       | 0x00                             | Age characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x001F | 0x28,0x03         | RD          | 0x0A,0x20,0x00,0x85,0x2A         | Date of Birth characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0020 | 0x85,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00              | Date of Birth characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0021 | 0x28,0x03         | RD          | 0x0A,0x22,0x00,0x8C,0x2A         | Gender characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0022 | 0x8C,0x2A         | RD,WR       | 0x00                             | Gender characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0023 | 0x28,0x03         | RD          | 0x0A,0x24,0x00,0x98,0x2A         | Weight characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0024 | 0x98,0x2A         | RD,WR       | 0x00,0x00                        | Weight characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0025 | 0x28,0x03         | RD          | 0x0A,0x26,0x00,0x8E,0x2A         | Height characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0026 | 0x8E,0x2A         | RD,WR       | 0x00,0x00                        | Height characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0027 | 0x28,0x03         | RD          | 0x0A,0x28,0x00,0x96,0x2A         | VO2 Max characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0028 | 0x96,0x2A         | RD,WR       | 0x00                             | VO2 Max characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0029 | 0x28,0x03         | RD          | 0x0A,0x2A,0x00,0x8D,0x2A         | Heart Rate Max characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002A | 0x8D,0x2A         | RD,WR       | 0x00                             | Heart Rate Max characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002B | 0x28,0x03         | RD          | 0x0A,0x2C,0x00,0x92,0x2A         | Resting Heart Rate characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002C | 0x92,0x2A         | RD,WR       | 0x00                             | Resting Heart Rate characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002D | 0x28,0x03         | RD          | 0x0A,0x2E,0x00,0x91,0x2A         | Maximum Recommended Heart Rate characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002E | 0x91,0x2A         | RD,WR       | 0x00                             | Maximum Recommended Heart Rate characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x002F | 0x28,0x03         | RD          | 0x0A,0x30,0x00,0x7F,0x2A         | Aerobic Threshold characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0030 | 0x7F,0x2A         | RD,WR       | 0x00                             | Aerobic Threshold characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0031 | 0x28,0x03         | RD          | 0x0A,0x32,0x00,0x83,0x2A         | Anaerobic Threshold characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0032 | 0x83,0x2A         | RD,WR       | 0x00                             | Anaerobic Threshold characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0033 | 0x28,0x03         | RD          | 0x0A,0x34,0x00,0x93,0x2A         | Sport Type for Aerobic and Anaerobic Thresholds characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0034 | 0x93,0x2A         | RD,WR       | 0x00                             | Sport Type for Aerobic and Anaerobic Thresholds characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0035 | 0x28,0x03         | RD          | 0x0A,0x36,0x00,0x86,0x2A         | Date of Threshold Assessment characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0036 | 0x86,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00              | Date of Threshold Assessment characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0037 | 0x28,0x03         | RD          | 0x0A,0x38,0x00,0x97,0x2A         | Waist Circumference characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0038 | 0x97,0x2A         | RD,WR       | 0x00,0x00                        | Waist Circumference characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0039 | 0x28,0x03         | RD          | 0x0A,0x3A,0x00,0x8F,0x2A         | Hip Circumference characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003A | 0x8F,0x2A         | RD,WR       | 0x00,0x00                        | Hip Circumference characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003B | 0x28,0x03         | RD          | 0x0A,0x3C,0x00,0x88,0x2A         | Fat Burn Heart Rate Lower Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003C | 0x88,0x2A         | RD,WR       | 0x00                             | Fat Burn Heart Rate Lower Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003D | 0x28,0x03         | RD          | 0x0A,0x3E,0x00,0x89,0x2A         | Fat Burn Heart Rate Upper Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003E | 0x89,0x2A         | RD,WR       | 0x00                             | Fat Burn Heart Rate Upper Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x003F | 0x28,0x03         | RD          | 0x0A,0x40,0x00,0x7E,0x2A         | Aerobic Heart Rate Lower Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0040 | 0x7E,0x2A         | RD,WR       | 0x00                             | Aerobic Heart Rate Lower Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0041 | 0x28,0x03         | RD          | 0x0A,0x42,0x00,0x84,0x2A         | Aerobic Heart Rate Upper Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0042 | 0x84,0x2A         | RD,WR       | 0x00                             | Aerobic Heart Rate Upper Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0043 | 0x28,0x03         | RD          | 0x0A,0x44,0x00,0x81,0x2A         | Anaerobic Heart Rate Lower Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0044 | 0x81,0x2A         | RD,WR       | 0x00                             | Anaerobic Heart Rate Lower Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0045 | 0x28,0x03         | RD          | 0x0A,0x46,0x00,0x82,0x2A         | Anaerobic Heart Rate Upper Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0046 | 0x82,0x2A         | RD,WR       | 0x00                             | Anaerobic Heart Rate Upper Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0047 | 0x28,0x03         | RD          | 0x0A,0x48,0x00,0x8B,0x2A         | Five Zone Heart Rate Limits characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0048 | 0x8B,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00              | Five Zone Heart Rate Limits characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0049 | 0x28,0x03         | RD          | 0x0A,0x4A,0x00,0x94,0x2A         | Three Zone Heart Rate Limits characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004A | 0x94,0x2A         | RD,WR       | 0x00,0x00                        | Three Zone Heart Rate Limits characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004B | 0x28,0x03         | RD          | 0x0A,0x4C,0x00,0x95,0x2A         | Two Zone Heart Rate Limit characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004C | 0x95,0x2A         | RD,WR       | 0x00                             | Two Zone Heart Rate Limit characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004D | 0x28,0x03         | RD          | 0x1A,0x4E,0x00,0x99,0x2A         | Database Change Increment characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004E | 0x99,0x2A         | RD,WR,NT    | 0x00,0x00,0x00,0x00              | Database Change Increment characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x004F | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0050 | 0x28,0x03         | RD          | 0x02,0x51,0x00,0x9A,0x2A         | User Index characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0051 | 0x9A,0x2A         | RD          | 0x00                             | User Index characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0052 | 0x28,0x03         | RD          | 0x28,0x53,0x00,0x9F,0x2A         | User Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0053 | 0x9F,0x2A         | WR,IN       | 0x00,0x00,0x00,0x00,0x00,0x00... | User Control Point characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0054 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0055 | 0x28,0x03         | RD          | 0x0A,0x56,0x00,0xA2,0x2A         | Language characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0056 | 0xA2,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Language characteristic value
 *  ============================================================================================
 *  Heart Rate Service
 *  ============================================================================================
 *  0x0057 | 0x28,0x00         | RD          | 0x0D,0x18                        | Heart Rate Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0058 | 0x28,0x03         | RD          | 0x10,0x59,0x00,0x37,0x2A         | Heart Rate Measurement characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0059 | 0x37,0x2A         | NT          | 0x00,0x00,0x00,0x00,0x00,0x00... | Heart Rate Measurement characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x005A | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x005B | 0x28,0x03         | RD          | 0x02,0x5C,0x00,0x38,0x2A         | Body Sensor Location characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x005C | 0x38,0x2A         | RD          | 0x00                             | Body Sensor Location characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x005D | 0x28,0x03         | RD          | 0x08,0x5E,0x00,0x39,0x2A         | Heart Rate Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x005E | 0x39,0x2A         | WR          | 0x00                             | Heart Rate Control Point characteristic value
 *  ============================================================================================
 *  Device Information Service
 *  ============================================================================================
 *  0x005F | 0x28,0x00         | RD          | 0x0A,0x18                        | Device Information Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0060 | 0x28,0x03         | RD          | 0x02,0x61,0x00,0x29,0x2A         | Manufacturer Name String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0061 | 0x29,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Manufacturer Name String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0062 | 0x28,0x03         | RD          | 0x02,0x63,0x00,0x24,0x2A         | Model Number String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0063 | 0x24,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Model Number String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0064 | 0x28,0x03         | RD          | 0x02,0x65,0x00,0x25,0x2A         | Serial Number String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0065 | 0x25,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Serial Number String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0066 | 0x28,0x03         | RD          | 0x02,0x67,0x00,0x27,0x2A         | Hardware Revision String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0067 | 0x27,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Hardware Revision String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0068 | 0x28,0x03         | RD          | 0x02,0x69,0x00,0x26,0x2A         | Firmware Revision String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0069 | 0x26,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Firmware Revision String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006A | 0x28,0x03         | RD          | 0x02,0x6B,0x00,0x28,0x2A         | Software Revision String characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006B | 0x28,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Software Revision String characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006C | 0x28,0x03         | RD          | 0x02,0x6D,0x00,0x23,0x2A         | System ID characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006D | 0x23,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | System ID characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006E | 0x28,0x03         | RD          | 0x02,0x6F,0x00,0x2A,0x2A         | IEEE 11073-20601 Regulatory Certification Data List characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x006F | 0x2A,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | IEEE 11073-20601 Regulatory Certification Data List characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0070 | 0x28,0x03         | RD          | 0x02,0x71,0x00,0x50,0x2A         | PnP ID characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0071 | 0x50,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | PnP ID characteristic value
 *  ============================================================================================
 *  Object Transfer Service
 *  ============================================================================================
 *  0x0072 | 0x28,0x00         | RD          | 0x25,0x18                        | Object Transfer Service Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0073 | 0x28,0x03         | RD          | 0x02,0x74,0x00,0xBD,0x2A         | OTS Feature characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0074 | 0xBD,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | OTS Feature characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0075 | 0x28,0x03         | RD          | 0x0A,0x76,0x00,0xBE,0x2A         | Object Name characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0076 | 0xBE,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Name characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0077 | 0x28,0x03         | RD          | 0x02,0x78,0x00,0xBF,0x2A         | Object Type characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0078 | 0xBF,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Type characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0079 | 0x28,0x03         | RD          | 0x02,0x7A,0x00,0xC0,0x2A         | Object Size characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007A | 0xC0,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Size characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007B | 0x28,0x03         | RD          | 0x0A,0x7C,0x00,0xC1,0x2A         | Object First-Created characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007C | 0xC1,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object First-Created characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007D | 0x28,0x03         | RD          | 0x0A,0x7E,0x00,0xC2,0x2A         | Object Last-Modified characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007E | 0xC2,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Last-Modified characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x007F | 0x28,0x03         | RD          | 0x02,0x80,0x00,0xC3,0x2A         | Object ID characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0080 | 0xC3,0x2A         | RD          | 0x00,0x00,0x00,0x00,0x00,0x00    | Object ID characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0081 | 0x28,0x03         | RD          | 0x0A,0x82,0x00,0xC4,0x2A         | Object Properties characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0082 | 0xC4,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00              | Object Properties characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0083 | 0x28,0x03         | RD          | 0x28,0x84,0x00,0xC5,0x2A         | Object Action Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0084 | 0xC5,0x2A         | WR,IN       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Action Control Point characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0085 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0086 | 0x28,0x03         | RD          | 0x28,0x87,0x00,0xC6,0x2A         | Object List Control Point characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0087 | 0xC6,0x2A         | WR,IN       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Control Point characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0088 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0089 | 0x28,0x03         | RD          | 0x0A,0x8A,0x00,0xC7,0x2A         | Object List Filter 0 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008A | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 0 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008B | 0x28,0x03         | RD          | 0x0A,0x8C,0x00,0xC7,0x2A         | Object List Filter 1 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008C | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 1 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008D | 0x28,0x03         | RD          | 0x0A,0x8E,0x00,0xC7,0x2A         | Object List Filter 2 characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008E | 0xC7,0x2A         | RD,WR       | 0x00,0x00,0x00,0x00,0x00,0x00... | Object List Filter 2 characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x008F | 0x28,0x03         | RD          | 0x20,0x90,0x00,0xC8,0x2A         | Object Changed characteristic Declaration
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0090 | 0xC8,0x2A         | IN          | 0x00,0x00,0x00,0x00,0x00,0x00... | Object Changed characteristic value
 *  -------+-------------------+-------------+----------------------------------+---------------
 *  0x0091 | 0x02,0x29         | RD,WR       | 0x00,0x00                        | Client Characteristic Configuration descriptor
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

    /* User Data Service : 74 */
    0x1C, 0x18,

    /* First Name : 76 */
    0x8A, 0x2A,

    /* Last Name : 78 */
    0x90, 0x2A,

    /* Email Address : 80 */
    0x87, 0x2A,

    /* Age : 82 */
    0x80, 0x2A,

    /* Date of Birth : 84 */
    0x85, 0x2A,

    /* Gender : 86 */
    0x8C, 0x2A,

    /* Weight : 88 */
    0x98, 0x2A,

    /* Height : 90 */
    0x8E, 0x2A,

    /* VO2 Max : 92 */
    0x96, 0x2A,

    /* Heart Rate Max : 94 */
    0x8D, 0x2A,

    /* Resting Heart Rate : 96 */
    0x92, 0x2A,

    /* Maximum Recommended Heart Rate : 98 */
    0x91, 0x2A,

    /* Aerobic Threshold : 100 */
    0x7F, 0x2A,

    /* Anaerobic Threshold : 102 */
    0x83, 0x2A,

    /* Sport Type for Aerobic and Anaerobic Thresholds : 104 */
    0x93, 0x2A,

    /* Date of Threshold Assessment : 106 */
    0x86, 0x2A,

    /* Waist Circumference : 108 */
    0x97, 0x2A,

    /* Hip Circumference : 110 */
    0x8F, 0x2A,

    /* Fat Burn Heart Rate Lower Limit : 112 */
    0x88, 0x2A,

    /* Fat Burn Heart Rate Upper Limit : 114 */
    0x89, 0x2A,

    /* Aerobic Heart Rate Lower Limit : 116 */
    0x7E, 0x2A,

    /* Aerobic Heart Rate Upper Limit : 118 */
    0x84, 0x2A,

    /* Anaerobic Heart Rate Lower Limit : 120 */
    0x81, 0x2A,

    /* Anaerobic Heart Rate Upper Limit : 122 */
    0x82, 0x2A,

    /* Five Zone Heart Rate Limits : 124 */
    0x8B, 0x2A,

    /* Three Zone Heart Rate Limits : 126 */
    0x94, 0x2A,

    /* Two Zone Heart Rate Limit : 128 */
    0x95, 0x2A,

    /* Database Change Increment : 130 */
    0x99, 0x2A,

    /* User Index : 132 */
    0x9A, 0x2A,

    /* User Control Point : 134 */
    0x9F, 0x2A,

    /* Language : 136 */
    0xA2, 0x2A,

    /* Heart Rate Service : 138 */
    0x0D, 0x18,

    /* Heart Rate Measurement : 140 */
    0x37, 0x2A,

    /* Body Sensor Location : 142 */
    0x38, 0x2A,

    /* Heart Rate Control Point : 144 */
    0x39, 0x2A,

    /* Device Information Service : 146 */
    0x0A, 0x18,

    /* Manufacturer Name String : 148 */
    0x29, 0x2A,

    /* Model Number String : 150 */
    0x24, 0x2A,

    /* Serial Number String : 152 */
    0x25, 0x2A,

    /* Hardware Revision String : 154 */
    0x27, 0x2A,

    /* Firmware Revision String : 156 */
    0x26, 0x2A,

    /* Software Revision String : 158 */
    0x28, 0x2A,

    /* System ID : 160 */
    0x23, 0x2A,

    /* IEEE 11073-20601 Regulatory Certification Data List : 162 */
    0x2A, 0x2A,

    /* PnP ID : 164 */
    0x50, 0x2A,

    /* Object Transfer Service : 166 */
    0x25, 0x18,

    /* OTS Feature : 168 */
    0xBD, 0x2A,

    /* Object Name : 170 */
    0xBE, 0x2A,

    /* Object Type : 172 */
    0xBF, 0x2A,

    /* Object Size : 174 */
    0xC0, 0x2A,

    /* Object First-Created : 176 */
    0xC1, 0x2A,

    /* Object Last-Modified : 178 */
    0xC2, 0x2A,

    /* Object ID : 180 */
    0xC3, 0x2A,

    /* Object Properties : 182 */
    0xC4, 0x2A,

    /* Object Action Control Point : 184 */
    0xC5, 0x2A,

    /* Object List Control Point : 186 */
    0xC6, 0x2A,

    /* Object List Filter 0 : 188 */
    0xC7, 0x2A,

    /* Object Changed : 190 */
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

    /* First Name */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Last Name */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Email Address */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Age */
    0x00,

    /* Date of Birth */
    0x00, 0x00, 0x00, 0x00,

    /* Gender */
    0x00,

    /* Weight */
    0x00, 0x00,

    /* Height */
    0x00, 0x00,

    /* VO2 Max */
    0x00,

    /* Heart Rate Max */
    0x00,

    /* Resting Heart Rate */
    0x00,

    /* Maximum Recommended Heart Rate */
    0x00,

    /* Aerobic Threshold */
    0x00,

    /* Anaerobic Threshold */
    0x00,

    /* Sport Type for Aerobic and Anaerobic Thresholds */
    0x00,

    /* Date of Threshold Assessment */
    0x00, 0x00, 0x00, 0x00,

    /* Waist Circumference */
    0x00, 0x00,

    /* Hip Circumference */
    0x00, 0x00,

    /* Fat Burn Heart Rate Lower Limit */
    0x00,

    /* Fat Burn Heart Rate Upper Limit */
    0x00,

    /* Aerobic Heart Rate Lower Limit */
    0x00,

    /* Aerobic Heart Rate Upper Limit */
    0x00,

    /* Anaerobic Heart Rate Lower Limit */
    0x00,

    /* Anaerobic Heart Rate Upper Limit */
    0x00,

    /* Five Zone Heart Rate Limits */
    0x00, 0x00, 0x00, 0x00,

    /* Three Zone Heart Rate Limits */
    0x00, 0x00,

    /* Two Zone Heart Rate Limit */
    0x00,

    /* Database Change Increment */
    0x00, 0x00, 0x00, 0x00,

    /* User Index */
    0x00,

    /* User Control Point */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Language */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Body Sensor Location */
    0x00,

    /* Heart Rate Control Point */
    0x00,

    /* Manufacturer Name String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Model Number String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Serial Number String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Hardware Revision String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Firmware Revision String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* Software Revision String */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* System ID */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* IEEE 11073-20601 Regulatory Certification Data List */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* PnP ID */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

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

    /* First Name */
    0x0A,       // Properties
    0x18, 0x00, // Attr Handle
    0x8A, 0x2A, // UUID

    /* Last Name */
    0x0A,       // Properties
    0x1A, 0x00, // Attr Handle
    0x90, 0x2A, // UUID

    /* Email Address */
    0x0A,       // Properties
    0x1C, 0x00, // Attr Handle
    0x87, 0x2A, // UUID

    /* Age */
    0x0A,       // Properties
    0x1E, 0x00, // Attr Handle
    0x80, 0x2A, // UUID

    /* Date of Birth */
    0x0A,       // Properties
    0x20, 0x00, // Attr Handle
    0x85, 0x2A, // UUID

    /* Gender */
    0x0A,       // Properties
    0x22, 0x00, // Attr Handle
    0x8C, 0x2A, // UUID

    /* Weight */
    0x0A,       // Properties
    0x24, 0x00, // Attr Handle
    0x98, 0x2A, // UUID

    /* Height */
    0x0A,       // Properties
    0x26, 0x00, // Attr Handle
    0x8E, 0x2A, // UUID

    /* VO2 Max */
    0x0A,       // Properties
    0x28, 0x00, // Attr Handle
    0x96, 0x2A, // UUID

    /* Heart Rate Max */
    0x0A,       // Properties
    0x2A, 0x00, // Attr Handle
    0x8D, 0x2A, // UUID

    /* Resting Heart Rate */
    0x0A,       // Properties
    0x2C, 0x00, // Attr Handle
    0x92, 0x2A, // UUID

    /* Maximum Recommended Heart Rate */
    0x0A,       // Properties
    0x2E, 0x00, // Attr Handle
    0x91, 0x2A, // UUID

    /* Aerobic Threshold */
    0x0A,       // Properties
    0x30, 0x00, // Attr Handle
    0x7F, 0x2A, // UUID

    /* Anaerobic Threshold */
    0x0A,       // Properties
    0x32, 0x00, // Attr Handle
    0x83, 0x2A, // UUID

    /* Sport Type for Aerobic and Anaerobic Thresholds */
    0x0A,       // Properties
    0x34, 0x00, // Attr Handle
    0x93, 0x2A, // UUID

    /* Date of Threshold Assessment */
    0x0A,       // Properties
    0x36, 0x00, // Attr Handle
    0x86, 0x2A, // UUID

    /* Waist Circumference */
    0x0A,       // Properties
    0x38, 0x00, // Attr Handle
    0x97, 0x2A, // UUID

    /* Hip Circumference */
    0x0A,       // Properties
    0x3A, 0x00, // Attr Handle
    0x8F, 0x2A, // UUID

    /* Fat Burn Heart Rate Lower Limit */
    0x0A,       // Properties
    0x3C, 0x00, // Attr Handle
    0x88, 0x2A, // UUID

    /* Fat Burn Heart Rate Upper Limit */
    0x0A,       // Properties
    0x3E, 0x00, // Attr Handle
    0x89, 0x2A, // UUID

    /* Aerobic Heart Rate Lower Limit */
    0x0A,       // Properties
    0x40, 0x00, // Attr Handle
    0x7E, 0x2A, // UUID

    /* Aerobic Heart Rate Upper Limit */
    0x0A,       // Properties
    0x42, 0x00, // Attr Handle
    0x84, 0x2A, // UUID

    /* Anaerobic Heart Rate Lower Limit */
    0x0A,       // Properties
    0x44, 0x00, // Attr Handle
    0x81, 0x2A, // UUID

    /* Anaerobic Heart Rate Upper Limit */
    0x0A,       // Properties
    0x46, 0x00, // Attr Handle
    0x82, 0x2A, // UUID

    /* Five Zone Heart Rate Limits */
    0x0A,       // Properties
    0x48, 0x00, // Attr Handle
    0x8B, 0x2A, // UUID

    /* Three Zone Heart Rate Limits */
    0x0A,       // Properties
    0x4A, 0x00, // Attr Handle
    0x94, 0x2A, // UUID

    /* Two Zone Heart Rate Limit */
    0x0A,       // Properties
    0x4C, 0x00, // Attr Handle
    0x95, 0x2A, // UUID

    /* Database Change Increment */
    0x1A,       // Properties
    0x4E, 0x00, // Attr Handle
    0x99, 0x2A, // UUID

    /* User Index */
    0x02,       // Properties
    0x51, 0x00, // Attr Handle
    0x9A, 0x2A, // UUID

    /* User Control Point */
    0x28,       // Properties
    0x53, 0x00, // Attr Handle
    0x9F, 0x2A, // UUID

    /* Language */
    0x0A,       // Properties
    0x56, 0x00, // Attr Handle
    0xA2, 0x2A, // UUID

    /* Heart Rate Measurement */
    0x10,       // Properties
    0x59, 0x00, // Attr Handle
    0x37, 0x2A, // UUID

    /* Body Sensor Location */
    0x02,       // Properties
    0x5C, 0x00, // Attr Handle
    0x38, 0x2A, // UUID

    /* Heart Rate Control Point */
    0x08,       // Properties
    0x5E, 0x00, // Attr Handle
    0x39, 0x2A, // UUID

    /* Manufacturer Name String */
    0x02,       // Properties
    0x61, 0x00, // Attr Handle
    0x29, 0x2A, // UUID

    /* Model Number String */
    0x02,       // Properties
    0x63, 0x00, // Attr Handle
    0x24, 0x2A, // UUID

    /* Serial Number String */
    0x02,       // Properties
    0x65, 0x00, // Attr Handle
    0x25, 0x2A, // UUID

    /* Hardware Revision String */
    0x02,       // Properties
    0x67, 0x00, // Attr Handle
    0x27, 0x2A, // UUID

    /* Firmware Revision String */
    0x02,       // Properties
    0x69, 0x00, // Attr Handle
    0x26, 0x2A, // UUID

    /* Software Revision String */
    0x02,       // Properties
    0x6B, 0x00, // Attr Handle
    0x28, 0x2A, // UUID

    /* System ID */
    0x02,       // Properties
    0x6D, 0x00, // Attr Handle
    0x23, 0x2A, // UUID

    /* IEEE 11073-20601 Regulatory Certification Data List */
    0x02,       // Properties
    0x6F, 0x00, // Attr Handle
    0x2A, 0x2A, // UUID

    /* PnP ID */
    0x02,       // Properties
    0x71, 0x00, // Attr Handle
    0x50, 0x2A, // UUID

    /* OTS Feature */
    0x02,       // Properties
    0x74, 0x00, // Attr Handle
    0xBD, 0x2A, // UUID

    /* Object Name */
    0x0A,       // Properties
    0x76, 0x00, // Attr Handle
    0xBE, 0x2A, // UUID

    /* Object Type */
    0x02,       // Properties
    0x78, 0x00, // Attr Handle
    0xBF, 0x2A, // UUID

    /* Object Size */
    0x02,       // Properties
    0x7A, 0x00, // Attr Handle
    0xC0, 0x2A, // UUID

    /* Object First-Created */
    0x0A,       // Properties
    0x7C, 0x00, // Attr Handle
    0xC1, 0x2A, // UUID

    /* Object Last-Modified */
    0x0A,       // Properties
    0x7E, 0x00, // Attr Handle
    0xC2, 0x2A, // UUID

    /* Object ID */
    0x02,       // Properties
    0x80, 0x00, // Attr Handle
    0xC3, 0x2A, // UUID

    /* Object Properties */
    0x0A,       // Properties
    0x82, 0x00, // Attr Handle
    0xC4, 0x2A, // UUID

    /* Object Action Control Point */
    0x28,       // Properties
    0x84, 0x00, // Attr Handle
    0xC5, 0x2A, // UUID

    /* Object List Control Point */
    0x28,       // Properties
    0x87, 0x00, // Attr Handle
    0xC6, 0x2A, // UUID

    /* Object List Filter 0 */
    0x0A,       // Properties
    0x8A, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object List Filter 1 */
    0x0A,       // Properties
    0x8C, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object List Filter 2 */
    0x0A,       // Properties
    0x8E, 0x00, // Attr Handle
    0xC7, 0x2A, // UUID

    /* Object Changed */
    0x20,       // Properties
    0x90, 0x00, // Attr Handle
    0xC8, 0x2A, // UUID

};

static const uint8_t gs_gatt_db_const_peer_specific_val_arr[] =
{
    /* Service Changed : Client Characteristic Configuration */
    0x00, 0x00,

    /* Switch State : Client Characteristic Configuration */
    0x00, 0x00,

    /* Database Change Increment : Client Characteristic Configuration */
    0x00, 0x00,

    /* User Control Point : Client Characteristic Configuration */
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
        0x0072,
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
        0x008F,
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
        0x0091,
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

    /* 14 : User Data Service */
    {
        /* UUID Offset */
        74,
        /* First Occurrence for Type */
        0x0016,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 15 : First Name */
    {
        /* UUID Offset */
        76,
        /* First Occurrence for Type */
        0x0018,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 16 : Last Name */
    {
        /* UUID Offset */
        78,
        /* First Occurrence for Type */
        0x001A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 17 : Email Address */
    {
        /* UUID Offset */
        80,
        /* First Occurrence for Type */
        0x001C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 18 : Age */
    {
        /* UUID Offset */
        82,
        /* First Occurrence for Type */
        0x001E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 19 : Date of Birth */
    {
        /* UUID Offset */
        84,
        /* First Occurrence for Type */
        0x0020,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 20 : Gender */
    {
        /* UUID Offset */
        86,
        /* First Occurrence for Type */
        0x0022,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 21 : Weight */
    {
        /* UUID Offset */
        88,
        /* First Occurrence for Type */
        0x0024,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 22 : Height */
    {
        /* UUID Offset */
        90,
        /* First Occurrence for Type */
        0x0026,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 23 : VO2 Max */
    {
        /* UUID Offset */
        92,
        /* First Occurrence for Type */
        0x0028,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 24 : Heart Rate Max */
    {
        /* UUID Offset */
        94,
        /* First Occurrence for Type */
        0x002A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 25 : Resting Heart Rate */
    {
        /* UUID Offset */
        96,
        /* First Occurrence for Type */
        0x002C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 26 : Maximum Recommended Heart Rate */
    {
        /* UUID Offset */
        98,
        /* First Occurrence for Type */
        0x002E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 27 : Aerobic Threshold */
    {
        /* UUID Offset */
        100,
        /* First Occurrence for Type */
        0x0030,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 28 : Anaerobic Threshold */
    {
        /* UUID Offset */
        102,
        /* First Occurrence for Type */
        0x0032,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 29 : Sport Type for Aerobic and Anaerobic Thresholds */
    {
        /* UUID Offset */
        104,
        /* First Occurrence for Type */
        0x0034,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 30 : Date of Threshold Assessment */
    {
        /* UUID Offset */
        106,
        /* First Occurrence for Type */
        0x0036,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 31 : Waist Circumference */
    {
        /* UUID Offset */
        108,
        /* First Occurrence for Type */
        0x0038,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 32 : Hip Circumference */
    {
        /* UUID Offset */
        110,
        /* First Occurrence for Type */
        0x003A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 33 : Fat Burn Heart Rate Lower Limit */
    {
        /* UUID Offset */
        112,
        /* First Occurrence for Type */
        0x003C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 34 : Fat Burn Heart Rate Upper Limit */
    {
        /* UUID Offset */
        114,
        /* First Occurrence for Type */
        0x003E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 35 : Aerobic Heart Rate Lower Limit */
    {
        /* UUID Offset */
        116,
        /* First Occurrence for Type */
        0x0040,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 36 : Aerobic Heart Rate Upper Limit */
    {
        /* UUID Offset */
        118,
        /* First Occurrence for Type */
        0x0042,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 37 : Anaerobic Heart Rate Lower Limit */
    {
        /* UUID Offset */
        120,
        /* First Occurrence for Type */
        0x0044,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 38 : Anaerobic Heart Rate Upper Limit */
    {
        /* UUID Offset */
        122,
        /* First Occurrence for Type */
        0x0046,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 39 : Five Zone Heart Rate Limits */
    {
        /* UUID Offset */
        124,
        /* First Occurrence for Type */
        0x0048,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 40 : Three Zone Heart Rate Limits */
    {
        /* UUID Offset */
        126,
        /* First Occurrence for Type */
        0x004A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 41 : Two Zone Heart Rate Limit */
    {
        /* UUID Offset */
        128,
        /* First Occurrence for Type */
        0x004C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 42 : Database Change Increment */
    {
        /* UUID Offset */
        130,
        /* First Occurrence for Type */
        0x004E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 43 : User Index */
    {
        /* UUID Offset */
        132,
        /* First Occurrence for Type */
        0x0051,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 44 : User Control Point */
    {
        /* UUID Offset */
        134,
        /* First Occurrence for Type */
        0x0053,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 45 : Language */
    {
        /* UUID Offset */
        136,
        /* First Occurrence for Type */
        0x0056,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 46 : Heart Rate Service */
    {
        /* UUID Offset */
        138,
        /* First Occurrence for Type */
        0x0057,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 47 : Heart Rate Measurement */
    {
        /* UUID Offset */
        140,
        /* First Occurrence for Type */
        0x0059,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 48 : Body Sensor Location */
    {
        /* UUID Offset */
        142,
        /* First Occurrence for Type */
        0x005C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 49 : Heart Rate Control Point */
    {
        /* UUID Offset */
        144,
        /* First Occurrence for Type */
        0x005E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 50 : Device Information Service */
    {
        /* UUID Offset */
        146,
        /* First Occurrence for Type */
        0x005F,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 51 : Manufacturer Name String */
    {
        /* UUID Offset */
        148,
        /* First Occurrence for Type */
        0x0061,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 52 : Model Number String */
    {
        /* UUID Offset */
        150,
        /* First Occurrence for Type */
        0x0063,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 53 : Serial Number String */
    {
        /* UUID Offset */
        152,
        /* First Occurrence for Type */
        0x0065,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 54 : Hardware Revision String */
    {
        /* UUID Offset */
        154,
        /* First Occurrence for Type */
        0x0067,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 55 : Firmware Revision String */
    {
        /* UUID Offset */
        156,
        /* First Occurrence for Type */
        0x0069,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 56 : Software Revision String */
    {
        /* UUID Offset */
        158,
        /* First Occurrence for Type */
        0x006B,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 57 : System ID */
    {
        /* UUID Offset */
        160,
        /* First Occurrence for Type */
        0x006D,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 58 : IEEE 11073-20601 Regulatory Certification Data List */
    {
        /* UUID Offset */
        162,
        /* First Occurrence for Type */
        0x006F,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 59 : PnP ID */
    {
        /* UUID Offset */
        164,
        /* First Occurrence for Type */
        0x0071,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 60 : Object Transfer Service */
    {
        /* UUID Offset */
        166,
        /* First Occurrence for Type */
        0x0072,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 61 : OTS Feature */
    {
        /* UUID Offset */
        168,
        /* First Occurrence for Type */
        0x0074,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 62 : Object Name */
    {
        /* UUID Offset */
        170,
        /* First Occurrence for Type */
        0x0076,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 63 : Object Type */
    {
        /* UUID Offset */
        172,
        /* First Occurrence for Type */
        0x0078,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 64 : Object Size */
    {
        /* UUID Offset */
        174,
        /* First Occurrence for Type */
        0x007A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 65 : Object First-Created */
    {
        /* UUID Offset */
        176,
        /* First Occurrence for Type */
        0x007C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 66 : Object Last-Modified */
    {
        /* UUID Offset */
        178,
        /* First Occurrence for Type */
        0x007E,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 67 : Object ID */
    {
        /* UUID Offset */
        180,
        /* First Occurrence for Type */
        0x0080,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 68 : Object Properties */
    {
        /* UUID Offset */
        182,
        /* First Occurrence for Type */
        0x0082,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 69 : Object Action Control Point */
    {
        /* UUID Offset */
        184,
        /* First Occurrence for Type */
        0x0084,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 70 : Object List Control Point */
    {
        /* UUID Offset */
        186,
        /* First Occurrence for Type */
        0x0087,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 71 : Object List Filter 0 */
    {
        /* UUID Offset */
        188,
        /* First Occurrence for Type */
        0x008A,
        /* Last Occurrence for Type */
        0x008E,
    },

    /* 72 : Object Changed */
    {
        /* UUID Offset */
        190,
        /* First Occurrence for Type */
        0x0090,
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
        0x004F,
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
    /* User Data Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0057,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 74),
    },

    /* Handle : 0x0017 */
    /* First Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0019,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 68),
    },

    /* Handle : 0x0018 */
    /* First Name */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        76,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 141),
    },

    /* Handle : 0x0019 */
    /* Last Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x001B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 73),
    },

    /* Handle : 0x001A */
    /* Last Name */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        78,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 243),
    },

    /* Handle : 0x001B */
    /* Email Address : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x001D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 78),
    },

    /* Handle : 0x001C */
    /* Email Address */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        80,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 345),
    },

    /* Handle : 0x001D */
    /* Age : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 83),
    },

    /* Handle : 0x001E */
    /* Age */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        82,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 447),
    },

    /* Handle : 0x001F */
    /* Date of Birth : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 88),
    },

    /* Handle : 0x0020 */
    /* Date of Birth */
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
        84,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 448),
    },

    /* Handle : 0x0021 */
    /* Gender : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 93),
    },

    /* Handle : 0x0022 */
    /* Gender */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        86,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 452),
    },

    /* Handle : 0x0023 */
    /* Weight : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 98),
    },

    /* Handle : 0x0024 */
    /* Weight */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        88,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 453),
    },

    /* Handle : 0x0025 */
    /* Height : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 103),
    },

    /* Handle : 0x0026 */
    /* Height */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        90,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 455),
    },

    /* Handle : 0x0027 */
    /* VO2 Max : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 108),
    },

    /* Handle : 0x0028 */
    /* VO2 Max */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        92,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 457),
    },

    /* Handle : 0x0029 */
    /* Heart Rate Max : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 113),
    },

    /* Handle : 0x002A */
    /* Heart Rate Max */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        94,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 458),
    },

    /* Handle : 0x002B */
    /* Resting Heart Rate : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 118),
    },

    /* Handle : 0x002C */
    /* Resting Heart Rate */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        96,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 459),
    },

    /* Handle : 0x002D */
    /* Maximum Recommended Heart Rate : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 123),
    },

    /* Handle : 0x002E */
    /* Maximum Recommended Heart Rate */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        98,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 460),
    },

    /* Handle : 0x002F */
    /* Aerobic Threshold : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0031,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 128),
    },

    /* Handle : 0x0030 */
    /* Aerobic Threshold */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        100,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 461),
    },

    /* Handle : 0x0031 */
    /* Anaerobic Threshold : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0033,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 133),
    },

    /* Handle : 0x0032 */
    /* Anaerobic Threshold */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        102,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 462),
    },

    /* Handle : 0x0033 */
    /* Sport Type for Aerobic and Anaerobic Thresholds : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 138),
    },

    /* Handle : 0x0034 */
    /* Sport Type for Aerobic and Anaerobic Thresholds */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        104,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 463),
    },

    /* Handle : 0x0035 */
    /* Date of Threshold Assessment : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 143),
    },

    /* Handle : 0x0036 */
    /* Date of Threshold Assessment */
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
        106,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 464),
    },

    /* Handle : 0x0037 */
    /* Waist Circumference : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 148),
    },

    /* Handle : 0x0038 */
    /* Waist Circumference */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        108,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 468),
    },

    /* Handle : 0x0039 */
    /* Hip Circumference : Characteristic Declaration */
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
        (uint8_t *)(gs_gatt_const_value_arr + 153),
    },

    /* Handle : 0x003A */
    /* Hip Circumference */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        110,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 470),
    },

    /* Handle : 0x003B */
    /* Fat Burn Heart Rate Lower Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x003D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 158),
    },

    /* Handle : 0x003C */
    /* Fat Burn Heart Rate Lower Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        112,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 472),
    },

    /* Handle : 0x003D */
    /* Fat Burn Heart Rate Upper Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x003F,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 163),
    },

    /* Handle : 0x003E */
    /* Fat Burn Heart Rate Upper Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        114,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 473),
    },

    /* Handle : 0x003F */
    /* Aerobic Heart Rate Lower Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0041,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 168),
    },

    /* Handle : 0x0040 */
    /* Aerobic Heart Rate Lower Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        116,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 474),
    },

    /* Handle : 0x0041 */
    /* Aerobic Heart Rate Upper Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0043,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 173),
    },

    /* Handle : 0x0042 */
    /* Aerobic Heart Rate Upper Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        118,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 475),
    },

    /* Handle : 0x0043 */
    /* Anaerobic Heart Rate Lower Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0045,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 178),
    },

    /* Handle : 0x0044 */
    /* Anaerobic Heart Rate Lower Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        120,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 476),
    },

    /* Handle : 0x0045 */
    /* Anaerobic Heart Rate Upper Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0047,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 183),
    },

    /* Handle : 0x0046 */
    /* Anaerobic Heart Rate Upper Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        122,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 477),
    },

    /* Handle : 0x0047 */
    /* Five Zone Heart Rate Limits : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0049,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 188),
    },

    /* Handle : 0x0048 */
    /* Five Zone Heart Rate Limits */
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
        124,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 478),
    },

    /* Handle : 0x0049 */
    /* Three Zone Heart Rate Limits : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x004B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 193),
    },

    /* Handle : 0x004A */
    /* Three Zone Heart Rate Limits */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        126,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 482),
    },

    /* Handle : 0x004B */
    /* Two Zone Heart Rate Limit : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x004D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 198),
    },

    /* Handle : 0x004C */
    /* Two Zone Heart Rate Limit */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        128,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 484),
    },

    /* Handle : 0x004D */
    /* Database Change Increment : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0050,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 203),
    },

    /* Handle : 0x004E */
    /* Database Change Increment */
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
        130,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 485),
    },

    /* Handle : 0x004F */
    /* Database Change Increment : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0054,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 4),
    },

    /* Handle : 0x0050 */
    /* User Index : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0052,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 208),
    },

    /* Handle : 0x0051 */
    /* User Index */
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
        132,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 489),
    },

    /* Handle : 0x0052 */
    /* User Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0055,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 213),
    },

    /* Handle : 0x0053 */
    /* User Control Point */
    {
        /* Properties */
        BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        20,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        134,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 490),
    },

    /* Handle : 0x0054 */
    /* User Control Point : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x005A,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 6),
    },

    /* Handle : 0x0055 */
    /* Language : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0058,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 218),
    },

    /* Handle : 0x0056 */
    /* Language */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        136,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 510),
    },

    /* Handle : 0x0057 */
    /* Heart Rate Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x005F,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 138),
    },

    /* Handle : 0x0058 */
    /* Heart Rate Measurement : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x005B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 223),
    },

    /* Handle : 0x0059 */
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
        140,
        /* Value */
        (NULL),
    },

    /* Handle : 0x005A */
    /* Heart Rate Measurement : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0085,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 8),
    },

    /* Handle : 0x005B */
    /* Body Sensor Location : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x005D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 228),
    },

    /* Handle : 0x005C */
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
        142,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 612),
    },

    /* Handle : 0x005D */
    /* Heart Rate Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0060,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 233),
    },

    /* Handle : 0x005E */
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
        144,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 613),
    },

    /* Handle : 0x005F */
    /* Device Information Service : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0072,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 146),
    },

    /* Handle : 0x0060 */
    /* Manufacturer Name String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0062,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 238),
    },

    /* Handle : 0x0061 */
    /* Manufacturer Name String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        148,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 614),
    },

    /* Handle : 0x0062 */
    /* Model Number String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0064,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 243),
    },

    /* Handle : 0x0063 */
    /* Model Number String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        150,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 716),
    },

    /* Handle : 0x0064 */
    /* Serial Number String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0066,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 248),
    },

    /* Handle : 0x0065 */
    /* Serial Number String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        152,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 818),
    },

    /* Handle : 0x0066 */
    /* Hardware Revision String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0068,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 253),
    },

    /* Handle : 0x0067 */
    /* Hardware Revision String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        154,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 920),
    },

    /* Handle : 0x0068 */
    /* Firmware Revision String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x006A,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 258),
    },

    /* Handle : 0x0069 */
    /* Firmware Revision String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        156,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1022),
    },

    /* Handle : 0x006A */
    /* Software Revision String : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x006C,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 263),
    },

    /* Handle : 0x006B */
    /* Software Revision String */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        158,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1124),
    },

    /* Handle : 0x006C */
    /* System ID : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x006E,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 268),
    },

    /* Handle : 0x006D */
    /* System ID */
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
        160,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1226),
    },

    /* Handle : 0x006E */
    /* IEEE 11073-20601 Regulatory Certification Data List : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0070,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 273),
    },

    /* Handle : 0x006F */
    /* IEEE 11073-20601 Regulatory Certification Data List */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        162,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1234),
    },

    /* Handle : 0x0070 */
    /* PnP ID : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0073,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 278),
    },

    /* Handle : 0x0071 */
    /* PnP ID */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        7,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        164,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1336),
    },

    /* Handle : 0x0072 */
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
        (uint8_t *)(gs_gatt_const_uuid_arr + 166),
    },

    /* Handle : 0x0073 */
    /* OTS Feature : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0075,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 283),
    },

    /* Handle : 0x0074 */
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
        168,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1343),
    },

    /* Handle : 0x0075 */
    /* Object Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0077,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 288),
    },

    /* Handle : 0x0076 */
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
        170,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1351),
    },

    /* Handle : 0x0077 */
    /* Object Type : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0079,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 293),
    },

    /* Handle : 0x0078 */
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
        172,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1471),
    },

    /* Handle : 0x0079 */
    /* Object Size : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x007B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 298),
    },

    /* Handle : 0x007A */
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
        174,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1487),
    },

    /* Handle : 0x007B */
    /* Object First-Created : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x007D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 303),
    },

    /* Handle : 0x007C */
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
        176,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1495),
    },

    /* Handle : 0x007D */
    /* Object Last-Modified : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x007F,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 308),
    },

    /* Handle : 0x007E */
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
        178,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1502),
    },

    /* Handle : 0x007F */
    /* Object ID : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0081,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 313),
    },

    /* Handle : 0x0080 */
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
        180,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1509),
    },

    /* Handle : 0x0081 */
    /* Object Properties : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0083,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 318),
    },

    /* Handle : 0x0082 */
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
        182,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1515),
    },

    /* Handle : 0x0083 */
    /* Object Action Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0086,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 323),
    },

    /* Handle : 0x0084 */
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
        184,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1519),
    },

    /* Handle : 0x0085 */
    /* Object Action Control Point : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0088,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 10),
    },

    /* Handle : 0x0086 */
    /* Object List Control Point : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0089,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 328),
    },

    /* Handle : 0x0087 */
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
        186,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1540),
    },

    /* Handle : 0x0088 */
    /* Object List Control Point : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY | BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0091,
        /* UUID Offset */
        24,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 12),
    },

    /* Handle : 0x0089 */
    /* Object List Filter 0 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x008B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 333),
    },

    /* Handle : 0x008A */
    /* Object List Filter 0 */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x008C,
        /* UUID Offset */
        188,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1547),
    },

    /* Handle : 0x008B */
    /* Object List Filter 1 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x008D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 338),
    },

    /* Handle : 0x008C */
    /* Object List Filter 1 */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        100,
        /* Next Attribute Type Index */
        0x008E,
        /* UUID Offset */
        188,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1647),
    },

    /* Handle : 0x008D */
    /* Object List Filter 2 : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x008F,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 343),
    },

    /* Handle : 0x008E */
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
        188,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 1747),
    },

    /* Handle : 0x008F */
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
        (uint8_t *)(gs_gatt_const_value_arr + 348),
    },

    /* Handle : 0x0090 */
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
        190,
        /* Value */
        (NULL),
    },

    /* Handle : 0x0091 */
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
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 14),
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

    /* 8 : First Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0017,
        /* Service Index */
        3,
    },

    /* 9 : Last Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0019,
        /* Service Index */
        3,
    },

    /* 10 : Email Address */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001B,
        /* Service Index */
        3,
    },

    /* 11 : Age */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001D,
        /* Service Index */
        3,
    },

    /* 12 : Date of Birth */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x001F,
        /* Service Index */
        3,
    },

    /* 13 : Gender */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0021,
        /* Service Index */
        3,
    },

    /* 14 : Weight */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0023,
        /* Service Index */
        3,
    },

    /* 15 : Height */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0025,
        /* Service Index */
        3,
    },

    /* 16 : VO2 Max */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0027,
        /* Service Index */
        3,
    },

    /* 17 : Heart Rate Max */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0029,
        /* Service Index */
        3,
    },

    /* 18 : Resting Heart Rate */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x002B,
        /* Service Index */
        3,
    },

    /* 19 : Maximum Recommended Heart Rate */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x002D,
        /* Service Index */
        3,
    },

    /* 20 : Aerobic Threshold */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x002F,
        /* Service Index */
        3,
    },

    /* 21 : Anaerobic Threshold */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0031,
        /* Service Index */
        3,
    },

    /* 22 : Sport Type for Aerobic and Anaerobic Thresholds */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0033,
        /* Service Index */
        3,
    },

    /* 23 : Date of Threshold Assessment */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0035,
        /* Service Index */
        3,
    },

    /* 24 : Waist Circumference */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0037,
        /* Service Index */
        3,
    },

    /* 25 : Hip Circumference */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0039,
        /* Service Index */
        3,
    },

    /* 26 : Fat Burn Heart Rate Lower Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x003B,
        /* Service Index */
        3,
    },

    /* 27 : Fat Burn Heart Rate Upper Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x003D,
        /* Service Index */
        3,
    },

    /* 28 : Aerobic Heart Rate Lower Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x003F,
        /* Service Index */
        3,
    },

    /* 29 : Aerobic Heart Rate Upper Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0041,
        /* Service Index */
        3,
    },

    /* 30 : Anaerobic Heart Rate Lower Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0043,
        /* Service Index */
        3,
    },

    /* 31 : Anaerobic Heart Rate Upper Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0045,
        /* Service Index */
        3,
    },

    /* 32 : Five Zone Heart Rate Limits */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0047,
        /* Service Index */
        3,
    },

    /* 33 : Three Zone Heart Rate Limits */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0049,
        /* Service Index */
        3,
    },

    /* 34 : Two Zone Heart Rate Limit */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x004B,
        /* Service Index */
        3,
    },

    /* 35 : Database Change Increment */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x004D,
        /* Service Index */
        3,
    },

    /* 36 : User Index */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0050,
        /* Service Index */
        3,
    },

    /* 37 : User Control Point */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0052,
        /* Service Index */
        3,
    },

    /* 38 : Language */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0055,
        /* Service Index */
        3,
    },

    /* 39 : Heart Rate Measurement */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0058,
        /* Service Index */
        4,
    },

    /* 40 : Body Sensor Location */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x005B,
        /* Service Index */
        4,
    },

    /* 41 : Heart Rate Control Point */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x005D,
        /* Service Index */
        4,
    },

    /* 42 : Manufacturer Name String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0060,
        /* Service Index */
        5,
    },

    /* 43 : Model Number String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0062,
        /* Service Index */
        5,
    },

    /* 44 : Serial Number String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0064,
        /* Service Index */
        5,
    },

    /* 45 : Hardware Revision String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0066,
        /* Service Index */
        5,
    },

    /* 46 : Firmware Revision String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0068,
        /* Service Index */
        5,
    },

    /* 47 : Software Revision String */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x006A,
        /* Service Index */
        5,
    },

    /* 48 : System ID */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x006C,
        /* Service Index */
        5,
    },

    /* 49 : IEEE 11073-20601 Regulatory Certification Data List */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x006E,
        /* Service Index */
        5,
    },

    /* 50 : PnP ID */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0070,
        /* Service Index */
        5,
    },

    /* 51 : OTS Feature */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0073,
        /* Service Index */
        6,
    },

    /* 52 : Object Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0075,
        /* Service Index */
        6,
    },

    /* 53 : Object Type */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0077,
        /* Service Index */
        6,
    },

    /* 54 : Object Size */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0079,
        /* Service Index */
        6,
    },

    /* 55 : Object First-Created */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x007B,
        /* Service Index */
        6,
    },

    /* 56 : Object Last-Modified */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x007D,
        /* Service Index */
        6,
    },

    /* 57 : Object ID */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x007F,
        /* Service Index */
        6,
    },

    /* 58 : Object Properties */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0081,
        /* Service Index */
        6,
    },

    /* 59 : Object Action Control Point */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0083,
        /* Service Index */
        6,
    },

    /* 60 : Object List Control Point */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0086,
        /* Service Index */
        6,
    },

    /* 61 : Object List Filter 0 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0089,
        /* Service Index */
        6,
    },

    /* 62 : Object List Filter 1 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x008B,
        /* Service Index */
        6,
    },

    /* 63 : Object List Filter 2 */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x008D,
        /* Service Index */
        6,
    },

    /* 64 : Object Changed */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x008F,
        /* Service Index */
        6,
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

    /* User Data Service */
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
        0x0056,
        /* Characteristic Start Index */
        8,
        /* Characteristic End Index */
        38,
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
        0x0057,
        /* Service End Handle */
        0x005E,
        /* Characteristic Start Index */
        39,
        /* Characteristic End Index */
        41,
    },

    /* Device Information Service */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x005F,
        /* Service End Handle */
        0x0071,
        /* Characteristic Start Index */
        42,
        /* Characteristic End Index */
        50,
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
        0x0072,
        /* Service End Handle */
        0x0091,
        /* Characteristic Start Index */
        51,
        /* Characteristic End Index */
        64,
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