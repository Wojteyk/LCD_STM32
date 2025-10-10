#pragma once

#include <stdint.h>

/** @brief Minimum time (in ms) required for the button signal to stabilize (debounce). */
#define DEBOUNCE_TIME_MS	50

/** @brief Time (in ms) after which a press is classified as a long press. */
#define LONG_PRESS_TIME_MS	1000


/**
 * @brief Defines the states of the button Finite State Machine (FSM).
 */
typedef enum {
	IDLE,               /**< The button is released and waiting for a press event. */
	DEBOUNCING,         /**< Press detected; time is being measured to eliminate switch bounce. */
	PRESSED,            /**< Signal is stable; waiting for release (short press) or timeout (long press). */
	LONG_PRESS_WAIT     /**< Long press action has occurred; waiting for the physical button to be released. */
} ButtonState;

/** @brief The current state of the button's FSM. */
extern ButtonState currentState;

/** @brief The time (HAL_GetTick() value in ms) when the stable press started. */
extern uint32_t pressStartTime;

/**
 * @brief The main execution function for the button FSM.
 * * This function must be called periodically in the main loop (polling)
 * or within a regular timer interrupt to continuously analyze the button's
 * pin state and press duration.
 */
void button_CheckState();
