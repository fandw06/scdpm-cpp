/**
 ****************************************************************************************
 *
 * @file spi_adxl.c
 *
 * @brief Adxl (ADXL 362) functions
 *
 * Copyright (C) 2017. Dawei Fan (df5ah@virginia.edu)
 *
 ****************************************************************************************
 */ 
#include "spi_adxl.h"
#include "user_periph_setup.h"
#include "spi.h"
  
 /**
 ****************************************************************************************
 * @brief Initialize adxl chip.
 * 
 ****************************************************************************************
 */
void adxl_init(uint8_t odr)
{
	
	// Initialize SPI asxl
	SPI_Pad_t spi_ADXL_Pad;
	spi_ADXL_Pad.pin = SPI_CS_PIN;
  spi_ADXL_Pad.port = SPI_GPIO_PORT;
	spi_init(&spi_ADXL_Pad, SPI_MODE_16BIT, SPI_ROLE_MASTER, SPI_CLK_IDLE_POL_LOW, SPI_PHA_MODE_0, SPI_MINT_DISABLE, SPI_XTAL_DIV_8);
	
	// Set activity and inactivity level.
	write_register(THR_ACT_L, 1);
	write_register(THR_ACT_H, 0);
	write_register(TIME_ACT, 3);
	
	write_register(THR_INACT_L, 1);
	write_register(THR_INACT_H, 0);
	write_register(TIME_INACT_L, 3);
	write_register(TIME_INACT_H, 0);
	
	write_register(ACT_INACT_CTL, (1 | (1 << 2)));
	
	// +-2g range, default is 50Hz
	write_register(FILTER_CTL, (0x10 | odr));  
	
	// Turn measurement on
	write_register(POWER_CTL, 2);  
}

/**
 ****************************************************************************************
 * @brief Read one byte value of acceleration data.
 * @param channel_address 
 * @return void
 ****************************************************************************************
 */
uint8_t read_accel(uint8_t channel_address)
{
	return read_register(channel_address);
}

/**
 ****************************************************************************************
 * @brief Read DEVID_AD.
 * @return DEVID_AD, default is 0xAD
 ****************************************************************************************
 */
uint8_t read_DEVID_AD()
{
	return read_register(DEVID_AD);
}	

/**
 ****************************************************************************************
 * @brief Read DEVID_MST.
 * @return DEVID_MST, default is 0x1D
 ****************************************************************************************
 */
uint8_t read_DEVID_MST()
{
	return read_register(DEVID_MST);
}	

/**
 ****************************************************************************************
 * @brief Read PARTID.
 * @return PARTID, default is 0xF2
 ****************************************************************************************
 */
uint8_t read_PARTID()
{
	return read_register(PARTID);
}	

/**
 ****************************************************************************************
 * @brief Read REVID.
 * @return REVID, default is 0x01
 ****************************************************************************************
 */
uint8_t read_REVID()
{
	return read_register(REVID);
}	

/**
 ****************************************************************************************
 * @brief Read STATUS.
 * @return STATUS
 ****************************************************************************************
 */
uint8_t read_STATUS(void)
{
	return read_register(STATUS);
}

/**
 ****************************************************************************************
 * @brief Read register.
 * @param register_address
 * @return Register value
 ****************************************************************************************
 */
uint8_t read_register(uint8_t register_address)
{
	spi_set_bitmode(SPI_MODE_16BIT);    
	spi_cs_low();            			            
	spi_access((READ << 8) | register_address);             
	spi_set_bitmode(SPI_MODE_8BIT); 
  uint8_t value = spi_access(0x00);
	spi_cs_high();
	return value;
}

/**
 ****************************************************************************************
 * @brief Write register.
 * @param register_address
 *        value
 * @return Register value
 ****************************************************************************************
 */
void write_register(uint8_t register_address, uint8_t value)
{
	spi_set_bitmode(SPI_MODE_32BIT);    
	spi_cs_low();            			            
	spi_access((WRITE << 24) | (register_address << 16) | (value << 8));             
	spi_cs_high();	
}

/**
 ****************************************************************************************
 * @brief Reset all registers value.
 * @param void
 * @return void
 ****************************************************************************************
 */
void reset(void)
{
	write_register(SOFT_RST, 0x52);
}
