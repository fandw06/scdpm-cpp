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
#include "i2c_light_sensor.h"
#include "i2c_temp_sensor.h"
#include "pwm.h"

#define RELOAD_100US      20                     // reload value fo 100us
#define NO_PWM            0x0 
//unit is 10ms
//Sampling frequency of three sensors
uint16_t BASE_INTV = 2;

// Data scheme
uint8_t scheme = 2;

uint8_t times = 100;

static uint8_t order = 0;


// Since there are 10bits of ECG, store the 2 LSB of 8 ECG sample into another short value ECG LSB
uint16_t ecgLSB16 = 0;

uint32_t ecgLSB32 = 0;

// ADC channels
// ECG: 00
// Vcap: 01

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
		// Receive 4Bytes command from the master, phone.
    uint8_t cmd[] = {0, 0, 0, 0};
    memcpy(&cmd, &param->value[0], param->length);

		// Decode the command
		switch (cmd[0]) {
			// Stream or stop
			case 0:
			{
				if (cmd[1] == CUSTS1_DATA_ENABLE_BLE) 
				{
					running = 1;
					// Change the function for different cases.
					//timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler_test);	
					//timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_empty);			
					//timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler_case1);			
					//timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme3);						
					//light_sensor_init();
				}
				else if (cmd[1] == CUSTS1_DATA_ENABLE_TIMER)
				{
					running = 1;
					timer0_setup();
				}
				break;
			}
			// Change the sampling rate of the sensors.
			// The unit of BASE_INTV is 10ms.
			case 1:
			{
				// The sensor to change sampling interval.
				uint8_t sensor = cmd[1];
				BASE_INTV = cmd[2] * 256 + cmd[3];
				break;
			}
			
			// Change connection interval, the unit is ms.
			case 2:
			{
				uint16_t conn_intv = cmd[1] * 256 + cmd[2];
				app_param_update_func(conn_intv);
				break;
			}
			// change data scheme, one byte para
			case 3:
			{
				scheme = cmd[1];
				data_buff.pos = 0;
				running = 1;

				app_easy_timer_cancel(timer_base);
				
				switch(scheme) {
					case 0:
						timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme0);	
						break;
					case 1:
						timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme1);	
						break;
					case 2:
						timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme2);	
						break;
					case 3:
						timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme3);	
						break;
					case 4:
						timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme4);	
						break;
					default:
						break;
				}	
				break;
			}

			defaut:
			{
				printf("Wrong command!");
				break;
			}
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
						break
				}
				default:
						break;
		}

		if (ke_state_get(TASK_APP) == APP_CONNECTED && running)
		{
				if (running)
						timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler);
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
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = val &0xFF;
						break;
				}
				// ECG, Acc
				case 2:
				case 8:
				case 14:
				{	
					  uint16_t val = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = val & 0xFF;
					 	uint8_t val2[] = {0, 0, 0};
						get_accel(val2);
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						break;
				}
				// ECG, vol
				case 18:
				{
					 	uint16_t val = 0;
						uint16_t val2 = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = val & 0xFF;
					  get_vol(&val2);
				    data_buff.data[data_buff.pos++] = ((val2 >> 2) & 0xFF) ;
					 	
					
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
			timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler);
		}
}


void app_base_val_timer_cb_handler_test() 
{
		uint8_t val[] = {0, 0};
	  /*
		// Test light sensor
		light_sensor_init();
		uint8_t* val = get_lux_bytes();
		*/
		// Test temperature sensor
		//temp_sensor_init();
		//
		//query_device(val);
		//query_rev(val);
		//get_temp_bytes(FAST, HOLD, val);
		//get_temp_bytes(NORM, NO_HOLD, val);
		
		// Test light sensor
		/*
		light_sensor_init();
		uint8_t* val = get_lux_bytes();
		
		
		data_buff.data[0] = val[0];
		data_buff.data[1] = val[1];
		data_buff.data[2] = 0;
		data_buff.data[3] = 0;
		data_buff.data[4] = 0;
		data_buff.data[5] = 0;
		
		*/
		
		// Test temp sensor
		/*
		temp_sensor_init();
		temp_sensor_config_onestop();
		get_temp_bytes(val);
		*/
		
		data_buff.data[0] = val[0];
		data_buff.data[1] = val[1];
		data_buff.data[2] = 0;
		data_buff.data[3] = 0;
		
		uint8 vol = 0;
//		get_vol(&vol);
		data_buff.data[4] = vol;
		data_buff.data[5] = 0;
		
		
		/*
		data_buff.data[0] = order;
		data_buff.data[1] = APP_CONNECTED;
		data_buff.data[2] = running;
		data_buff.data[3] = ke_state_get(TASK_APP);
		
		uint16_t ttt = 1203/1.25;
		data_buff.data[4] = (ttt>>8);
		data_buff.data[5] = (ttt & 0xff);
		*/
		uint8_t val_acc[3] = {0, 0, 0};
		get_accel(val_acc);
		data_buff.data[6] = val_acc[0];
		data_buff.data[7] = val_acc[1];
		data_buff.data[8] = val_acc[2];
		
		/*
		for (int i = 6; i< 40; i++)
			data_buff.data[i] = i;
		*/
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

		order++;
		//timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler_test);
		
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_val_timer_cb_handler_test);
		}
}



void app_base_timer_handler_scheme0()
{
		switch (data_buff.pos) {
			  // ECG
				case 0:
				case 1:
				case 2:
				case 3:
				case 8:
				case 9:
				case 10:
				case 11:					
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						break;
				}
				// ECG, Acc
				case 4:
				{	
					  uint16_t val = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
					 	uint8_t val2[] = {0, 0, 0};
						get_accel(val2);
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						break;
				}
				// ECG, accel, vol, temp, light
				case 12:
				{
					  //ecg
					 	uint16_t val = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						
						//accel
						uint8_t val2[] = {0, 0, 0};
						get_accel(val2);
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						
						// vol
						uint16_t val3 = 0;
					  get_vol(&val3);
				    data_buff.data[data_buff.pos++] = (val3 >> 2);
					 	
						// temp
						uint8_t val4[] = {0, 0};
						temp_sensor_init();
						temp_sensor_config_onestop();
						get_temp_bytes(val4);
						data_buff.data[data_buff.pos++] = val4[0];
						data_buff.data[data_buff.pos++] = val4[1];
						
						// light
						uint8_t val5[] = {0, 0};
						light_sensor_init();
						get_lux_bytes(val5);
						//data_buff.data[data_buff.pos++] = val5[0];
						data_buff.data[data_buff.pos++] = val5[1];
												
						// send packet to kernel
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
		
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme0);
		}
}

void app_base_timer_handler_scheme1()
{
		switch (data_buff.pos) {
			  // ECG
				case 0:
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 0);
						break;
				}
				case 1:
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 2);
						break;
				}
				case 2:
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 4);
						break;
				}
				case 7:
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 8);
						break;
				}
				case 8:
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 10);
						break;
				}
				case 9:					
				{
					  uint16_t val = 0;
					  get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 12);
						break;
				}
				// ECG, Acc
				case 3:
				{	
					  uint16_t val = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 6);
					 	uint8_t val2[] = {0, 0, 0};
						get_accel(val2);
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						break;
				}
				// ECG, accel, vol, temp, light
				case 10:
				{
					  //ecg
					 	uint16_t val = 0;
						get_ecg(&val);
						data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
						ecgLSB16 |= ((val & 0x3)<< 14);
						
						//accel
						uint8_t val2[] = {0, 0, 0};
						get_accel(val2);
						data_buff.data[data_buff.pos++] = val2[0];
				    data_buff.data[data_buff.pos++] = val2[1];
				    data_buff.data[data_buff.pos++] = val2[2];
						
						// vol
						uint16_t val3 = 0;
					  get_vol(&val3);
				    data_buff.data[data_buff.pos++] = (val3 >> 2);
					 	
						// temp
						uint8_t val4[] = {0, 0};
						temp_sensor_init();
						temp_sensor_config_onestop();
						get_temp_bytes(val4);
						data_buff.data[data_buff.pos++] = val4[0];
						data_buff.data[data_buff.pos++] = val4[1];
						
						// light
						uint8_t val5[] = {0, 0};
						light_sensor_init();
						get_lux_bytes(val5);
						//data_buff.data[data_buff.pos++] = val5[0];
						data_buff.data[data_buff.pos++] = val5[1];
												
						// ECG LSBs
						data_buff.data[data_buff.pos++] = ecgLSB16 & 0xFF;
						data_buff.data[data_buff.pos++] = (ecgLSB16 >> 8) & 0xFF;
						ecgLSB16 = 0;
						// send packet to kernel
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
		
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme1);
		}
}

void app_base_timer_handler_scheme2()
{
		switch (data_buff.pos) {
			  // accel
				case 0:
				case 8:
				{
					  //accel
						uint8_t val[] = {0, 0, 0};
						get_accel(val);
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
				    data_buff.data[data_buff.pos++] = val[2];
						break;
				}
				// accel, vol
				case 3:
				{
					  //accel
						uint8_t val[] = {0, 0, 0};
						get_accel(val);
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
				    data_buff.data[data_buff.pos++] = val[2];
						// vol
						uint16_t val2 = 0;
					  get_vol(&val2);
				    data_buff.data[data_buff.pos++] = ((val2 >> 2)&0xFF);
						data_buff.data[data_buff.pos++] = (val2 & 0x03);
						break;
				}
				// accel, vol, temp, light
				case 11:
				{
					  //accel
						uint8_t val[] = {0, 0, 0};
						get_accel(val);
						data_buff.data[data_buff.pos++] = val[0];
				    data_buff.data[data_buff.pos++] = val[1];
				    data_buff.data[data_buff.pos++] = val[2];
						// vol
						uint16_t val2 = 0;
					  get_vol(&val2);
				    data_buff.data[data_buff.pos++] = ((val2 >> 2)&0xFF);
						data_buff.data[data_buff.pos++] = (val2 & 0x03);
						
						// temp
						uint8_t val4[] = {0, 0};
						temp_sensor_init();
						temp_sensor_config_onestop();
						get_temp_bytes(val4);
						data_buff.data[data_buff.pos++] = val4[0];
						data_buff.data[data_buff.pos++] = val4[1];
						
						// light
						uint8_t val5[] = {0, 0};
						light_sensor_init();
						get_lux_bytes(val5);
						data_buff.data[data_buff.pos++] = val5[0];
						data_buff.data[data_buff.pos++] = val5[1];
						
						// send packet to kernel
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
		
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme2);
		}
}

void app_base_timer_handler_scheme3()
{
		if (data_buff.pos < 18) {
			//accel
			uint8_t val[] = {0, 0, 0};
			get_accel(val);
			data_buff.data[data_buff.pos++] = val[0];
			data_buff.data[data_buff.pos++] = val[1];
		  data_buff.data[data_buff.pos++] = val[2];
		}
		else {
			// send packet to kernel
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
		}
		
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme3);
		}
}






void app_base_timer_handler_scheme4()
{
	  uint8_t offset = 2 * data_buff.pos;
	  if (data_buff.pos < 15) {
			uint16_t val = 0;
			get_ecg(&val);
			data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
			ecgLSB32 |= ((val & 0x3)<< offset);
		}
		else {
			uint16_t val = 0;
			get_ecg(&val);
			data_buff.data[data_buff.pos++] = ((val >> 2)&0xFF);
			ecgLSB32 |= ((val & 0x3)<< offset);
			
			data_buff.data[data_buff.pos++] = (ecgLSB32 & 0xFF);
			data_buff.data[data_buff.pos++] = ((ecgLSB32 >> 8) & 0xFF);
			data_buff.data[data_buff.pos++] = ((ecgLSB32 >> 16) & 0xFF);
			data_buff.data[data_buff.pos++] = ((ecgLSB32 >> 24) & 0xFF);
			
			data_buff.pos = 0;
			ecgLSB32 = 0;			
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
		}
		
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_scheme4);
		}
}



void app_base_timer_handler_empty()
{	
		/*
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
		*/
		// Repeate the timer_hander.
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer_base = app_easy_timer(BASE_INTV, app_base_timer_handler_empty);
		}
}



uint8_t get_ecg(uint16_t *ecg)
{
		// Initialize adc, channel 00
	  adc_init(GP_ADC_SE, 0, 0);
	  adc_enable_channel(ADC_CHANNEL_P00);
		int data = adc_get_sample();
	
		// 10 bit ecg data
		*ecg = (data & 0x3FF);	
		return 0;
}

uint8_t get_vol(uint16_t *vol)
{
		// Initialize adc, channel 01
	  adc_init(GP_ADC_SE, 0, GP_ADC_ATTN3X);
	  adc_enable_channel(ADC_CHANNEL_P01);
		int data = adc_get_sample();
	  *vol = (data & 0x3FF);
		return 0;
}

uint8_t get_accel(uint8_t *accel)
{
    *(accel++) = read_accel(XDATA);
		*(accel++) = read_accel(YDATA);
		*accel = read_accel(ZDATA);
		return 0;
}


void app_param_update_func(uint16_t intv_min)
{
		struct gapc_param_update_cmd * req = KE_MSG_ALLOC(
																						GAPC_PARAM_UPDATE_CMD, 
																						TASK_GAPC, 
																						TASK_APP, 
																						gapc_param_update_cmd); // Fill in the parameter structure
		uint16_t n_min = intv_min / 1.25;
		req->operation = GAPC_UPDATE_PARAMS; 
		req->params.intv_min = n_min;	// N * 1.25ms
		req->params.intv_max = n_min + 10;	// N * 1.25ms
		req->params.latency = 0;	// Conn Events skipped
		req->params.time_out = 400;	// N * 10ms
		ke_msg_send(req);
		return;
}


// Timer 0 test.

void timer0_general_user_callback_function(void) 
{

    uint8_t val_acc[3] = {0, 0, 0};
		get_accel(val_acc);
		//if (data_buff.pos < 16) {
			data_buff.data[data_buff.pos++] = times;
			data_buff.data[data_buff.pos++] = val_acc[0];
			data_buff.data[data_buff.pos++] = val_acc[1];
			data_buff.data[data_buff.pos++] = val_acc[2];
		//}
		
		/*
		for (int i = 6; i< 40; i++)
			data_buff.data[i] = i;
		*/
		//else {
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
			
		//}
		
		times--;
		//timer0_start();
		/*
		if(((ke_state_get(TASK_APP) == APP_CONNECTED) || (ke_state_get(TASK_APP) == APP_PARAM_UPD))&& running)
		{
			timer0_setup();
		}
		*/
}

void timer0_setup()
{
    // Stop timer for enter settings
    timer0_stop();
		SetWord16(SET_FREEZE_REG,FRZ_WDOG); 
		times = 100;
	
    // register callback function for SWTIM_IRQn irq
    timer0_register_callback(timer0_general_user_callback_function);

    // Enable TIMER0 clock
    set_tmr_enable(CLK_PER_REG_TMR_ENABLED);

    // Sets TIMER0,TIMER2 clock division factor to 8, so TIM0 Fclk is F = 16MHz/8 = 2Mhz
    set_tmr_div(CLK_PER_REG_TMR_DIV_8);

    // clear PWM settings register to not generate PWM
    timer0_set_pwm_high_counter(NO_PWM);
    timer0_set_pwm_low_counter(NO_PWM);
    
    // Set timer with 2MHz source clock divided by 10 so Fclk = 2MHz/10 = 200kHz
    timer0_init(TIM0_CLK_32K, PWM_MODE_ONE, TIM0_CLK_DIV_BY_10);

    // reload value for 100ms (T = 1/200kHz * RELOAD_100MS = 0,000005 * 20 = 100us = 0.1ms)
    timer0_set_pwm_on_counter(RELOAD_100US); //500us

    // Enable SWTIM_IRQn irq
    timer0_enable_irq();
    
		
    // Start Timer0
    timer0_start();
		
		while (times);
		
	  //set_tmr_enable(CLK_PER_REG_TMR_DISABLED);
		
}




