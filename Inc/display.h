/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __display_H
#define __display_H
#ifdef __cplusplus
 extern "C" {
#endif

void updateRgDisplay(uint32_t red_mask, uint32_t green_mask);
void updateStatusDisplay(uint32_t status_mask);
void updateCharacterDisplay(uint32_t display_val);
unsigned char getDigitFromInt(uint32_t display_val, unsigned char pos);
unsigned char getDigitMask(unsigned char digit);

typedef enum {
	RED_DISPLAY=1,
	GREEN_DISPLAY,
	DIGIT_DISPLAY,
	STATUS_DISPLAY
} EDisplayDestination;

typedef struct {
	EDisplayDestination displayDestination;
	uint32_t val;
} displayMessage_t;


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
