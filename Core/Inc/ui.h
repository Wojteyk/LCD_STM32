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

#define BTN_DEFAULT_WIDTH      110
#define BTN_DEFAULT_HEIGHT     25
#define BTN_DEFAULT_RADIUS     8

#define BTN_RETURN_WIDTH      16
#define BTN_RETURN_HEIGHT     16
#define BTN_RETURN_RADIUS     4

#define HIGHLIGHT_COLOR 	  WHITE
#define BACKGROUND_COLOR   	  BLACK

/**
 * @brief Structure representing a static label on the UI.
 *
 * This label displays fixed text that does not change at runtime.
 */
typedef struct{
    uint8_t x;          ///< X position of the top-left corner of the label.
    uint8_t y;          ///< Y position of the top-left corner of the label.
    const char *text;   ///< Pointer to the text string displayed on the label.
    uint16_t textColor; ///< 16-bit text color in RGB565 format.
    uint16_t bgColor;   ///< 16-bit background color in RGB565 format.
} Label_Const;

/**
 * @brief Structure representing a dynamic label on the UI.
 *
 * This label can display text that may change at runtime. The text
 * can be modified via the `dataPtr` pointer.
 */
typedef struct{
    uint8_t x;          ///< X position of the top-left corner of the label.
    uint8_t y;          ///< Y position of the top-left corner of the label.
    char *text;         ///< Pointer to the text string displayed on the label.
    uint16_t textColor; ///< 16-bit text color in RGB565 format.
    uint16_t bgColor;   ///< 16-bit background color in RGB565 format.
    char *dataPtr;      ///< Pointer to the dynamic data string used to update the label.
} Label_Dynamic;

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
 *
 * A Page groups together all UI elements that are displayed
 * on a particular screen, including buttons and labels.
 * This allows the UI to manage and render multiple pages independently.
 */
typedef struct {
    Button* const *buttons;        ///< Pointer to a constant array of pointers to Button structures on this page.
    size_t buttonCount;             ///< The total number of buttons in the 'buttons' array.

    const Label_Const* const *labels_Const; ///< Pointer to a constant array of pointers to static labels.
    size_t label_Const_Count;                ///< The number of static labels on this page.

    Label_Dynamic* const *labels_Dynamic;   ///< Pointer to a constant array of pointers to dynamic labels.
    size_t label_Dynamic_Count;             ///< The number of dynamic labels on this page.
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
void Ui_MoveHighlight(uint8_t dirDown);

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

/**
 * @brief Handles detected move actions from encoder.
 * @param dirDown Direction of movement: non-zero for down/next, zero for up/previous.
 * @details Usually called by FSM or hardware interrupt when a movement event is detected.
 */
void Ui_MoveActionDetected(uint8_t dirDown);

/**
 * @brief Update the UI with the latest DHT11 sensor readings.
 *
 * This function formats the temperature and humidity values into strings
 * and updates the corresponding dynamic labels on the sensors page.
 * If the currently displayed page is the sensors page, the labels
 * are redrawn and the LCD buffer is copied to the screen.
 *
 * @param temperature The current temperature in Celsius.
 * @param humidity The current relative humidity in percent.
 */
void Ui_UpdateDHTData(float temperature, float humidity);

