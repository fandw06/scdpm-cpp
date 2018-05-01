/**
 ****************************************************************************************
 *
 * @file i2c_sensor.h
 *
 * @brief i2c sensor(light, temperature, etc) driver over i2c interface header file.
 *        This is adapted from 'i2c_eeprom.h'. 
 *
 ****************************************************************************************
 */


#ifndef _I2C_SENSOR_H_
#define _I2C_SENSOR_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define I2C_MAX_RETRIES                 (200000)

enum I2C_SPEED_MODES
{
    I2C_STANDARD = 1,
    I2C_FAST,
};

enum I2C_ADDRESS_MODES
{
    I2C_7BIT_ADDR,
    I2C_10BIT_ADDR,
};

enum I2C_ADRESS_BYTES_COUNT
{
    I2C_1BYTE_ADDR,
    I2C_2BYTES_ADDR,
    I2C_3BYTES_ADDR,
};

typedef enum
{
    I2C_NO_ERROR,
    I2C_7B_ADDR_NOACK_ERROR,
    I2C_INVALID_EEPROM_ADDRESS
} i2c_error_code;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize I2C controller as a master for I2C sensor handling.
 * @param[in] dev_address  Slave device address
 * @param[in] speed        Speed
 * @param[in] address_mode Addressing mode
 * @param[in] address_size Address size
 * @retun void
 ****************************************************************************************
 */
void i2c_sensor_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size);

/**
 ****************************************************************************************
 * @brief Disable I2C controller and clock.
 * @return void
 ****************************************************************************************
 */
void i2c_sensor_release(void);

/**
 ****************************************************************************************
 * @brief Poll until I2C device is ready.
 * @return i2c_error_code Enumeration type that defines the returned error code
 ****************************************************************************************
 */
i2c_error_code i2c_wait_until_device_ready(void);

/**
 ****************************************************************************************
 * @brief Read single byte from I2C sensor.
 * @param[in] address     Memory address to read the byte from
 * @param[in|out] byte    Pointer to the read byte
 * @return i2c_error_code Enumeration type that defines the returned error code
 ****************************************************************************************
 */
i2c_error_code i2c_sensor_read_byte(uint32_t address, uint8_t *byte);

/**
 ****************************************************************************************
 * @brief Write single byte to I2C sensor.
 * @param[in] address     Memory position to write the byte to
 * @param[in] byte        Byte to be written
 * @return i2c_error_code Enumeration type that defines the returned error code
 ****************************************************************************************
 */
i2c_error_code i2c_sensor_write_byte(uint32_t address, uint8_t byte);

i2c_error_code i2c_sensor_write_bytes(uint32_t address, uint8_t* byte, uint8_t len);

i2c_error_code i2c_sensor_read_bytes(uint32_t address, uint8_t *byte, uint8_t len);
#endif // _I2C_SENSOR_H_
