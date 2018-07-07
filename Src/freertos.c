/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "globals.h"
#include "display.h"
#include "buttons.h"
#include "dds.h"
#include "ham.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
osThreadId displayTaskHandle;
osThreadId buttonTaskHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	osThreadDef(displayTask, StartDisplayTask, osPriorityNormal, 0, 128);
	displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

	osThreadDef(buttonTask, StartButtonTask, osPriorityNormal, 0, 128);
	buttonTaskHandle = osThreadCreate(osThread(buttonTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
	xDisplayQueue = xQueueCreate(10, sizeof(displayMessage_t));
	xButtonQueue = xQueueCreate(30, sizeof(buttonMessage_t));

  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	uint32_t count = 0;
	uint32_t displayVal = 0;




	displayMessage_t displayMessage;
	buttonMessage_t buttonMessage;

	BaseType_t xStatus;

	uint8_t fUpHold = 0;
	uint8_t fDownHold = 0;

	uint8_t bandNum = 4;
	uint8_t classType = CLASS_EXTRA;

	uint32_t freq = get_default_freq(bandNum);
	uint32_t oldFreq = freq;
	setVFO(freq);

	displayVal = freq / 100;
	displayMessage.displayDestination = DIGIT_DISPLAY;
	displayMessage.val = displayVal;
	xStatus = xQueueSendToBack(xDisplayQueue, &displayMessage, 0);

	if (xStatus != pdPASS) {
		//todo: add an assert or wait
	}

	for (;;) {
		count++;

		if (fUpHold == 1)
			freq += 1000;

		if (fDownHold == 1)
			freq -= 1000;

		if (uxQueueMessagesWaiting(xButtonQueue) > 0) {
			xStatus = xQueueReceive(xButtonQueue, &buttonMessage, 0);
			if (xStatus == pdPASS) {

				//BAND UP BUTTON
				if (buttonMessage.buttonName == BUTTON1) {
					if (buttonMessage.buttonEvent == BUTTON_PRESS) {
						bandNum = next_band(bandNum, classType);
						freq = get_default_freq(bandNum);
					}
				}

				//BAND DOWN BUTTON
				if (buttonMessage.buttonName == BUTTON2) {
					if (buttonMessage.buttonEvent == BUTTON_PRESS) {
						bandNum = previous_band(bandNum, classType);
						freq = get_default_freq(bandNum);
					}
				}


				//FREQ UP BUTTON
				if (buttonMessage.buttonName == BUTTON4) {

					if (buttonMessage.buttonEvent == BUTTON_PRESS)
						freq += 100;
					if (buttonMessage.buttonEvent == BUTTON_HOLD)
						fUpHold = 1;
					if (buttonMessage.buttonEvent == BUTTON_RELEASE)
						fUpHold = 0;
				}


				//FREQ UP BUTTON
				if (buttonMessage.buttonName == BUTTON6) {

					if (buttonMessage.buttonEvent == BUTTON_PRESS)
						freq -= 100;
					if (buttonMessage.buttonEvent == BUTTON_HOLD)
						fDownHold = 1;
					if (buttonMessage.buttonEvent == BUTTON_RELEASE)
						fDownHold = 0;

				}

				//FREQ SWEEP BUTTON
				if (buttonMessage.buttonName == BUTTON5) {

					//start a sweep sequence
					if (buttonMessage.buttonEvent == BUTTON_HOLD) {
						uint32_t swp_start_freq = 1000000;
						uint32_t swp_stop_freq  = 30000000;
						uint32_t swp_steps = 100;
						uint32_t swp_duration = 500; //ms
						uint32_t swp_current_freq = swp_start_freq;

						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); //used to trigger the scope
						for(uint32_t step = 0; step < swp_steps; step++) {
							swp_current_freq = swp_start_freq + (swp_stop_freq-swp_start_freq)/swp_steps*step;
							setVFO(swp_current_freq);
						}
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

					}

				}
			}

		}


		if (freq != oldFreq) {

			freq = check_freq_range(freq, bandNum, classType); //make sure the freq is a valid HAM frequency

			oldFreq = freq;
			setVFO(freq);
			displayVal = freq / 100;
			displayMessage.displayDestination = DIGIT_DISPLAY;
			displayMessage.val = displayVal;

			xStatus = xQueueSendToBack(xDisplayQueue, &displayMessage, 0);
			if (xStatus != pdPASS) {
				//todo: add an assert or wait
			}

		}

		osDelay(50);
	}
}

  /* USER CODE END StartDefaultTask */


/* USER CODE BEGIN Application */
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
