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
 * @file uart.c
 * @author Michael Bubestinger
 * C-file for UART Interface
 */

#include "uart.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"



#include <stdio.h>


uint8_t		  RX_Buffer[255];
uint8_t		  RX_Insert, RX_Extract;

uint8_t UART_CharAvailable(void)
{
	return(RX_Insert != RX_Extract);
}

uint8_t UART_GetChar(void)
{
	uint8_t c = RX_Buffer[RX_Extract];
	RX_Extract = (RX_Extract + 1)%255;
	return c;
}

void UART_SendChar(uint8_t c)
{
	USART_SendData(USART1,c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void UART_Send(uint8_t* c, uint8_t len)
{
	uint8_t i = 0;

	while(i<len)
	{
		USART_SendData(USART1, (uint8_t) c[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		i++;
	}
}
void UART_Puts(uint8_t* c)
{
	uint8_t i = 0;

	while(c[i] != 0x0)
	{
		USART_SendData(USART1, (uint8_t) c[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		i++;
	}
}


void UART_Protocol_Init()
{

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 , ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	  {
		RX_Buffer[RX_Insert] = USART_ReceiveData(USART1);
		RX_Insert = (RX_Insert + 1)%255;
	  }
}

