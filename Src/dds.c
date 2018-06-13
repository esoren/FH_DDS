/*
 * dds.c
 *
 *  Created on: May 29, 2018
 *      Author: esoren
 */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "dds.h"

#define DDS_CLK 100000000.0f

//DDS_CLK = PA8
//DDS_FQ_UD = PB5
//DDS_DATA = PB4


void resetDDS() {
	return;

}


void setVFO(uint32_t freq) {
	uint32_t config = 0;

	//set pointer to bit 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	asm("NOP");
	asm("NOP");
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

	float bitfreq = 4294967296.0f/DDS_CLK;   //TODO: add calibration
	bitfreq *= freq;
	config = (uint32_t)bitfreq;

	for(unsigned char bit = 0; bit < 32; bit++) {
		//setup the data pin
		if( (config >> bit) & 0x01 ) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		}

		asm("NOP");
		asm("NOP");

		//clock
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		asm("NOP");
		asm("NOP");
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	for(unsigned char bit = 0; bit < 8; bit++) {
		//clock
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		asm("NOP");
		asm("NOP");
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}

	//commit changes
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	asm("NOP");
	asm("NOP");
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

	return;
}
