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

#define BTN_DEFAULT_WIDTH      100
#define BTN_DEFAULT_HEIGHT     25
#define BTN_DEFAULT_RADIUS     8

/**
 * @brief Structure representing an interactive menu button.
 *
 * Contains all parameters necessary for drawing and handling user interaction.
 */
typedef struct {
	int x;                 ///< X position (top-left corner).
	int y;                 ///< Y position (top-left corner).
	int width;             ///< The width of the button.
	int height;            ///< The height of the button.
	int radius;            ///< The corner radius for the rounded rectangle.

	const char *text;      ///< The text displayed on the button.
	uint16_t textColor;    ///< 16-bit text color (RGB565).
	uint16_t bgColor;      ///< 16-bit background color (RGB565).

	void(*onClick)(void);  ///< Pointer to the callback function executed upon press.
} Button_menu;

/**
 * @brief Draws the entire UI menu using predefined global Button_menu instances.
 *
 * This function calls the drawing routines for all buttons defined
 * within the ui.c module (e.g., firstButton, secondButton, thirdButton).
 */
void Ui_DrawMenu();
