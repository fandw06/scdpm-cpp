/**
 ****************************************************************************************
 *
 * @file i2c_temp_sensor.c
 *
 * @brief PCT2202 driver over i2c interface implementation file.
 *
 ****************************************************************************************
 */
 
#include "i2c_temp_sensor.h"
#include "i2c_sensor.h"

#include "global_io.h"
#include "gpio.h"
#include "user_periph_setup.h"
#include "i2c_sensor.h"


extern void i2c_sensor_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size);
extern i2c_error_code i2c_sensor_read_byte(uint32_t address, uint8_t *byte);
extern i2c_error_code i2c_sensor_write_bytes(uint32_t address, uint8_t* byte, uint8_t len);

void temp_sensor_init()
{
	i2c_sensor_init(PCT2202_ADDRESS, PCT2202_SPEED_MODE, PCT2202_ADDRESS_MODE, PCT2202_ADDRESS_SIZE);	
}

void temp_sensor_config_onestop() {
	uint8_t cmd[2] = {0, 0};
	// MSB, 0b01100001
	cmd[0] = 0x61;
	// LSB
	cmd[1] = 0xa0;
	i2c_sensor_write_bytes(PCT2202_REG_CONFIG, cmd, 2);
}

i2c_error_code get_temp_bytes(uint8_t *data){
	uint8_t cmd[2] = {0, 0};
	// MSB, OS ready
	cmd[0] = 0xe1;
	// LSB
	cmd[1] = 0xa0;
	i2c_sensor_write_bytes(PCT2202_REG_CONFIG, cmd, 2);
	i2c_sensor_read_bytes(PCT2202_REG_TEMP, data, 2);
	return I2C_NO_ERROR;
}

