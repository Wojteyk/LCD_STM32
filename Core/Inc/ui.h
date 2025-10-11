/*
 * ui.h
 *
 * Created on: Oct 10, 2025
 * Author: wojte
 */

#pragma once

#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "ui_hw.h"

#define BTN_DEFAULT_WIDTH      100
#define BTN_DEFAULT_HEIGHT     25
#define BTN_DEFAULT_RADIUS     8

#define BTN_RETURN_WIDTH      16
#define BTN_RETURN_HEIGHT     16
#define BTN_RETURN_RADIUS     4

#define HIGHLIGHT_COLOR 	  WHITE
#define BACKGROUND_COLOR   	  BLACK

/**
 * @brief Structure representing an interactive menu button.
 *
 * Contains all parameters necessary for drawing and handling user interaction.
 */
typedef struct Button {
	int x;                 ///< X position (top-left corner).
	int y;                 ///< Y position (top-left corner).
	int width;             ///< The width of the button.
	int height;            ///< The height of the button.
	int radius;            ///< The corner radius for the rounded rectangle.

	const char *text;      ///< The text displayed on the button.
	uint16_t textColor;    ///< 16-bit text color (RGB565).
	uint16_t bgColor;      ///< 16-bit background color (RGB565).

	void(*onClick)(struct Button *self);  ///< Pointer to the callback function executed upon press.
} Button;

/**
 * @brief Represents a single screen or view in the UI.
 * @details Contains a collection of buttons that are displayed together on the screen.
 */
typedef struct {
	Button* const *buttons; ///< A constant array of pointers to the Button structures on this page.
	size_t buttonCount;     ///< The total number of buttons in the 'buttons' array.
} Page;

/**
 * @brief Defines a color theme for UI elements.
 * @details Contains colors for text and backgrounds, used for changing themes.
 */
typedef struct {
    uint16_t textColor;     ///< The 16-bit text color (RGB565).
    uint16_t bgColor;       ///< The 16-bit background color for buttons (RGB565).
} Theme;

/**
 * @brief Draws the entire UI menu using predefined global Button_menu instances.
 *
 * This function calls the drawing routines for all buttons defined
 * within the ui.c module (e.g., firstButton, secondButton, thirdButton).
 */
void Ui_DrawPage();

/**
 * @brief Sets a new page as the active view, resetting the highlight state.
 * @param newPage Pointer to the Page structure to activate.
 */
void Ui_SetCurrentPage(const Page *newPage);

/**
 * @brief Changes the background and text color theme for ALL buttons in the application.
 * * Ui_DrawPage is called afterwards for changes to become visible.
 * @param btnTextColor New 16-bit text color (RGB565).
 * @param btnBgColor New 16-bit background color (RGB565).
 */
void Ui_ChangeMenuTheme( uint16_t btnTextColor, uint16_t btnBgColor);

/**
 * @brief Moves the highlight (cursor) to the next button in the list.
 * * If the highlight reaches the end of the list, it wraps back to the first element.
 */
void Ui_MoveHighlightDown();

/**
 * @brief Callback invoked by the button Finite State Machine (FSM) upon detecting a stable short press.
 * * This function maps the FSM event to the 'onClick' action of the currently highlighted button.
 */
void Ui_FSM_ShortPressActionDetected();

/**
 * @brief Callback invoked by the button Finite State Machine (FSM) upon detecting a stable long press event.
 * * This function is defined in the UI core module (ui.c) to handle the resulting action,
 * which often triggers continuous actions or a different page flow.
 */
void Ui_FSM_LongPressActionDetected();





