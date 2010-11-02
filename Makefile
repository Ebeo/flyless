
# Target file name (without extension).
TARGET = flyless


CHIP = STM32F10x_512k_64k
# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = 0
CDEFS += -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER

LINKERSCRIPT = ./firmware/STM32F10x_512k_64k_flash.ld

FIRMLIBDIR   = ./firmware
STMLIBDIR    = $(FIRMLIBDIR)/STM32F10x_StdPeriph_Lib_V3.3.0/Libraries
STMSPDDIR    = $(STMLIBDIR)/STM32F10x_StdPeriph_Driver
STMSPDSRCDIR = $(STMSPDDIR)/src
STMSPDINCDIR = $(STMSPDDIR)/inc
CMSISCOREDIR = $(STMLIBDIR)/CMSIS/CM3/CoreSupport
CMSISDEVDIR  = $(STMLIBDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x


RTOS 	 = $(FIRMLIBDIR)/FreeRTOSV6.1.0/Source
RTOSPORT = $(RTOS)/portable/GCC/ARM_CM3
RTOSMEM  = $(RTOS)/portable/MemMang
RTOSINC  = $(RTOS)/include


PIXHAWK 	= ./src/pixhawk
MAVLINK 	= ./src/mavlink
MAVLINKCOM 	= ./src/mavlink/common/
HAL 		= ./src/hal
SYSTEM  	= ./src/system

## Includes for FreeRTOS, Periph and CMSIS
CUSTOMINCDIR = $(STMSPDINCDIR) $(CMSISDEVDIR) $(CMSISCOREDIR) $(RTOS) $(RTOSINC) $(RTOSPORT) $(RTOSMEM) 

## Custom Includes
CUSTOMINCDIR += ./src $(PIXHAWK) $(HAL) $(SYSTEM) $(MAVLINK) $(MAVLINKCOM)

## optional Libraries (*.a) 
# CUSTOMLIBDIR = 

## Main Files
SRC   = ./src/main.c
SRC += ./src/stm32f10x_it.c
SRC += ./src/syscalls.c

## HAL Files
SRC += ./src/hal/adxl345.c
SRC += ./src/hal/itg3200.c
SRC += ./src/hal/led.c
SRC += ./src/hal/uart.c

## Files form PIXHAWK Project
SRC += ./src/pixhawk/attitude_observer.c

## SYSTEM Files
SRC += ./src/system/kalman_task.c
SRC += ./src/system/protocol_task.c
SRC += ./src/system/led_task.c

## USED PERIPH Files
SRC += $(STMSPDSRCDIR)/stm32f10x_gpio.c
SRC += $(STMSPDSRCDIR)/stm32f10x_usart.c
SRC += $(STMSPDSRCDIR)/stm32f10x_spi.c
SRC += $(STMSPDSRCDIR)/stm32f10x_i2c.c
SRC += $(STMSPDSRCDIR)/stm32f10x_rcc.c
SRC += $(STMSPDSRCDIR)/stm32f10x_flash.c
SRC += $(STMSPDSRCDIR)/misc.c

## FreeRTOS
SRC += $(RTOS)/croutine.c
SRC += $(RTOS)/list.c
SRC += $(RTOS)/queue.c
SRC += $(RTOS)/tasks.c
SRC += $(RTOSPORT)/port.c
SRC += $(RTOSMEM)/heap_2.c


include makefile.mk