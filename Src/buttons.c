/*
 * buttons.c
 *
 *  Created on: Jun 10, 2018
 *      Author: esoren
 */

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "display.h"
#include "globals.h"
#include "buttons.h"

//const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
const uint16_t BUTTON_TASK_DELAY_MS = 20;
const uint16_t BUTTON_DEBOUNCE_TIME_MS = 50;
const uint16_t BUTTON_HOLD_TIME_MS = 300;



void StartButtonTask(void const * argument) {
	displayMessage_t displayMessage;

	const uint16_t buttonDebounceCalls = (BUTTON_DEBOUNCE_TIME_MS+BUTTON_TASK_DELAY_MS-1)/BUTTON_TASK_DELAY_MS;
	const uint16_t buttonHoldCalls = (BUTTON_DEBOUNCE_TIME_MS+BUTTON_TASK_DELAY_MS-1)/BUTTON_TASK_DELAY_MS;

	uint8_t buttonPressed = 0;

	BaseType_t xStatus;

	for(;;) {


		//buttonPressed = HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);



	    osDelay(BUTTON_TASK_DELAY_MS);

	}

}
