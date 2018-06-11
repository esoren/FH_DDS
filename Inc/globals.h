/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __globals_H
#define __globals_H
#ifdef __cplusplus
 extern "C" {
#endif


// For the displays
extern uint32_t RED_MASK;
extern uint32_t GREEN_MASK;
extern uint32_t SEGMENT_DISP;
extern uint32_t STATUS_MASK;

extern QueueHandle_t xDisplayQueue;
extern QueueHandle_t xButtonQueue;


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
