/*
	FLYLess
	flyless.wikidot.com
	contact: <m.bubestinger@gmail.com>


  	This file is part of FLYless.

    FLYLess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FLYLess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLYLess.  If not, see <http://www.gnu.org/licenses/>.
*/



/**
 * @addtogroup  Driver
 * @{
 * @addtogroup 	ADXL345
 * @{
 * @file adxl345.h
 * @author Michael Bubestinger
 *
 * Headerfile for Analog-Device ADXL345
 * Datasheet: http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include "stm32f10x.h"
#include "mav_vect.h"


#define DEVID			0x00
#define THRESH_TAP		0x1D
#define OFSX			0x1E
#define OFSY			0x1F
#define OFSZ			0x20
#define DUR				0x21
#define LATENT			0x22
#define WINDOW			0x23
#define THRESH_ACT		0x24
#define THRESH_INACT	0x25
#define TIME_INACT		0x26
#define ACT_INACT_CTL	0x27
#define THRESH_FF		0x28
#define TIME_FF			0x29
#define TAP_AXES		0x2A
#define ACT_TAP_STATUS	0x2B
#define BW_RATE			0x2C
#define POWER_CTL		0x2D
#define INT_ENABLE		0x2E
#define INT_MAP			0x2F
#define INT_SOURCE		0x30
#define DATA_FORMAT 	0x31
#define DATAX0			0x32
#define DATAX1			0x33
#define DATAY0			0x34
#define DATAY1			0x35
#define DATAZ0			0x36
#define DATAZ1			0x37
#define FIFO_CTL		0x38
#define FIFO_STATUS		0x39


#define ADXL_MB			0x40	// multiple bytes
#define ADXL_R			0x80	// read byte
#define ADXL_W			0x00	// write byte (dummy for better readability)


#define X_OFF			0xFF // Offset-definitions for every single axis in 2s-complement format!
#define Y_OFF			0xFA // Offset-definitions for every single axis in 2s-complement format!
#define Z_OFF			0x06 // Offset-definitions for every single axis in 2s-complement format!

/**
 * @brief ADXL Setup
 * Initialize GPIOs and SPI for SPI1 inclusive GPIOC_14 and GPIO_15 for interrupt handling
 * Initial values for the sennsor are hardcoded ! -> :(
 * @return ERROR = 0, SUCCESS = !ERROR... this check is based on reading the DEVID.
 * @todo implementing interrupt handler
 */
ErrorStatus ADXL_SPI_Setup(void);

/**
 * @brief Write a single byte on the SPI interface
 * @param reg the register to write
 * @param byte the byte to write
 * Writes a single byte on the SPI interface to the ADXL.
 * CS and interrupt handling are all done by this function!
 */
void 		ADXL_Write(uint8_t reg, uint8_t byte);

/**
 * @brief Reads a single byte from the SPI interface
 * @param reg the register to read
 * @return byte the value from the register
 * Reads a single byte from the ADXL.
 * CS and interrupt handling are all done by this function!
 */
uint8_t 	ADXL_ReadByte(uint8_t reg);

/**
 * @brief Reads a sequence of bytes from the SPI interface
 * @param reg the register to read
 * @param *data pointer to the buffer
 * @param count number of bytes to read
 * Reads a complete byte sequence of the length [count] from the ADXL
 * the bytes are stored at the adress of [data]
 */
void 		ADXL_ReadSeq(uint8_t reg,uint8_t *data,uint8_t count);
/**
 * @brief returns the accelerometer values
 * @param *acc pointer to the buffer for the acc values
 * Reads all 3 axis and stores them at the adress of [acc]
 * acc[0] = x
 * acc[1] = y
 * acc[2] = z
 */
void 		ADXL_GetACC(int16_vect3* acc);
/**
 * @brief Calculates the angel based on the accelerometer values
 * @param *acc_g pointer to the acc values converted in "G-forces"
 * @param *angel pointer to the buffer for the calculated angels
 * This function calculates the angle based on a few application notes on the internet
 * it make use of <math.h> for atan...
 * @todo implement faster and smaller atan() function
 */
void 		ADXL_CalAngle(float* acc_g, float* angel);
/**
 * @brief Converts the raw values to G forces
 * @param *acc pointer to the buffer which holds the accelerometer values
 * @param @acc_g pointer to the buffer for the converted values
 * This function brings the raw values of the sensor to real world units
 */
void 		ADXL_Convert_to_G(int16_vect3* acc, float_vect3* acc_g);

/**
 * @brief dummy function for clearing the receive buffer of the SPI interface
 */
void 		ADXL_Clr_Rcv_Buf(void);

#endif /* ADXL345_H_ */
/**
 * @}
 * @}
 */
