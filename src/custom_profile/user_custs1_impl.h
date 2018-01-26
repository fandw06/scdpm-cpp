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

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

enum
{
    CUSTS1_DATA_DISABLE = 0,
    CUSTS1_DATA_ENABLE,
};

/**
 ****************************************************************************************
 * @brief A data buffer for ecg and accel data.
 ****************************************************************************************
*/
struct user_data_buffer {
    const uint8_t SIZE;
	  uint8_t data[20];
    uint8_t pos;
};


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "custs1_task.h"
#include "datasheet.h"
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
 * @brief Base sampling timer callback handler.
 * @return void
 ****************************************************************************************
 */
void app_base_val_timer_cb_handler(void);																					
															
uint8_t get_ecg(void);
uint8_t get_vol(void);
uint8_t* get_accel(void);
static inline void ble_turn_radio_off(void);
static inline void ble_turn_radio_on(void);
/// @} APP

#endif // _USER_CUSTS1_IMPL_H_
