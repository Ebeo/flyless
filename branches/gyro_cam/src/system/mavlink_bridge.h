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

#ifndef MAVLINK_BRIDGE_H_
#define MAVLINK_BRIDGE_H_


#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

#include "global_data.h"
#include "mavlink_types.h"



static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
	UART_SendChar(ch);
}

#endif /* MAVLINK_BRIDGE_H_ */
