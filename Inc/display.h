/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __display_H
#define __display_H
#ifdef __cplusplus
 extern "C" {
#endif

osThreadId displayTaskHandle;
void StartDisplayTask(void const * argument);
void updateRgDisplay(unsigned int red_mask, unsigned int green_mask);
void updateStatusDisplay(unsigned int status_mask);
void updateCharacterDisplay(unsigned int display_val);
unsigned char getDigitFromInt(unsigned int display_val, unsigned char pos);
unsigned char getDigitMask(unsigned char digit);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
