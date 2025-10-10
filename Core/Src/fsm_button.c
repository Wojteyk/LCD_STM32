/*
 * fsm_button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "fsm_button.h"
#include "main.h"

static ButtonState currentState;

static uint32_t pressStartTime;

static uint8_t buttonIsPressed()
{
	return !HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}

void button_CheckState()
{
	uint32_t currentTime = HAL_GetTick();
	uint8_t isCurrentlyPressed = buttonIsPressed();

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

