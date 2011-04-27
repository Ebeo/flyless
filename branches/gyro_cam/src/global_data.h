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


#include <string.h>


struct global_struct
{

	/**********************
	 * System State
	 **********************/

	uint8_t 	mode;
	uint8_t		nav;
	uint8_t		state;

	float_vect3	attitude;		/* Attitude euler Angles */
	float		height;			/* Estimated Height above system start */


	/***********************
	 * Sensor Values
	 **********************/

	int16_vect3 acc_off;		/* Offset value */
	int16_vect3 acc_raw; 		/* Accelerometer RAW */
	float_vect3 acc_g;			/* Accelerometer in g */

	int16_vect3 gyro_raw;   	/* Gyro rates RAW */
	float_vect3 gyro_rad;       /* Gyro rates in rad/s */
	uint16_t 	gyro_temp_raw;  /* Temperature of Gyro-Sensor RAW */
	float		gyro_temp;		/* Temperature of Gyro-Sensor in °C */

	uint32_t    pressure_raw;	/* Pressure RAW */
	uint32_t	pressure_pasc;	/* Pressure in Pascal */
	uint32_t	pressure_ground;/* Initial Value */
	uint32_t	pressure_diff;  /* pressure_init - pressure_raw */

	int16_vect3	magnet_raw;		/* Magnet raw values */

	uint32_t 	pid_output;		/* Stellwert für Servo */
	uint16_t	pid_ist;
	uint16_t	pid_soll;

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

	global_data.pid_output = 0;
	global_data.pid_ist = 0;
	global_data.pid_soll = 100;
}

#endif /* GLOBAL_DATA_H_ */
