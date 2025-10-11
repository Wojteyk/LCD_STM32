/*
 * ui_hw.h
 *
 *  Created on: Oct 11, 2025
 *      Author: wojte
 */

#pragma once

#include "gpio.h"
#include "tim.h"

void HW_setBacklightBrightness(uint8_t percentage);

uint8_t HW_isPressedButton();
