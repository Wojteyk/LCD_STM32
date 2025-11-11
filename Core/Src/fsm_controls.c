/*
 * fsm_button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include <fsm_controls.h>	// to do - chanege name please

#define ENCODER_STEP 4

static ButtonState currentState;

static uint32_t fsm_pressStartTime, encoder_pressStartTime;

static int16_t encoderValue;


void button_CheckState()
{
	uint32_t currentTime = HAL_GetTick();
	uint8_t isCurrentlyPressed = HW_isPressedButton();

	switch(currentState)
	{
	case IDLE:
		if(isCurrentlyPressed){
			currentState = DEBOUNCING;
			fsm_pressStartTime = HAL_GetTick();
		}
		break;

	case DEBOUNCING:
		if(!isCurrentlyPressed)
		{
			currentState = IDLE;
		}
		else if((currentTime - fsm_pressStartTime) > DEBOUNCE_FSM_TIME_MS)
		{
			currentState = PRESSED;
		}
		break;

	case PRESSED:
		if(!isCurrentlyPressed){
			Ui_FSM_ShortPressActionDetected();
			currentState = IDLE;
		}
		else if ((currentTime - fsm_pressStartTime) > LONG_PRESS_TIME_MS){
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

void encoder_CheckValue()
{
    int16_t currentEncoderValue = (int16_t)__HAL_TIM_GET_COUNTER(&htim8);
    int16_t diff = currentEncoderValue - encoderValue;
    uint32_t currentTime = HAL_GetTick();

    if((currentTime - encoder_pressStartTime) > DEBOUNCE_ENCODER_TIME_MS){
		// overflow correction
		if (diff > 30000) diff -= 65536;
		else if (diff < -30000) diff += 65536;

		if (diff >= ENCODER_STEP) {
			encoderValue = currentEncoderValue;
			Ui_MoveActionDetected(0);
		}
		else if (diff <= -ENCODER_STEP) {
			encoderValue = currentEncoderValue;
			Ui_MoveActionDetected(1);
		}
		encoder_pressStartTime = currentTime;
    }
}

