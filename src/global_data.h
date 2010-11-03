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

#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_

#include "stm32f10x.h"

#include "mav_vect.h"
#include "mavlink_types.h"

#include <string.h>



#define ONBOARD_PARAM_NAME_LENGTH 15
mavlink_system_t mavlink_system;


enum
{
	PARAM_SYSTEM_ID = 0,
	PARAM_COMPONENT_ID,
	PARAM_SYSTEM_TYPE,
	PARAM_SW_VERSION,
	PARAM_UART_BAUD,

	PARAM_GYRO_OFF_X,
	PARAM_GYRO_OFF_Y,
	PARAM_GYRO_OFF_Z,

	ONBOARD_PARAM_COUNT
///< Store parameters in EEPROM and expose them over MAVLink paramter interface
} global_param_id;




struct global_struct
{
	/**********************
	 * Global Parameters for MAVLINK
	 **********************/
	float param[ONBOARD_PARAM_COUNT];
	char param_name[ONBOARD_PARAM_COUNT][ONBOARD_PARAM_NAME_LENGTH];

	/**********************
	 * System State
	 **********************/

	float_vect3 attitude;		/* Attitude euler Angles */
	float		height;			/* Estimated Height above system start */


	/***********************
	 * Sensor Values
	 **********************/

	int16_vect3 acc_off;		/* Offset value */
	int16_vect3 acc_raw; 		/* Accelerometer RAW */
	float_vect3 acc_g;			/* Accelerometer in g */

	int16_vect3 gyro_off;		/* Offset value */
	int16_vect3 gyro_raw;   	/* Gyro rates RAW */
	float_vect3 gyro_rad;       /* Gyro rates in rad/s */
	uint16_t 	gyro_temp_raw;  /* Temperature of Gyro-Sensor RAW */
	float		gyro_temp;		/* Temperature of Gyro-Sensor in °C */

	uint32_t    pressure_raw;	/* Pressure RAW */
	uint32_t	pressure_pasc;	/* Pressure in Pascal */
	uint32_t	pressure_ground;/* Initial Value */
	uint32_t	pressure_diff;  /* pressure_init - pressure_raw */

	int16_vect3	magnet_raw;		/* Magnet raw values */

}global_data;


static inline void global_data_reset(void)
{
	global_data.acc_off.x = 0;
	global_data.acc_off.y = 0;
	global_data.acc_off.z = 0;
	global_data.acc_raw.x = 0;
	global_data.acc_raw.y = 0;
	global_data.acc_raw.z = 0;
	global_data.acc_g.x = 0;
	global_data.acc_g.y = 0;
	global_data.acc_g.z = 0;


	global_data.gyro_off.x = 0;
	global_data.gyro_off.y = 0;
	global_data.gyro_off.z = 0;
	global_data.gyro_raw.x = 0;
	global_data.gyro_raw.y = 0;
	global_data.gyro_raw.z = 0;
	global_data.gyro_rad.x = 0;
	global_data.gyro_rad.y = 0;
	global_data.gyro_rad.z = 0;
	global_data.gyro_temp_raw = 0;
	global_data.gyro_temp = 0;

	global_data.pressure_raw = 0;
	global_data.pressure_pasc = 0;
	global_data.pressure_ground = 0;
	global_data.pressure_diff = 0;

	global_data.magnet_raw.x = 0;
	global_data.magnet_raw.y = 0;
	global_data.magnet_raw.z = 0;
}

static inline void global_data_reset_param_defaults(void)
{
	global_data.param[PARAM_SYSTEM_ID] = 010;
	strcpy(global_data.param_name[PARAM_SYSTEM_ID], "SYS_ID");

	global_data.param[PARAM_COMPONENT_ID] = 200;
	strcpy(global_data.param_name[PARAM_COMPONENT_ID], "SYS_COMP_ID");

	global_data.param[PARAM_SYSTEM_TYPE] = MAV_HELICOPTER;
	strcpy(global_data.param_name[PARAM_SYSTEM_TYPE], "SYS_TYPE");

	global_data.param[PARAM_SW_VERSION] = 0000;
	strcpy(global_data.param_name[PARAM_SW_VERSION], "SYS_SW_VER");

	global_data.param[PARAM_UART_BAUD] = 115200;
	strcpy(global_data.param_name[PARAM_UART_BAUD], "UART_BAUD");

	global_data.param[PARAM_GYRO_OFF_X] = 0;
	strcpy(global_data.param_name[PARAM_GYRO_OFF_X], "GYRO_OFF_X");

	global_data.param[PARAM_GYRO_OFF_Y] = 0;
	strcpy(global_data.param_name[PARAM_GYRO_OFF_Y], "GYRO_OFF_Y");

	global_data.param[PARAM_GYRO_OFF_Z] = 0;
	strcpy(global_data.param_name[PARAM_GYRO_OFF_Z], "GYRO_OFF_Z");

	mavlink_system.sysid = global_data.param[PARAM_SYSTEM_ID];
	mavlink_system.compid = global_data.param[PARAM_COMPONENT_ID];
}
#endif /* GLOBAL_DATA_H_ */
