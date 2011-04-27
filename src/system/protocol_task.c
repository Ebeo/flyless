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

#include "global_data.h"

#include "uart.h"
#include "adxl345.h"

#include "mav_vect.h"


#include <stdio.h>






void PROTOCOL_Task( void *pvParameters )
{
	while(1)
	{
		vTaskDelay( 1 / portTICK_RATE_MS);
		put_logview();
	}
}




void put_logview()
{
	char roll[5] = "";
	char nick[5] = "";
	char yaw[5]  = "";

	char x[5] = "";
	char y[5] = "";
	char z[5] = "";

	char angle_x[5] = "";
	char angle_y[5] = "";

	char pid_output[5] = "";
	char pid_ist[5] = "";

	UART_Puts((uint8_t* )"$1;1;;");
	sprintf(x,"%0.2f;",  global_data.acc_g.x);
	UART_Puts((uint8_t*)x); //x
	sprintf(y,"%0.2f;",  global_data.acc_g.y);
	UART_Puts((uint8_t*)y); //y
	sprintf(z,"%0.2f;",  global_data.acc_g.z);
	UART_Puts((uint8_t*)z); //z
	sprintf(roll,"%0.2f;",  global_data.gyro_rad.x);
	UART_Puts((uint8_t*)roll); //roll
	sprintf(nick,"%0.2f;",  global_data.gyro_rad.y);
	UART_Puts((uint8_t*)nick); //nick
	sprintf(yaw,"%0.2f;",  global_data.gyro_rad.z);
	UART_Puts((uint8_t*)yaw); //yaw
	sprintf(angle_x,"%0.2f;",  ((global_data.attitude.x * 57.295) - 0.6)*3 );
	UART_Puts((uint8_t*)angle_x);
	sprintf(angle_y,"%0.2f;",  ((global_data.attitude.y * 57.295) - 4.58));
	UART_Puts((uint8_t*)angle_y);


	sprintf(pid_output,"%u;",  global_data.pid_output);
	UART_Puts((uint8_t*)pid_output);
	sprintf(pid_ist,"%u;",  global_data.pid_soll);
	UART_Puts((uint8_t*)pid_ist);

	UART_Puts((uint8_t* )"0\r\n");
}


