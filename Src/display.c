/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "display.h"
#include "globals.h"

/*
 0        0b11110011
 1        0b01100000
 2        0b11010101
 3        0b11110100
 4        0b01100110
 5        0b10110110
 6        0b10110111
 7        0b11100000
 8        0b11110111
 9        0b11100110
 */
//const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
void StartDisplayTask(void const * argument) {
	uint32_t status_mask = 0;
	uint32_t digit_val = 0;
	uint32_t red_mask = 0;
	uint32_t green_mask = 0;
	displayMessage_t displayMessage;
	BaseType_t xStatus;

	for (;;) {

		//Check the queue to see if any displays are changing
		if (uxQueueMessagesWaiting(xDisplayQueue) > 0) {
			xStatus = xQueueReceive(xDisplayQueue, &displayMessage, 0);
			if (xStatus == pdPASS) {

				switch (displayMessage.displayDestination) {
				case (STATUS_DISPLAY):
					status_mask = displayMessage.val;
					break;
				case (GREEN_DISPLAY):
					green_mask = displayMessage.val;
					break;
				case (RED_DISPLAY):
					red_mask = displayMessage.val;
					break;
				case (DIGIT_DISPLAY):
					digit_val = displayMessage.val;
					break;

				}

			}
		}

		// UPDATE and multiplex the displays
		updateStatusDisplay(status_mask);
		updateRgDisplay(red_mask, green_mask);
		updateCharacterDisplay(digit_val);
		osDelay(1);
	}
}

void updateCharacterDisplay(uint32_t display_val) {
	static unsigned char current_digit = 0;
	unsigned char digit = 0;
	unsigned int digit_bitmask = 0;

	digit = getDigitFromInt(display_val, current_digit);
	digit_bitmask = getDigitMask(digit);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);

	//CLEAR THE SHIFT REGISTER
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	asm("NOP");
	asm("NOP");
	asm("NOP");
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);

	for (unsigned char inc = 0; inc < 8; inc++) {

		//SET THE DATA LINE
		if ((digit_bitmask >> inc & 0x01) == 1) {
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
		}

		//CLOCK THE SHIFT REGISTER
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
		asm("NOP");
		asm("NOP");
		asm("NOP");
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);

	}

	switch (current_digit) {
	case 0:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
		break;
	}

	current_digit = (current_digit + 1) % 6;
	return;
}

unsigned char getDigitFromInt(uint32_t display_val, unsigned char pos) {

	unsigned char digit = 0;
	if (display_val > 999999) {
		display_val = 999999;
	}

	for (int i = 0; i < pos; i++) {
		display_val /= 10;
	}
	digit = display_val % 10;

	return digit;

}

unsigned char getDigitMask(unsigned char digit) {
	unsigned char digit_mask = 0;

	if (digit > 9) {
		digit = 8;
	}
	switch (digit) {
	case 0:
		digit_mask = 0b11110011;
		break;
	case 1:
		digit_mask = 0b01100000;
		break;
	case 2:
		digit_mask = 0b11010101;
		break;
	case 3:
		digit_mask = 0b11110100;
		break;
	case 4:
		digit_mask = 0b01100110;
		break;
	case 5:
		digit_mask = 0b10110110;
		break;
	case 6:
		digit_mask = 0b10110111;
		break;
	case 7:
		digit_mask = 0b11100000;
		break;
	case 8:
		digit_mask = 0b11110111;
		break;
	case 9:
		digit_mask = 0b11100110;
		break;
	}
	return digit_mask;
}

void updateRgDisplay(uint32_t red_mask, uint32_t green_mask) {
	static unsigned char bank = 0;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

	switch (bank) {
	case (0):
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);

		if (red_mask >> 3 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		if (red_mask >> 7 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		if (red_mask >> 11 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
		if (red_mask >> 15 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
		if (red_mask >> 19 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

		if (green_mask >> 3 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		if (green_mask >> 7 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		if (green_mask >> 11 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
		if (green_mask >> 15 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
		if (green_mask >> 19 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);

		break;
	case (1):
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);

		if (red_mask >> 2 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		if (red_mask >> 6 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		if (red_mask >> 10 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
		if (red_mask >> 14 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
		if (red_mask >> 18 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

		if (green_mask >> 2 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		if (green_mask >> 6 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		if (green_mask >> 10 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
		if (green_mask >> 14 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
		if (green_mask >> 18 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);

		break;

	case (2):
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

		if (red_mask >> 1 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		if (red_mask >> 5 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		if (red_mask >> 9 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
		if (red_mask >> 13 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
		if (red_mask >> 17 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

		if (green_mask >> 1 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		if (green_mask >> 5 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		if (green_mask >> 9 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
		if (green_mask >> 13 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
		if (green_mask >> 17 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);

		break;

	case (3):
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);

		if (red_mask >> 0 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		if (red_mask >> 4 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		if (red_mask >> 8 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
		if (red_mask >> 12 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
		if (red_mask >> 16 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

		if (green_mask >> 0 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		if (green_mask >> 4 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		if (green_mask >> 8 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
		if (green_mask >> 12 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
		if (green_mask >> 16 & 0x01)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);

		break;

	}

	bank = (bank + 1) % 4;
}

//0: 5
//1: 1
//2: 7
//3: 2
//4: 3
//5: 4
//6: 6
//7: 0

void updateStatusDisplay(uint32_t status_mask) {
	unsigned static char led = 0;
	led = (led + 1) % 8;

	//Start with LED 8 (power LED)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

	switch (led) {
	case (0):
		if (status_mask >> 0 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		}
		break;
	case (1):
		if (status_mask >> 1 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		}
		break;
	case (2):
		if (status_mask >> 2 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		}
		break;
	case (3):
		if (status_mask >> 3 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		}
		break;
	case (4):
		if (status_mask >> 4 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		}
		break;
	case (5):
		if (status_mask >> 5 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		}
		break;
	case (6):
		if (status_mask >> 6 & 0x01) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		}
		break;

	}

	return;
}

