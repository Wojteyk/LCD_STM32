/*
 * fsm_button.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include <fsm_controls.h>	// to do - chanege name please

#define ENCODER_STEP 4

static uint32_t encoder_pressStartTime;

static int16_t encoderValue;

volatile uint8_t firstDebounce = 1;

void button_CheckState()
{
	if(HW_isPressedButton())
	{
		__HAL_TIM_SET_COUNTER(&htim14, 0);
		HAL_TIM_Base_Start_IT(&htim14);
		firstDebounce = 0;
	}else if (!firstDebounce)
	{
		Ui_FSM_ShortPressActionDetected();
		firstDebounce = 1;
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

