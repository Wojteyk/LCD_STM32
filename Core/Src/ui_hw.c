/*
 * ui_hw.c
 *
 *  Created on: Oct 11, 2025
 *      Author: wojte
 */
#include "ui_hw.h"

void HW_setBacklightBrightness(uint8_t percentage)
{
  if (percentage > 100) {
	  percentage = 100;
  }

  uint32_t period = __HAL_TIM_GET_AUTORELOAD(&htim10);

  // calculate duty cycle
  uint32_t ccr_value = (period * percentage) / 100;

  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, ccr_value);

}


uint8_t HW_isPressedButton()
{
	return !HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}

uint16_t HW_GetEncoderValue(){
	return __HAL_TIM_GET_COUNTER(&htim8);
}
