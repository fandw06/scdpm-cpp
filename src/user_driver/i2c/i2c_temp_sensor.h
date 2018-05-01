/**
 ****************************************************************************************
 *
 * @file i2c_temp_sensor.h
 *
 * @brief PCT2202 driver over i2c interface header file.
 *
 ****************************************************************************************
 */
 
#include "i2c_sensor.h"
 
#ifndef _I2C_TEMP_SENSOR_H_
#define _I2C_TEMP_SENSOR_H_

#define PCT2202_ADDRESS       0x48                 // connect A0 to GND      
#define PCT2202_SPEED_MODE    I2C_STANDARD         // fast mode (400 kbits/s)              */
#define PCT2202_ADDRESS_MODE  I2C_7BIT_ADDR        // 7-bit addressing                     */
#define PCT2202_ADDRESS_SIZE  I2C_1BYTE_ADDR       // 8-bit memory address  

#define PCT2202_REG_TEMP      0x00                 // temperature, read only
#define PCT2202_REG_CONFIG    0x01                 // configuration, rw        
#define PCT2202_REG_TLOW      0x02                 // t_low, rw
#define PCT2202_REG_THIGH     0x03                 // t_high, rw

// Configuration byte 2
#define PCT2202_EM            (1 << 4)             // extend mode, use 0
#define PCT2202_AL            (1 << 5)             // alert, r, unsued
#define PCT2202_CR0           (1 << 6)             
#define PCT2202_CR1           (1 << 7)             // conversion, default 4Hz.

// Configuration byte 1
#define PCT2202_SD            (1 << 0)             // shutting down, used, one-time conversion
#define PCT2202_TM            (1 << 1)             // thermostat mode, unused
#define PCT2202_POL           (1 << 2)             // polarity, unused
#define PCT2202_F0            (1 << 3)             // fault queue, unused
#define PCT2202_F1            (1 << 4)             // fault queue, unused
#define PCT2202_R0            (1 << 5)             // r
#define PCT2202_R1            (1 << 6)             // r
#define PCT2202_OS            (1 << 7)             // one shot mode, in SD mode, writting 1 to start a one shot conversion


void temp_sensor_init(void);

void temp_sensor_config_onestop(void);

i2c_error_code get_temp_bytes(uint8_t *data);


#endif // _I2C_TEMP_SENSOR_H_
