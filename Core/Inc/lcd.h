/*
 * lcd.h
 *
 *  Created on: May 9, 2025
 *      Author: wojte
 */
#pragma once

#include <stdint.h>

#define LCD_WIDTH 160
#define LCD_HEIGHT 128

//color definitions
#define BLACK     0x0000
#define RED       0xf800
#define GREEN     0x07e0
#define BLUE      0x001f
#define YELLOW    0xffe0
#define MAGENTA   0xf81f
#define CYAN      0x07ff
#define WHITE     0xffff

void lcdCopy();

void lcdFillPixel(int x, int y, uint16_t color);

void lcdInit();

void lcdDrawLine(int x0, int x1, int y0, int y1, uint16_t color);

void lcdDrawRectangle(int x, int y, int width, int heigth, uint16_t color);

void lcdFillRectangle(int x, int y, int width, int heigth, uint16_t color);

