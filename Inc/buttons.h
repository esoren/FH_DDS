/*
 * buttons.h
 *
 *  Created on: Jun 10, 2018
 *      Author: esoren
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

void StartButtonTask(void const * argument);

typedef enum {
	BUTTON1=1,
	BUTTON2,
	BUTTON3,
	BUTTON4,
	BUTTON5,
	BUTTON6
} EButtonName;

typedef enum {
	BUTTON_PRESS=1,
	BUTTON_RELEASE,
	BUTTON_HOLD
} EButtonEvent;


typedef struct {
	EButtonName buttonName;
	EButtonEvent buttonEvent;
} buttonMessage_t;






#endif /* BUTTONS_H_ */
