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
const uint16_t BUTTON_DEBOUNCE_TIME_MS = 20;
const uint16_t BUTTON_HOLD_TIME_MS = 300;

void StartButtonTask(void const * argument) {

	const uint16_t buttonDebounceCalls = (BUTTON_DEBOUNCE_TIME_MS
			+ BUTTON_TASK_DELAY_MS - 1) / BUTTON_TASK_DELAY_MS;
	const uint16_t buttonHoldCalls = (BUTTON_HOLD_TIME_MS
			+ BUTTON_TASK_DELAY_MS - 1) / BUTTON_TASK_DELAY_MS;


	buttonMessage_t buttonMessage;
	BaseType_t xStatus;

	uint8_t buttonPressed =  0;
	EButtonState button_state[NUM_BUTTONS] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};
	uint16_t count[NUM_BUTTONS] = {0,0,0,0,0,0}; //todo: initialize based on number of buttons

	GPIO_TypeDef * buttonGPIOBank[NUM_BUTTONS] = 	{	BUTTON1_GPIO_Port,
														BUTTON2_GPIO_Port,
														BUTTON3_GPIO_Port,
														BUTTON4_GPIO_Port,
														BUTTON5_GPIO_Port,
														BUTTON6_GPIO_Port };

	uint16_t buttonPin[NUM_BUTTONS] = 				{	BUTTON1_Pin,
														BUTTON2_Pin,
														BUTTON3_Pin,
														BUTTON4_Pin,
														BUTTON5_Pin,
														BUTTON6_Pin };

	EButtonName buttonName[NUM_BUTTONS] =			{	BUTTON1,
														BUTTON2,
														BUTTON3,
														BUTTON4,
														BUTTON5,
														BUTTON6 };




	for (;;) {

		//check each of the buttons and send relevant events
		for (uint8_t iBtn = 0; iBtn < NUM_BUTTONS; iBtn++ ) {
			if (HAL_GPIO_ReadPin(buttonGPIOBank[iBtn], buttonPin[iBtn]) == GPIO_PIN_RESET) {
				buttonPressed = 1;
			} else {
				buttonPressed = 0;
			}

			switch (button_state[iBtn]) {
			case (BUTTON_RELEASED):
				if (buttonPressed) {
					button_state[iBtn] = BUTTON_DEBOUNCE;
					count[iBtn] = 0;
				}
				break;
			case (BUTTON_DEBOUNCE):
				if (!buttonPressed) {
					button_state[iBtn] = BUTTON_RELEASED;
					if (count[iBtn] > buttonDebounceCalls) {

						buttonMessage.buttonName = buttonName[iBtn];
						buttonMessage.buttonEvent = BUTTON_PRESS;

						xStatus = xQueueSendToBack(xButtonQueue, &buttonMessage,
									10);
						if (xStatus != pdPASS) {
							//todo: add an assert or wait
						}
					}
				} else {
					count[iBtn]++;
					if (count[iBtn] > buttonHoldCalls) {
						button_state[iBtn] = BUTTON_HELD;
						buttonMessage.buttonName = buttonName[iBtn];
						buttonMessage.buttonEvent = BUTTON_HOLD;

						xStatus = xQueueSendToBack(xButtonQueue, &buttonMessage,
								10);
						if (xStatus != pdPASS) {
							//todo: add an assert or wait
						}
					}
				}
				break;
			case (BUTTON_HELD):
				if (!buttonPressed) {
					button_state[iBtn] = BUTTON_RELEASED;
					buttonMessage.buttonName = buttonName[iBtn];
					buttonMessage.buttonEvent = BUTTON_RELEASE;

					xStatus = xQueueSendToBack(xButtonQueue, &buttonMessage, 10);
					if (xStatus != pdPASS) {
						//todo: add an assert or wait
					}
				}
				break;

			}
		}
		osDelay(BUTTON_TASK_DELAY_MS);

	}

}
