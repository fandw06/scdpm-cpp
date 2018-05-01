/**
 ****************************************************************************************
 *
 * @file i2c_light_sensor.c
 *
 * @brief MAX44009 driver over i2c interface implementation file.
 *
 ****************************************************************************************
 */
 
#include "i2c_light_sensor.h"

extern void i2c_sensor_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size);
extern i2c_error_code i2c_sensor_read_byte(uint32_t address, uint8_t *byte);

void light_sensor_init()
{
	i2c_sensor_init(MAX_44009_ADDRESS, MAX_44009_SPEED_MODE, MAX_44009_ADDRESS_MODE, MAX_44009_ADDRESS_SIZE);	
}

i2c_error_code get_lux_bytes(uint8_t *data)
{
	uint8_t high = 0;
	uint8_t low = 0;
	i2c_sensor_read_byte(0x03, &high);
	i2c_sensor_read_byte(0x04, &low);
	*data = low;
	*(data + 1) = high;
	return I2C_NO_ERROR;
}
