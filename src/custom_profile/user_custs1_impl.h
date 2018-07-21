/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.h
 *
 * @brief Peripheral project Custom1 Server implementation header file.
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_IMPL_H_
#define _USER_CUSTS1_IMPL_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "custs1_task.h"
#include "datasheet.h"
#include "user_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */

enum
{
    CUSTS1_DATA_DISABLE = 0,
    CUSTS1_DATA_ENABLE_BLE,
};


/**
 ****************************************************************************************
 * @brief A data buffer for ecg and accel data.
 ****************************************************************************************
*/
struct user_data_buffer {
    const uint8_t SIZE;
	  uint8_t data[PACKET_LEN];
    uint8_t pos;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Control point write indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 * @return void
 ****************************************************************************************
*/
void user_custs1_ctrl_wr_ind_handler(ke_msg_id_t const msgid,
                                      struct custs1_val_write_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
																		
																						
/**
 ****************************************************************************************
 * @brief Base sampling timer callback handlers.
				Scheme0~N are for product, and others are for testing. 
 * @return void
 ****************************************************************************************
 */
void app_base_val_timer_cb_handler(void);			

// Test schemes																			
void app_base_timer_handler_empty(void);																			
void app_base_timer_handler_test_basic(void);		
																			
// Product schemes
// Full 51																			
void app_base_timer_handler_scheme0(void);			
// Full 41
void app_base_timer_handler_scheme1(void);	
// Environment tracker
void app_base_timer_handler_scheme2(void);	
// Motion tracker
void app_base_timer_handler_scheme3(void);	
// ECG tracker		
void app_base_timer_handler_scheme4(void);
// ECG + accel + vol, used for boards without sensor board.
void app_base_timer_handler_scheme5(void);


uint8_t get_ecg(uint16_t *ecg);
uint8_t get_vol(uint16_t *vol);
uint8_t get_accel(uint8_t *accel);

void app_param_update_func(uint16_t intv_min);
/// @} APP

#endif // _USER_CUSTS1_IMPL_H_
