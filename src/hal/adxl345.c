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
 * @file adxl345.c
 * @author Michael Bubestinger
 * C-file for Analog-Device ADXL345
 * Datasheet: http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
 */

#include "adxl345.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "mav_vect.h"

#include <math.h>

/**
 * @brief macro for selecting the ADXL345
 */
#define ADXL_CS_LOW		GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
/**
 * @brief macro for releasing the ADXL345
 */
#define ADXL_CS_HIGH	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);


ErrorStatus ADXL_SPI_Setup(void)
{

	/*
	 * Enable CLOCKs and initialize the GPIO and SPI structures for our interface
	 */
	SPI_InitTypeDef		SPI_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;


	/*
	 * Chipselect
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/*
  	 * MISO
  	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/*
  	 * MOSI and Clock
  	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/*
  	 * Interrupt lines
  	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

  	/*
  	 * SPI Config
  	 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
  	SPI_Init(SPI1, &SPI_InitStructure);

  	/*
  	 * start the interface
  	 */
   	SPI_Cmd(SPI1,ENABLE);

   	/*
   	 * check if the DEVID is matching 0xE5 (taken from the datasheet)
   	 * if not stop everything and return an error!
   	 */
   	if( ADXL_ReadByte(DEVID) != 0xE5) return ERROR;

   	/*
   	 * power up the sensor and switch to the right dataformat
   	 */
	ADXL_Write(POWER_CTL,0x08);
	ADXL_Write(DATA_FORMAT,0x0B);

	/*
	 * add offset values to the output of the ADXL345...(all handled by the device itself
	 */
	ADXL_Write(OFSX,X_OFF);
	ADXL_Write(OFSY,Y_OFF);
	ADXL_Write(OFSZ,Z_OFF);

	/*
	 * everything went fine
	 */
	return SUCCESS;
}

void ADXL_Write(uint8_t reg, uint8_t byte)
{
	/*
	 * select the ADXL345
	 */
	ADXL_CS_LOW;
	/*
	 * write the register address
	 */
	SPI_I2S_SendData(SPI1, ADXL_W | reg);
	/*
	 * clr receive buffer
	 */
	ADXL_Clr_Rcv_Buf();
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/*
	 * send the data
	 */
	SPI_I2S_SendData(SPI1, byte);
	/*
	 * clr receive buffer
	 */
	ADXL_Clr_Rcv_Buf();
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	/*
	 * deselect the ADXL345
	 */
	ADXL_CS_HIGH;
}

uint8_t ADXL_ReadByte(uint8_t reg)
{
	uint8_t data;

	/*
	 * select the ADXL345
	 */
	ADXL_CS_LOW;
	/*
	 * write the register address with the READ bit
	 */
	SPI_I2S_SendData(SPI1, ADXL_R | reg);
	/*
	 * clr receive buffer
	 */
	ADXL_Clr_Rcv_Buf();
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/*
	 * send dummy byte for the clocks!
	 */
	SPI_I2S_SendData(SPI1, 0xFF);
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	/*
	 * read the received data from the register
	 */
	data = SPI_I2S_ReceiveData(SPI1);
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	/*
	 * deselect the ADXL345
	 */
	ADXL_CS_HIGH;

	return data;
}


void ADXL_ReadSeq(uint8_t reg,uint8_t *data,uint8_t count)
{
	uint8_t i = 0;

	/*
	 * select the ADXL345
	 */
	ADXL_CS_LOW;

	/*
	 * send register address including Read-Bit and Multible Bytes-Bit
	 */
	SPI_I2S_SendData(SPI1, ADXL_R | ADXL_MB | reg);
	/*
	 * clr receive buffer
	 */
	ADXL_Clr_Rcv_Buf();
	/*
	 * wait for finish
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*
	 * read all data
	 */
	while(i < count)
	{
		SPI_I2S_SendData(SPI1, 0xFF);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*data = SPI_I2S_ReceiveData(SPI1);
		data++;
		i++;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	/*
	 * deselect the ADXL345
	 */
	ADXL_CS_HIGH;
}

void ADXL_Clr_Rcv_Buf()
{
	/*
	 * Clears the receive buffer or we would get overrun errors
	 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);
}

void ADXL_GetACC(int16_vect3* acc)
{

	uint8_t raw_data[6];

	/*
	 * get 6 bytes! X,Y,Z
	 */
	ADXL_ReadSeq(DATAX0,raw_data,6);

	/*
	 * add LOW-Byte to HIGH-Byte
	 */
	acc->x = (raw_data[1]<<8) | (raw_data[0]);
	acc->y = (raw_data[3]<<8) | (raw_data[2]);
	acc->z = (raw_data[5]<<8) | (raw_data[4]);

}

void ADXL_Convert_to_G(int16_vect3* acc, float_vect3* acc_g)
{
	/*
	 * convert raw values to real units
	 * 0.0039 is taken from the datasheet
	 */
	acc_g->x = acc->x * 0.0039;
	acc_g->y = acc->y * 0.0039;
	acc_g->z = acc->z * 0.0039;

}
void ADXL_CalAngle(float* acc_g, float* angel)
{
	angel[0] = atan(  acc_g[0] / sqrt(acc_g[1]*acc_g[1]+acc_g[2]*acc_g[2])) * 57.2958;
	angel[1] = atan(  acc_g[1] / sqrt(acc_g[0]*acc_g[0]+acc_g[2]*acc_g[2])) * 57.2958;
	angel[2] = atan(  sqrt(acc_g[0]*acc_g[0] + acc_g[1]*acc_g[1]) / acc_g[3]) * 57.2958;
}


