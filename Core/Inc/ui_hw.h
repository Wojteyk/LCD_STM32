/*
 * ui_hw.h
 *
 *  Created on: Oct 11, 2025
 *      Author: wojte
 */

#pragma once

#include "gpio.h"
#include "tim.h"


/**
 * @brief Sets the brightness of the LCD backlight.
 * @details This function controls the LCD backlight brightness by adjusting
 * the duty cycle of a PWM signal.
 * @param percentage The desired brightness level, from 0 (off) to 100 (full brightness).
 * Values above 100 will be capped at 100.
 * @retval None
 */
void HW_setBacklightBrightness(uint8_t percentage);

/**
 * @brief Checks if the user button is currently pressed.
 * @details Reads the state of the GPIO pin connected to the user button.
 * @retval 1 if the button is pressed.
 * @retval 0 if the button is not pressed.
 */
uint8_t HW_isPressedButton();

uint16_t HW_GetEncoderValue();
