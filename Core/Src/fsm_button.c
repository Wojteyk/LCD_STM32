/*
 * fsm_button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "fsm_button.h"

static ButtonState currentState;

static uint32_t pressStartTime;


void button_CheckState()
{
	uint32_t currentTime = HAL_GetTick();
	uint8_t isCurrentlyPressed = HW_isPressedButton();

	switch(currentState)
	{
	case IDLE:
		if(isCurrentlyPressed){
			currentState = DEBOUNCING;
			pressStartTime = HAL_GetTick();
		}
		break;

	case DEBOUNCING:
		if(!isCurrentlyPressed)
		{
			currentState = IDLE;
		}
		else if((currentTime - pressStartTime) > DEBOUNCE_TIME_MS)
		{
			currentState = PRESSED;
		}
		break;

	case PRESSED:
		if(!isCurrentlyPressed){
			Ui_FSM_ShortPressActionDetected();
			currentState = IDLE;
		}
		else if ((currentTime - pressStartTime) > LONG_PRESS_TIME_MS){
			Ui_FSM_LongPressActionDetected();
			currentState = LONG_PRESS_WAIT;
		}
		break;

	case LONG_PRESS_WAIT:
		if(!isCurrentlyPressed){
			currentState = IDLE;
		}
		break;
	}
}

