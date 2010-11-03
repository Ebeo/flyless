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




/* Includes */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "global_data.h"
#include "mavlink_types.h"

#include "led.h"
#include "uart.h"
#include "adxl345.h"
#include "itg3200.h"


#include "protocol_task.h"
#include "kalman_task.h"
#include "led_task.h"


#define tskKALMAN_PRIORITY 3
#define tskPROTOCOL_PRIORITY 1
#define tskLED_PRIORITY	1



void SetClock();

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	SystemInit();

	mavlink_system_t mavlink_system;

	/* Hardware Init */

	/* Init clock system to 64MHz from internal RC-oscillator */
	SetClock();

	/* Init LEDs and setup Blinktime to 1/2 second */
	LED_Init();

	/* reset global data values */
	global_data_reset();
	global_data_reset_param_defaults();


	/* UART Init */
	UART_Protocol_Init();
	UART_Puts((uint8_t* )"UUART: OK\r\n");

	/* ADXL345 Init */
	if(ADXL_SPI_Setup() != SUCCESS)
	{
		UART_Puts((uint8_t* )"ADXL: FAILED\r\n");
	}else
	{
		UART_Puts((uint8_t* )"ADXL: OK\r\n");
	}

	/* ITG 3200 Init */
	/* no Fail-Check implemented */
	ITG_I2C_Setup();
	ITG_RefOffset(&global_data.gyro_off);

	UART_Puts((uint8_t* )"ITG 3200: OK\r\n");


	/* Create Tasks and start the scheduler */
	UART_Puts((uint8_t* )"Now starting the scheduler!\r\n");

	xTaskCreate( KALMAN_Task, 	( signed char * ) "KALMAN"	, configMINIMAL_STACK_SIZE * 2	 	,( void * ) NULL, tskKALMAN_PRIORITY 	, NULL );
	xTaskCreate( PROTOCOL_Task, ( signed char * ) "PROT"	, configMINIMAL_STACK_SIZE * 3		,( void * ) NULL, tskPROTOCOL_PRIORITY 	, NULL );
	xTaskCreate( LED_Task, 	  	( signed char * ) "LED"   	, configMINIMAL_STACK_SIZE 			,( void * ) NULL, tskLED_PRIORITY    	, NULL );

	vTaskStartScheduler();
	UART_Puts((uint8_t* )"\r\nFailed! U have a problem!");
	while(1);

}


void SetClock()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_DeInit();

	FLASH_SetLatency(FLASH_Latency_2);

	RCC_HCLKConfig(RCC_SYSCLK_Div1);	/* 64MHz */
	RCC_PCLK1Config(RCC_HCLK_Div2);		/* 32MHz */
	RCC_PCLK2Config(RCC_HCLK_Div1);		/* 64MHz */


	RCC_HSICmd(ENABLE);
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16); /* Sysclock 64MHz */
	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	while (RCC_GetSYSCLKSource() != 0x08)
	{
	}

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);
	RCC_MCOConfig(RCC_MCO_SYSCLK);
}



void vApplicationStackOverflowHook( void )
{
/* This will be called if a task overflows its stack.  pxCurrentTCB
can be inspected to see which is the offending task. */
  for( ;; );
}
