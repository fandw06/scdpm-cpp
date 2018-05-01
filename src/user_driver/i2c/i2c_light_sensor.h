/**
 ****************************************************************************************
 *
 * @file i2c_light_sensor.h
 *
 * @brief MAX44009 driver over i2c interface header file.
 *
 ****************************************************************************************
 */
 
#include "i2c_sensor.h"
 
#ifndef _I2C_LIGHT_SENSOR_H_
#define _I2C_LIGHT_SENSOR_H_

//0x4a
#define MAX_44009_ADDRESS       0x4A             // connect A0 to ground              */
#define MAX_44009_SPEED_MODE    I2C_STANDARD         // fast mode (400 kbits/s)              */
#define MAX_44009_ADDRESS_MODE  I2C_7BIT_ADDR    // 7-bit addressing                     */
#define MAX_44009_ADDRESS_SIZE  I2C_1BYTE_ADDR   // 8-bit memory address  

// Register address
#define STATUS_INT   0x00
#define EN_INT       0x01
#define CONFIG       0x02
// High: E3~E0 M7~M4
#define LUX_HIGH     0x03
// Low:  ----  M3~M0
#define LUX_LOW      0x04
#define THR_HIGH     0x05
#define THR_LOW      0x06
#define THR_TIMER    0x07

void light_sensor_init(void);

i2c_error_code get_lux_bytes(uint8_t *data);

#endif // _I2C_SENSOR_H_
