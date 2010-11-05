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
 * @addtogroup  ITG3200
 * @{
 * @file itg3200.h
 * @author Michael Bubestinger
 *
 * Headerfile for Invesense ITG3200
 * Datasheet: http://invensense.com/mems/gyro/documents/PS-ITG-3200-00-01.4.pdf
 */

#ifndef ITG3200_H_
#define ITG3200_H_

#include "stm32f10x.h"
#include "mav_vect.h"

#define OWNADDRESS 	0x30	// dummy address for the STM32
#define ITGADDRESS  0xD0	// address of the ITG3200
#define I2C_SPEED 	100000	// I2C Speed 400KHz


#define WHO_AM_I 			0x00
#define SMPLRT_DIV			0x15
#define DLPF_FS				0x16
#define INT_CFG				0x17
#define INT_STATUS			0x1A
#define TEMP_OUT_H			0x1B
#define TEMP_OUT_L			0x1C
#define GYRO_XOUT_H			0x1D
#define GYRO_XOUT_L			0x1E
#define GYRO_YOUT_H 		0x1F
#define GYRO_YOUT_L 		0x20
#define GYRO_ZOUT_H			0x21
#define	GYRO_ZOUT_L 		0x22
#define PWR_MGM 			0x3E

typedef enum {TX = 0, RX = 1, TXREG = 2} ITG3200_Dir;

/**
 * @brief ITG Setup
 * Initialize GPIOs and I2C for the ITG
 * Initial values for the sensor are hardcoded ! -> :(
 * @todo implement failure check
 */
void ITG_I2C_Setup(void);

/**
 * @brief Get gyro rate
 * @param *rate pointer to the buffer for the gyro values
 * Stores the gyro rate raw values
 * rate[0] = roll
 * rate[1] = nick
 * rate[2] = yaw
 */
void ITG_GetRate(int16_vect3* rate);

void ITG_GetRAD(int16_vect3 rate_raw, float_vect3* rate_rad);
void ITG_RefOffset(float_vect3* offset);

/**
 * @brief Read register
 * @param register to read
 * @return byte read from register
 * return a byte from the register
 */
uint8_t ITG_Read(uint8_t reg);
/**
 * @brief write a byte to the a register
 * @param reg register adress
 * @param data byte to write
 * Writes a single byte to a specified register
 */
void ITG_Write(uint8_t reg, uint8_t data);
/**
 * @brief dummy function
 * checks if the I2C interface is blocked
 */
uint8_t ITG_IsBlocked(void);


#endif /* ITG3200_H_ */
/**
 * @}
 * @}
 */
