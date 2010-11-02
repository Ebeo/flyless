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

/**
 * @addtogroup  Driver
 * @{
 * @addtogroup  UART
 * @{
 * @file uart.h
 * @author Michael Bubestinger
 *
 * Headerfile for UART Interface
 */

#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_

#include "stm32f10x.h"

void UART_Protocol_Init();
void UART_Puts(uint8_t* c);
void UART_Send(uint8_t* c, uint8_t len);

#endif /* UART_PROTOCOL_H_ */
