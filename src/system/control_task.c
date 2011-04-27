


#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x.h"

#include "global_data.h"

#include "pid.h"

#define K_P     0.60
#define K_I     0.40
#define K_D     0.00


struct PID_DATA pidData;

void CONTROL_Task( void *pvParameters ){

	int16_t ref, meas;
	pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);

	while(1){
		vTaskDelay( 10 / portTICK_RATE_MS);

		ref  = (int16_t) (((global_data.attitude.x * 57.295) - 0.6) * 3) + 512;
		global_data.pid_soll = ref;
		meas = global_data.pid_ist;
		global_data.pid_output = pid_Controller(ref, meas, &pidData);
		global_data.pid_ist = global_data.pid_output;
	}
}
