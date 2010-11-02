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
#include "stm32f10x_rcc.h"

#include "itg3200.h"
#include "adxl345.h"


#include "attitude_observer.h"
#include "mav_vect.h"


#define DELAY			( 5 / portTICK_RATE_MS)

float_vect3 		gyros;
int16_vect3		    acc;
float_vect3 		acc_g;
float_vect3 		angles;

void KALMAN_Task( void *pvParameters )
{

	GPIO_InitTypeDef 	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

    float_vect3 init_state_accel;
  	float_vect3 init_state_mag;

  	init_state_accel.x = 0;
  	init_state_accel.y = 0;
  	init_state_accel.z = 0;

  	init_state_mag.x = 1;
  	init_state_mag.y = 1;
  	init_state_mag.z = 1;

  	attitude_observer_init(init_state_accel, init_state_mag);

	while(1)
	{
		vTaskDelay(DELAY);

		GPIO_ResetBits(GPIOA,GPIO_Pin_0);

		ITG_GetRate(&gyros);
		attitude_observer_predict(gyros);

		ADXL_GetACC(&acc);


		attitude_observer_correct_accel(acc);
		attitude_observer_get_angles(&angles);

		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	}
}
