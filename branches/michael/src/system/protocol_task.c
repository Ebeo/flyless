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

#include "mavlink_bridge.h"
#include "mav_vect.h"
#include "mavlink.h"


#include <stdio.h>


#define ONE_MS			( portTICK_RATE_MS)


// Define the system type, in this case an airplane
int system_type = MAV_FIXED_WING;
uint8_t param = 0;

// Initialize the required buffers


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


	mavlink_message_t msg;
	mavlink_status_t status;
	mavlink_param_set_t set;


	mavlink_msg_heartbeat_send(0, global_data.param[PARAM_SYSTEM_TYPE], 0);
	mavlink_msg_sys_status_send(0, MAV_MODE_UNINIT, MAV_NAV_GROUNDED, MAV_STATE_STANDBY, 33, 0, 0);

	while(1)
	{
		vTaskDelay( 100 / ONE_MS);
		ADXL_Convert_to_G(&global_data.acc_raw,&global_data.acc_g);
		mavlink_msg_heartbeat_send(0, global_data.param[PARAM_SYSTEM_TYPE], 0);


		if (param < ONBOARD_PARAM_COUNT)
			{
				mavlink_msg_param_value_send(0,
						(int8_t*) global_data.param_name[param],
						global_data.param[param], ONBOARD_PARAM_COUNT, param);
				param++;
			}


		mavlink_msg_attitude_send(0, 100, global_data.attitude.x,global_data.attitude.y, global_data.attitude.z, global_data.gyro_rad.x, global_data.gyro_rad.y, global_data.gyro_rad.z);


		while(UART_CharAvailable())
		{
			uint8_t c = UART_GetChar();
			if(mavlink_parse_char(0, c, &msg, &status))
			{
				switch(msg.msgid)
				{
					case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
					break;
					case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
						param = 0;
					break;
					case MAVLINK_MSG_ID_PARAM_SET:
					//	mavlink_msg_param_set_decode(&msg, &set);
					//	global_data.param[(char*) set.param_id] = set.param_value;
					break;

				}
			}
		}



		/*
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
		sprintf(angle_x,"%0.2f;",  (global_data.attitude.x * 57.295));
		UART_Puts((uint8_t*)angle_x);
		sprintf(angle_y,"%0.2f;",  (global_data.attitude.y * 57.295));
		UART_Puts((uint8_t*)angle_y);
		UART_Puts((uint8_t* )"0\r\n");
		*/
	}
}


