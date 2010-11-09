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


#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#include "led.h"
#include "led_task.h"

void LED_Task( void *pvParameters )
{
	uint8_t toggle = 0;

	while(1)
	{
		vTaskDelay(ONE_SECOND);
		if(toggle == 1)
		{

			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			toggle = 0;
		}
		else if(toggle == 0)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			toggle = 1;
		}
	}
}
