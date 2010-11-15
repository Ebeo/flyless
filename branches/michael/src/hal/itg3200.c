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
 * @file itg3200.c
 * @author Michael Bubestinger
 *
 * C-File for Invesense ITG3200
 * Datasheet: http://invensense.com/mems/gyro/documents/PS-ITG-3200-00-01.4.pdf
 */



#include "FreeRTOS.h"
#include "task.h"

#include "itg3200.h"

#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "uart.h"

#include "mav_vect.h"

#include <stdio.h>



volatile ITG3200_Dir ITG3200_DIR;
volatile uint8_t ITG3200_REGISTER = 0;
volatile uint8_t ITG3200_TX_DATA = 0;
volatile uint8_t ITG3200_RX_DATA = 0;
volatile uint8_t ITG3200_BLOCKED = 0;

float_vect3* gyro_offset;

uint8_t TX_ID = 0;
uint8_t RX_ID = 0;

void I2C1_EV_IRQHandler()
{
	/*
	 * state machine for the whole I2C events
	 */
	switch (I2C_GetLastEvent(I2C1))
	{

		/*
		 * Master Mode
		 */
		case I2C_EVENT_MASTER_MODE_SELECT:
			/*
			 * we want to transmit a single byte
			 */
			if(ITG3200_DIR == TX)
			{
				I2C_Send7bitAddress(I2C1, ITGADDRESS, I2C_Direction_Transmitter);
			/*
			 * we want to read a register value
			 * first step is TRANSMITTING the register address then RECEIVE the data
 			 */
			}else if(ITG3200_DIR == TXREG)
			{
				I2C_Send7bitAddress(I2C1, ITGADDRESS, I2C_Direction_Transmitter);
			/*
			 * we want to receive something
			 */
			}else if(ITG3200_DIR == RX)
			{
				I2C_Send7bitAddress(I2C1, ITGADDRESS, I2C_Direction_Receiver);
			}
		break;

			/*
			 * we get here after transmitting address + write bit
			 */
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
			/*
			 * Send Register adress
			 */
			I2C_SendData(I2C1, ITG3200_REGISTER);
		break;

			/*
			 * First byte transmitted
			 */
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
			/*
			 * Send the Data to write and generate STOP event
			 */
			if(ITG3200_DIR == TX)
			{
				I2C_SendData(I2C1, ITG3200_TX_DATA);
				I2C_GenerateSTOP(I2C1,ENABLE);
				ITG3200_BLOCKED = 0;
			/*
			 * Generate second start and switch to receiver mode
			 */
			}else if(ITG3200_DIR == TXREG)
			{
				ITG3200_DIR = RX;
				I2C_GenerateSTART(I2C1,ENABLE);
			}
		break;

			/*
			 * not implemented
			 */
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
		break;

		    /*
			 * not implemented
			 */
		case I2C_EVENT_MASTER_MODE_ADDRESS10:
		break;

			/*
			 * we received a byte...
			 */
		case I2C_EVENT_MASTER_BYTE_RECEIVED:
			ITG3200_RX_DATA = I2C_ReceiveData(I2C1);
			ITG3200_BLOCKED = 0;
		break;
			/*
			 * we switched the mode
			 */
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
			I2C_GenerateSTOP(I2C1,ENABLE);
		break;

	}
}

/*
 * dummey irq handler
 */
void I2C2_ER_IRQHandler(void)
{
  if (I2C_GetITStatus(I2C1, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
}

void ITG_Write(uint8_t reg, uint8_t data)
{
	/*
	 * check if I2C interace is in use
	 */
	if(ITG3200_BLOCKED == 0)
	{
		ITG3200_BLOCKED = 1; // block it

		/*
		 * fill the global buffer
		 */
		ITG3200_REGISTER = reg;
		ITG3200_DIR = TX;
		ITG3200_TX_DATA = data;
		/*
		 * start the transmitting
		 */
		I2C_GenerateSTART(I2C1,ENABLE);
	}
	/*
	 * wait till finished
	 */
	while(ITG3200_BLOCKED == 1);
}

uint8_t ITG_Read(uint8_t reg)
{
	/*
	 * check if I2C interface is in use
	 */
	if(ITG3200_BLOCKED == 0)
	{
		ITG3200_BLOCKED = 1;	// block it
		/*
		 * fill global buffers
		 */
		ITG3200_REGISTER = reg;
		ITG3200_DIR = TXREG;
		I2C_GenerateSTART(I2C1,ENABLE);
	}
	/*
	 * wait till finished
	 */
 	while(ITG3200_BLOCKED == 1);
 	/*
 	 * return data
 	 */
	return ITG3200_RX_DATA;
}

uint8_t ITG_IsBlocked()
{
	return ITG3200_BLOCKED;
}

void ITG_GetRate(int16_vect3* rate)
{
	int16_t data[3];

	/*
	 * read all values
	 */
	data[0]  = (ITG_Read(GYRO_XOUT_H) << 8);
	data[0] |=  ITG_Read(GYRO_XOUT_L);

	data[1]  = (ITG_Read(GYRO_YOUT_H) << 8);
	data[1] |=  ITG_Read(GYRO_YOUT_L);

	data[2]  = (ITG_Read(GYRO_ZOUT_H) << 8);
	data[2] |=  ITG_Read(GYRO_ZOUT_L);

	rate->x = data[0] + gyro_offset->x;
	rate->y = data[1] + gyro_offset->y;
	rate->z = data[2] + gyro_offset->z;

}

void ITG_GetRAD(int16_vect3 rate_raw, float_vect3* rate_rad)
{
	/*
	 * calculate values in rad/s
	 */
	rate_rad->x	= (rate_raw.x) * 0.0012139;	  // to °/s -> / 14.375 //
	rate_rad->y	= (rate_raw.y) * 0.0012139;	  // to rad/s ->  (x / 14.375) * 0.01745 //
	rate_rad->z = (rate_raw.z) * 0.0012139;	  // == x * 0.0012139 //

}


void ITG_RefOffset(float_vect3* offset)
{
	gyro_offset = offset;
}


void ITG_I2C_Setup()
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	I2C_InitTypeDef  	I2C_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	uint16_t i;



    I2C_Cmd(I2C1, ENABLE);
  	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = OWNADDRESS;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


	ITG_Write(PWR_MGM, 0x80);
	for(i = 0;i<9999;i++);
	ITG_Write(SMPLRT_DIV, 0x00);
	for(i = 0;i<9999;i++);
	ITG_Write(DLPF_FS, 0x1E);
	for(i = 0;i<9999;i++);
	ITG_Write(INT_CFG, 0x00);
	for(i = 0;i<9999;i++);
	ITG_Write(PWR_MGM, 0x00);
	for(i = 0;i<9999;i++);
}


