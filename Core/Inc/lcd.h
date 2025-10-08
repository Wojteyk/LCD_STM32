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
#include <font.h>

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
void lcdDrawRectangle(int x, int y, int width, int height, uint16_t color);

/**
 * @brief Draws a filled rectangle.
 * @param x Top-left corner X coordinate
 * @param y Top-left corner Y coordinate
 * @param width Rectangle width in pixels
 * @param height Rectangle height in pixels
 * @param color 16-bit RGB565 color value
 */
void lcdFillRectangle(int x, int y, int width, int height, uint16_t color);


/**
 * @brief Draws the outline of a circle.
 * @param x0 X coordinate of the circle center
 * @param y0 Y coordinate of the circle center
 * @param radius Circle radius in pixels
 * @param color 16-bit RGB565 color value
 */
void lcdDrawCircle(int x0, int y0, int radius, uint16_t color);

/**
 * @brief Draws a filled circle.
 * @param x0 X coordinate of the circle center
 * @param y0 Y coordinate of the circle center
 * @param radius Circle radius in pixels
 * @param color 16-bit RGB565 color value
 */
void lcdFillCircle(int x0, int y0, int radius, uint16_t color);

/**
 * @brief Draws the outline of a rectangle with rounded corners.
 * @param x0     X coordinate of the top-left corner
 * @param y0     Y coordinate of the top-left corner
 * @param width  Total width of the rectangle in pixels
 * @param height Total height of the rectangle in pixels
 * @param radius Radius of the corners in pixels
 * @param color  16-bit RGB565 color value
 */
void lcdDrawRoundRectangle(int x0, int y0, int width, int height, int radius, uint16_t color);

/**
 * @brief Draws a filled rectangle with rounded corners.
 * @param x0     X coordinate of the top-left corner
 * @param y0     Y coordinate of the top-left corner
 * @param width  Total width of the rectangle in pixels
 * @param height Total height of the rectangle in pixels
 * @param radius Radius of the corners in pixels
 * @param color  16-bit RGB565 color value
 */
void lcdFillRoundRectangle(int x0, int y0, int width, int height, int radius, uint16_t color);


/**
 * @brief Draws a string of text on the screen.
 * @param x0      X coordinate of the top-left corner of the text
 * @param y0      Y coordinate of the top-left corner of the text
 * @param str     Pointer to the null-terminated string to be drawn
 * @param color   16-bit RGB565 color value for the text
 * @param bgColor 16-bit RGB565 color value for the background
 */
void lcdDrawText(int x0, int y0, const char* str, uint16_t color, uint16_t bgColor);



