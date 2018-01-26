/**
 ****************************************************************************************
 *
 * @file spi_adxl.h
 *
 * @brief Adxl (ADXL 362) functions
 *
 * Copyright (C) 2017. Dawei Fan (df5ah@virginia.edu)
 *
 ****************************************************************************************
 */

#ifndef _SPI_ADXL_H_
#define _SPI_ADXL_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "spi.h"
#include "global_io.h"
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

// ADXL commands
#define READ            0x0B         // Read register
#define WRITE           0x0A         // Write register
#define READ_FIFO       0x0D         // Read FIFO

// Register map
#define DEVID_AD        0x00         // Default is 0xAD
#define DEVID_MST       0x01         // Default is 0x1D
#define PARTID          0x02         // Default is 0xF2
#define REVID           0x03         // Default is 0x01
#define XDATA           0x08
#define YDATA           0x09
#define ZDATA           0x0A
#define STATUS          0x0B
#define FIFO_EN_L       0x0C
#define FIFO_EN_H       0x0D
#define XDATA_L         0x0E
#define XDATA_H         0x0F
#define YDATA_L         0x10
#define YDATA_H         0x11
#define ZDATA_L         0x12
#define ZDATA_H         0x13
#define TEMP_L          0x14
#define TEMP_H          0x15
#define SOFT_RST        0x1F
#define THR_ACT_L       0x20
#define THR_ACT_H       0x21
#define TIME_ACT        0x22
#define THR_INACT_L     0x23
#define THR_INACT_H     0x24
#define TIME_INACT_L    0x25
#define TIME_INACT_H    0x26
#define ACT_INACT_CTL   0x27
#define FIFO_CONTROL    0x28
#define FIFO_SAMPLES    0x29
#define INTMAP1         0x2A
#define INTMAP2         0x2B
#define FILTER_CTL      0x2C
#define POWER_CTL       0x2D
#define SELF_TEST       0x2E


// ADXL phase and polarity
#define ADXL_PHA0
#define ADXL_POL0
#define ADXL_WIRE4

// ADC freq, output data rate
#define ODR_12_5HZ      0
#define ODR_25HZ        1
#define ODR_50HZ        2
#define ODR_100HZ       3
#define ODR_200HZ       4
#define ODR_400HZ       7

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain
 * is down. The Uart and SPI clocks are set.
 * @return void
 ****************************************************************************************
 */
void adxl_init(uint8_t odr);

/**
 ****************************************************************************************
 * @brief Read one byte value of acceleration data.
 * @param channel_address 
 * @return void
 ****************************************************************************************
 */
uint8_t read_accel(uint8_t channel_address);

/**
 ****************************************************************************************
 * @brief Read DEVID_AD.
 * @return DEVID_AD, default is 0xAD
 ****************************************************************************************
 */
uint8_t read_DEVID_AD(void);

/**
 ****************************************************************************************
 * @brief Read DEVID_MST.
 * @return DEVID_MST, default is 0x1D
 ****************************************************************************************
 */
uint8_t read_DEVID_MST(void);

/**
 ****************************************************************************************
 * @brief Read PARTID.
 * @return PARTID, default is 0xF2
 ****************************************************************************************
 */
uint8_t read_PARTID(void);

/**
 ****************************************************************************************
 * @brief Read REVID.
 * @return REVID, default is 0x01
 ****************************************************************************************
 */
uint8_t read_REVID(void);

/**
 ****************************************************************************************
 * @brief Read STATUS.
 * @return STATUS
 ****************************************************************************************
 */
uint8_t read_STATUS(void);

/**
 ****************************************************************************************
 * @brief Read register.
 * @param register_address
 * @return Register value
 ****************************************************************************************
 */
uint8_t read_register(uint8_t register_address);

/**
 ****************************************************************************************
 * @brief Write register.
 * @param register_address
 *        value
 * @return void
 ****************************************************************************************
 */
void write_register(uint8_t register_address, uint8_t value);

/**
 ****************************************************************************************
 * @brief Reset all registers value.
 * @param void
 * @return void
 ****************************************************************************************
 */
void reset(void);
#endif // _SPI_ADXL_H_
