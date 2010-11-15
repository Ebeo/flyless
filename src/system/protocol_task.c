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



int system_type = MAV_HELICOPTER;
uint8_t param = 0;

void handle_mav_link_mess(void);


void PROTOCOL_Task( void *pvParameters )
{

	mavlink_msg_heartbeat_send(0, global_data.param[PARAM_SYSTEM_TYPE], 0);
	mavlink_msg_boot_send(0, global_data.param[PARAM_SW_VERSION]);
	//mavlink_msg_statustext_send(0, 1, "FLYless");

	while(1)
	{
		vTaskDelay( 100 / ONE_MS);
		ADXL_Convert_to_G(&global_data.acc_raw,&global_data.acc_g);
		mavlink_msg_heartbeat_send(0, global_data.param[PARAM_SYSTEM_TYPE], 0);
		mavlink_msg_sys_status_send(0, global_data.mode, global_data.nav, global_data.state, 3000, 0, 0);
		mavlink_msg_attitude_send(0, 100, global_data.attitude.x,global_data.attitude.y, global_data.attitude.z, global_data.gyro_rad.x, global_data.gyro_rad.y, global_data.gyro_rad.z);
		mavlink_msg_raw_imu_send(0, 100, global_data.acc_g.x, global_data.acc_g.y, global_data.acc_g.z, global_data.gyro_raw.x, global_data.gyro_raw.y, global_data.gyro_raw.z, global_data.magnet_raw.x, global_data.magnet_raw.y, global_data.magnet_raw.z);

		handle_mav_link_mess();



		/* should not be here */

		int8_t x1,x2,x3,x4;

		x1 = (int8_t)global_data.param[8];
		x2 = (int8_t)global_data.param[9];
		x3 = (int8_t)global_data.param[10];
		x4 = (int8_t)global_data.param[11];

		SERVO_SetValue(x1,x2,x3,x4);


	}
}

void handle_mav_link_mess(void)
{

	int i,j;


	mavlink_message_t msg;
	mavlink_status_t status;
	mavlink_param_set_t set;
	char* key;

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
				{
					mavlink_msg_param_set_decode(&msg, &set);
					key = (char*) set.param_id;
					for (i = 0; i < ONBOARD_PARAM_COUNT; i++)
						{
							uint8_t match = 1;
							for (j = 0; j < ONBOARD_PARAM_NAME_LENGTH; j++)
							{
								// Compare
								if (((char) (global_data.param_name[i][j]))	!= (char) (key[j]))
								{
									match = 0;
								}

								// End matching if null termination is reached
								if (((char) global_data.param_name[i][j]) == '\0')
								{
									break;
								}
							}

							// Check if matched
							if (match)
							{
								// Only write and emit changes if there is actually a difference
								// AND only write if new value is NOT "not-a-number"
								// AND is NOT infy
								if (global_data.param[i] != set.param_value)
								{
									global_data.param[i] = set.param_value;
									// Report back new value
									mavlink_msg_param_value_send(MAVLINK_COMM_0,
											(int8_t*) global_data.param_name[i],
											global_data.param[i], ONBOARD_PARAM_COUNT, param);
								}
							}
						}
				}
				break;

			}
		}
	}

	if (param < ONBOARD_PARAM_COUNT)
		{
			mavlink_msg_param_value_send(0,
					(int8_t*) global_data.param_name[param],
					global_data.param[param], ONBOARD_PARAM_COUNT, param);
			param++;
		}
}


void put_logview()
{
	char roll[10] = "";
	char nick[10] = "";
	char yaw[10]  = "";

	char x[10] = "";
	char y[10] = "";
	char z[10] = "";

	char angle_x[10] = "";
	char angle_y[10] = "";

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
}


