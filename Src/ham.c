/*
 * ham.c
 *
 *  Created on: Jun 11, 2018
 *      Author: esoren
 */

#include "ham.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"


const uint32_t class_min_freq[NUM_CLASSES][NUM_BANDS]  =     {
							//160M      80M      40M      30M       20M       17M       15M       12M       10M
							{0000000, 3525000, 7025000, 00000000, 00000000, 00000000, 21025000, 00000000, 28000000},
							{1800000, 3525000, 7025000, 10100000, 14025000, 18068000, 21025000, 24890000, 28000000},
							{1800000, 3500000, 7000000, 10100000, 14000000, 18068000, 21000000, 24890000, 28000000}};


const uint32_t class_max_freq[NUM_CLASSES][NUM_BANDS]  =    {
							//160M      80M      40M      30M       20M       17M       15M       12M       10M
							{0000000, 3600000, 7125000, 00000000, 00000000, 00000000, 21200000, 00000000, 28000000},
							{2000000, 4000000, 7300000, 10150000, 14350000, 18168000, 21450000, 24990000, 29700000},
							{2000000, 4000000, 7300000, 10150000, 14350000, 18168000, 21450000, 24990000, 29700000}};


const uint8_t band_allowed[NUM_CLASSES][NUM_BANDS]    =    {
		                   //160M 80M 40M 30M 20M 17M 15M 12M 10M
							{  0,  1,  1,  0,  0,  0,  1,  0,  1},
							{  1,  1,  1,  1,  1,  1,  1,  1,  1},
							{  1,  1,  1,  1,  1,  1,  1,  1,  1}};

const uint32_t default_band_freq[NUM_BANDS] =
							//160M      80M      40M      30M       20M       17M       15M       12M       10M
							{1900000, 3700000, 7030000, 10000000, 14060000, 18068000, 21300000, 2495000, 2850000};


uint32_t check_freq_range(uint32_t freq, uint8_t bandNum, uint8_t classNum) {
	if (freq < class_min_freq[classNum][bandNum])
		freq = class_min_freq[classNum][bandNum];

	if (freq > class_max_freq[classNum][bandNum])
		freq = class_max_freq[classNum][bandNum];

	return freq;
}

uint8_t next_band(uint8_t bandNum, uint8_t classNum) {
	bandNum = (bandNum+1) % NUM_BANDS;
	while(0 == band_allowed[classNum][bandNum]) {
		bandNum = (bandNum+1) % NUM_BANDS;
	}
	return bandNum;
}

uint8_t previous_band(uint8_t bandNum, uint8_t classNum) {
	bandNum = (bandNum+(NUM_BANDS-1)) % NUM_BANDS;
	while(0 == band_allowed[classNum][bandNum]) {
		bandNum = (bandNum+(NUM_BANDS-1)) % NUM_BANDS;
	}
	return bandNum;

}

uint32_t get_default_freq(uint8_t bandNum) {
	return default_band_freq[bandNum];
}

