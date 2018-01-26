/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.c
 *
 * @brief Custom1 Server implementation source code.
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gpio.h"
#include "app_api.h"
#include "app.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "user_main.h"
#include "user_periph_setup.h"
#include "adc.h"
#include "spi_adxl.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct user_data_buffer data_buff = {
    .SIZE = 20,
    .data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	  .pos = 0,
};

// The frequency ratio of accel, vol, and ecg.
// Total bytes should be no more than 20B.
// Eg, ratio = {2, 1, 6}, then total bytes in a packet is 2*3+1*2+6*2 = 20B
/*
const int ratio[3]    = {2, 1, 6}; 
const int interval[3] = {3, 6, 1};
*/

// acc:vol:ecg = 3:1:10, butes: 20B
const int ratio[3]    = {3, 1, 10}; 
const int interval[3] = {3, 10, 1};

uint8_t ecg_data;
uint8_t vol_data;
uint8_t acc_data[3];

ke_msg_id_t timer_base;
bool running;
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void user_custs1_ctrl_wr_ind_handler(ke_msg_id_t const msgid,
                                      struct custs1_val_write_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t val = 0;
    memcpy(&val, &param->value[0], param->length);

		if (val == CUSTS1_DATA_ENABLE) 
		{
				timer_base = app_easy_timer(BASE_INTERVAL, app_base_val_timer_cb_handler);
			  running = 1;
		}
		else 
		{
			  running = 0;
		}
}
/* this is for ratio 2-1-6, 2bytes.
void app_base_val_timer_cb_handler()
{
		switch (data_buff.pos) {
			  // ECG
				case 0:
				case 2:
				{
					  uint8_t *val = get_ecg();
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
						break;
				}
				// ECG, Acc
				case 4:
				{	
					  uint8_t *val = get_ecg();
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
					 	uint8_t *val2 = get_accel();
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						break;
				}
				// ECG, accel
				case 9:
				case 11:
				{
					 	uint8_t *val = get_ecg();
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
						break;		
				}
				case 13:
				{
						uint8_t *val = get_ecg();
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
					 	uint8_t *val2 = get_accel();
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
				    uint8_t *val3 = get_vol();
						data_buff.data[data_buff.pos++] = val3[0];
				    data_buff.data[data_buff.pos++] = val3[1];
				

						data_buff.pos = 0;
						struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																															TASK_CUSTS1,
																															TASK_APP,
																															custs1_val_ntf_req,
																															DEF_CUST1_SENSOR_VAL_CHAR_LEN);
						req->conhdl = app_env->conhdl;
						req->handle = CUST1_IDX_SENSOR_VAL_VAL;
						req->length = DEF_CUST1_SENSOR_VAL_CHAR_LEN;
						memcpy(req->value, &data_buff.data, DEF_CUST1_SENSOR_VAL_CHAR_LEN);
						ke_msg_send(req);
						break;
				}
				default:
						break;
		}

		if (ke_state_get(TASK_APP) == APP_CONNECTED && running)
		{
				if (running)
						timer_base = app_easy_timer(BASE_INTERVAL, app_base_val_timer_cb_handler);
		}
}
*/

void app_base_val_timer_cb_handler()
{
		switch (data_buff.pos) {
			  // ECG
				case 0:
				case 1:
				case 6:
				case 7:
				case 12:
				case 13:
				{
					  uint8_t val = get_ecg();
						data_buff.data[data_buff.pos++] = val;
						break;
				}
				// ECG, Acc
				case 2:
				case 8:
				case 14:
				{	
					  uint8_t val = get_ecg();
						data_buff.data[data_buff.pos++] = val;
					 	uint8_t *val2 = get_accel();
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						break;
				}
				// ECG, vol
				case 18:
				{
					 	uint8_t val = get_ecg();
						data_buff.data[data_buff.pos++] = val;
					  val = get_vol();
				    data_buff.data[data_buff.pos++] = val;
					 	
					
					  data_buff.pos = 0;
						struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																															TASK_CUSTS1,
																															TASK_APP,
																															custs1_val_ntf_req,
																															DEF_CUST1_SENSOR_VAL_CHAR_LEN);
						req->conhdl = app_env->conhdl;
						req->handle = CUST1_IDX_SENSOR_VAL_VAL;
						req->length = DEF_CUST1_SENSOR_VAL_CHAR_LEN;
						memcpy(req->value, &data_buff.data, DEF_CUST1_SENSOR_VAL_CHAR_LEN);
						ke_msg_send(req);
						break;		
				}
				default:
						break;
		}

		if (ke_state_get(TASK_APP) == APP_CONNECTED && running)
		{
				if (running)
						timer_base = app_easy_timer(BASE_INTERVAL, app_base_val_timer_cb_handler);
		}
}

uint8_t get_ecg()
{
		// Initialize adc, channel 00
	  adc_init(GP_ADC_SE, 0, 0);
	  adc_enable_channel(ADC_CHANNEL_P00);
		int data = adc_get_sample();

		ecg_data = (data >> 2);
		return ecg_data;
}

uint8_t get_vol()
{
		// Initialize adc, channel 01
	  adc_init(GP_ADC_SE, 0, GP_ADC_ATTN3X);
	  adc_enable_channel(ADC_CHANNEL_P01);
		int data = adc_get_sample();
	  vol_data = (data >> 2);
		return vol_data;
}

uint8_t* get_accel()
{
    acc_data[0] = read_accel(XDATA);
		acc_data[1] = read_accel(YDATA);
		acc_data[2] = read_accel(ZDATA);
		return acc_data;
}

/**
 ****************************************************************************************
 * @brief Turn the radio off according to the current sleep_mode and check if we can go
 *        into deep sleep.
 * @param[in] current_sleep_mode The current sleep mode proposed by the system so far
 * @return sleep_mode_t return the allowable sleep mode
 ****************************************************************************************
 */
static inline void ble_turn_radio_off(void)
{
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 1); // turn off radio
}

static inline void ble_turn_radio_on(void)
{
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 0); // turn off radio
}