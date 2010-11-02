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

#include "uart.h"
#include "adxl345.h"

#include "attitude_observer.h"
#include "mav_vect.h"

#include <stdio.h>


#define ONE_MS			( portTICK_RATE_MS)




extern float_vect3 			gyros;
extern int16_vect3			acc;
extern float_vect3 			acc_g;
extern float_vect3 			angles;

void PROTOCOL_Task( void *pvParameters )
{
	char roll[10] = "";
	char nick[10] = "";
	char yaw[10]  = "";

	char x[10] = "";
	char y[10] = "";
	char z[10] = "";

	char angle_x[10] = "";
	char angle_y[10] = "";

	while(1)
	{
		vTaskDelay( 100 / ONE_MS);
	//	attitude_observer_get_angles(&angles);
		ADXL_Convert_to_G(&acc, &acc_g);
		UART_Puts((uint8_t* )"$1;1;;");
		sprintf(x,"%0.2f;",  acc_g.x);
		UART_Puts((uint8_t*)x); //x
		sprintf(y,"%0.2f;",  acc_g.y);
		UART_Puts((uint8_t*)y); //y
		sprintf(z,"%0.2f;",  acc_g.z);
		UART_Puts((uint8_t*)z); //z
		sprintf(roll,"%0.2f;",  gyros.x);
		UART_Puts((uint8_t*)roll); //roll
		sprintf(nick,"%0.2f;",  gyros.y);
		UART_Puts((uint8_t*)nick); //nick
		sprintf(yaw,"%0.2f;",  gyros.z);
		UART_Puts((uint8_t*)yaw); //yaw
		sprintf(angle_x,"%0.2f;",  (angles.x * 57.295));
		UART_Puts((uint8_t*)angle_x);
		sprintf(angle_y,"%0.2f;",  (angles.y * 57.295));
		UART_Puts((uint8_t*)angle_y);
		UART_Puts((uint8_t* )"0\r\n");
	}
}
