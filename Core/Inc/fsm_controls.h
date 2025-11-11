#pragma once

#include <stdint.h>
#include "ui.h"
#include "ui_hw.h"

#define DEBOUNCE_ENCODER_TIME_MS 100

/** @brief Minimum time (in ms) required for the button signal to stabilize (debounce). */
#define DEBOUNCE_FSM_TIME_MS	50

/** @brief Time (in ms) after which a press is classified as a long press. */
#define LONG_PRESS_TIME_MS	600

extern volatile uint8_t buttonPressedFlag;

/**
 * @brief Defines the states of the button Finite State Machine (FSM).
 */
typedef enum {
	IDLE,               /**< The button is released and waiting for a press event. */
	DEBOUNCING,         /**< Press detected; time is being measured to eliminate switch bounce. */
	PRESSED,            /**< Signal is stable; waiting for release (short press) or timeout (long press). */
	LONG_PRESS_WAIT     /**< Long press action has occurred; waiting for the physical button to be released. */
} ButtonState;

/**
 * @brief Handles the button state change, triggered by an EXTI interrupt.
 * * This function initiates the debounce timer (TIM14) upon button press and
 * detects a short release if confirmed press monitoring is active.
 * * NOTE: For this interrupt-driven model, this function should be called
 * from the HAL_GPIO_EXTI_Callback() or a task/queue handler.
 */
void button_CheckState();

/**
 * @brief Callback invoked by the button Finite State Machine (FSM) upon detecting a stable short press.
 * * This function is defined in the UI core module (ui.c) to handle the resulting action,
 * which involves executing the 'onClick' action of the currently highlighted button.
 */
extern void Ui_FSM_ShortPressActionDetected();

/**
 * @brief Callback function executed by the FSM upon detecting a stable long press event.
 * * This function is defined in the UI core module (ui.c) to handle the resulting action.
 */
extern void Ui_FSM_LongPressActionDetected();

extern void Ui_MoveActionDetected(uint8_t dirDown);

void encoder_CheckValue();


