/*
 * lcd.h
 *
 *  Created on: May 9, 2025
 *      Author: wojte
 *
 *  Header file for LCD driver based on the ST7735S controller.
 *  Provides a high-level API for drawing graphics and transferring
 *  framebuffer data to the display.
 */

#pragma once

#include <stdint.h>

// Dispaly dimensions
#define LCD_WIDTH 160
#define LCD_HEIGHT 128

//Color definitions
#define BLACK			0x0000
#define RED				0x00f8
#define GREEN			0xe007
#define BLUE			0x1f00
#define YELLOW			0xe0ff
#define MAGENTA			0x1ff8
#define CYAN			0xff07
#define WHITE			0xffff

/**
 * @brief Transfers the framebuffer content to the display.
 *        Call this after drawing operations to refresh the screen.
 */
void lcdCopy();

/**
 * @brief Sets a single pixel in the framebuffer.
 * @param x X coordinate (0–LCD_WIDTH-1)
 * @param y Y coordinate (0–LCD_HEIGHT-1)
 * @param color 16-bit RGB565 color value
 */
void lcdFillPixel(int x, int y, uint16_t color);

/**
 * @brief Initializes the LCD display and its controller.
 *        Must be called once before using any drawing functions.
 */
void lcdInit();

/**
 * @brief Draws a straight line between two points.
 * @param x0 Starting X coordinate
 * @param x1 Ending X coordinate
 * @param y0 Starting Y coordinate
 * @param y1 Ending Y coordinate
 * @param color 16-bit RGB565 color value
 */
void lcdDrawLine(int x0, int x1, int y0, int y1, uint16_t color);

/**
 * @brief Draws a rectangle outline (not filled).
 * @param x Top-left corner X coordinate
 * @param y Top-left corner Y coordinate
 * @param width Rectangle width in pixels
 * @param height Rectangle height in pixels
 * @param color 16-bit RGB565 color value
 */
void lcdDrawRectangle(int x, int y, int width, int heigth, uint16_t color);

/**
 * @brief Draws a filled rectangle.
 * @param x Top-left corner X coordinate
 * @param y Top-left corner Y coordinate
 * @param width Rectangle width in pixels
 * @param height Rectangle height in pixels
 * @param color 16-bit RGB565 color value
 */
void lcdFillRectangle(int x, int y, int width, int heigth, uint16_t color);

