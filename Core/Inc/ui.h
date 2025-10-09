/*
 * ui.h
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */

#pragma once

#include <stdio.h>
#include <string.h>
#include "lcd.h"

#define BTN_DEFAULT_WIDTH      100
#define BTN_DEFAULT_HEIGHT     25
#define BTN_DEFAULT_RADIUS     8

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int radius;

	const char *text;
	uint16_t textColor;
	uint16_t bgColor;

	void(*onClick)(void);
} Button_menu;

void Ui_DrawButton(const Button_menu *btn);
