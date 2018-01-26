/**
 ****************************************************************************************
 *
 * @file user_custs1_def.h
 *
 * @brief Custom1 Server (CUSTS1) profile database declarations.
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_DEF_H_
#define _USER_CUSTS1_DEF_H_

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1 Server (CUSTS1) profile database declarations.
 *
 * @{
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */

#define DEF_CUST1_SVC_UUID_128             {0x2F, 0x2A, 0x93, 0xA6, 0xBD, 0xD8, 0x41, 0x52, 0xAC, 0x0B, 0x10, 0x99, 0x2E, 0xC6, 0xFE, 0xED}

#define DEF_CUST1_CTRL_POINT_UUID_128      {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define DEF_CUST1_SENSOR_VAL_UUID_128      {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

#define DEF_CUST1_CTRL_POINT_CHAR_LEN      1
#define DEF_CUST1_SENSOR_VAL_CHAR_LEN      20

#define CUST1_CONTROL_POINT_USER_DESC      "Control"
#define CUST1_SENSOR_VAL_USER_DESC         "Sensor"

/// Custom1 Service Data Base Characteristic enum
enum
{
    CUST1_IDX_SVC = 0,

    CUST1_IDX_CONTROL_POINT_CHAR,
    CUST1_IDX_CONTROL_POINT_VAL,
    CUST1_IDX_CONTROL_POINT_USER_DESC,

    CUST1_IDX_SENSOR_VAL_CHAR,
    CUST1_IDX_SENSOR_VAL_VAL,
    CUST1_IDX_SENSOR_VAL_NTF_CFG, 
    CUST1_IDX_SENSOR_VAL_USER_DESC,
	
    CUST1_IDX_NB
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern struct attm_desc_128 custs1_att_db[CUST1_IDX_NB];

/// @} USER_CONFIG

#endif // _USER_CUSTS1_DEF_H_
