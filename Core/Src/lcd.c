/*
 * lcd.c
 *
 *  Created on: May 9, 2025
 *      Author: wojte
 */
#include <stdlib.h>
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "spi.h"

#define CMD(x) ((x) | 0x100)
#define ST7735S_SLPOUT			0x11
#define ST7735S_DISPOFF			0x28
#define ST7735S_DISPON			0x29
#define ST7735S_CASET			0x2a
#define ST7735S_RASET			0x2b
#define ST7735S_RAMWR			0x2c
#define ST7735S_MADCTL			0x36
#define ST7735S_COLMOD			0x3a
#define ST7735S_FRMCTR1			0xb1
#define ST7735S_FRMCTR2			0xb2
#define ST7735S_FRMCTR3			0xb3
#define ST7735S_INVCTR			0xb4
#define ST7735S_PWCTR1			0xc0
#define ST7735S_PWCTR2			0xc1
#define ST7735S_PWCTR3			0xc2
#define ST7735S_PWCTR4			0xc3
#define ST7735S_PWCTR5			0xc4
#define ST7735S_VMCTR1			0xc5
#define ST7735S_GAMCTRP1		0xe0
#define ST7735S_GAMCTRN1		0xe1

#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 0


static void lcdCmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_SET);
}

static void lcdData(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_SET);
}

static void lcdSend(uint16_t value)
{
	if(value & 0x100)
	{
		lcdCmd(value);
	} else
	{
		lcdData(value);
	}
}

static void lcdData16(uint16_t value)
{
	lcdData(value >> 8);
	lcdData(value);
}

static void lcdSetWindow(int x, int y, int width, int height)
{
	lcdCmd(ST7735S_CASET);
	lcdData16(LCD_OFFSET_X + x);
	lcdData16(LCD_OFFSET_X +  x + width - 1);

	lcdCmd(ST7735S_RASET);
	lcdData16(LCD_OFFSET_Y + y);
	lcdData16(LCD_OFFSET_Y + y + height -1);
}

static const uint16_t initTable[] = {
		 CMD(ST7735S_FRMCTR1), 0x01, 0x2c, 0x2d,
		 CMD(ST7735S_FRMCTR2), 0x01, 0x2c, 0x2d,
		 CMD(ST7735S_FRMCTR3), 0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d,
		 CMD(ST7735S_INVCTR), 0x07,
		 CMD(ST7735S_PWCTR1), 0xa2, 0x02, 0x84,
		 CMD(ST7735S_PWCTR2), 0xc5,
		 CMD(ST7735S_PWCTR3), 0x0a, 0x00,
		 CMD(ST7735S_PWCTR4), 0x8a, 0x2a,
	     CMD(ST7735S_PWCTR5), 0x8a, 0xee,
		 CMD(ST7735S_VMCTR1), 0x0e,
		 CMD(ST7735S_GAMCTRP1), 0x0f, 0x1a, 0x0f, 0x18, 0x2f, 0x28, 0x20, 0x22,
		                         0x1f, 0x1b, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10,
		 CMD(ST7735S_GAMCTRN1), 0x0f, 0x1b, 0x0f, 0x17, 0x33, 0x2c, 0x29, 0x2e,
		                         0x30, 0x30, 0x39, 0x3f, 0x00, 0x07, 0x03, 0x10,
		 CMD(0xf0), 0x01,
		 CMD(0xf6), 0x00,
		 CMD(ST7735S_COLMOD), 0x05,
		 CMD(ST7735S_MADCTL), 0x60,
};

static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT];

void lcdInit()
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	//send all init messages
	for(int i=0; i < sizeof(initTable) / sizeof(uint16_t); i++)
	{
		lcdSend(initTable[i]);
	}

	HAL_Delay(100);

	lcdCmd(ST7735S_SLPOUT); // wake up
	HAL_Delay(120);

	lcdCmd(ST7735S_DISPON); // turn on display

}

void lcdFillPixel(int x, int y, uint16_t color)
{
	frameBuffer[x + y * LCD_WIDTH] = color;
}

void lcdCopy()
{
	lcdSetWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
	lcdCmd(ST7735S_RAMWR);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*) frameBuffer, sizeof(frameBuffer), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

}

void lcdFillBackground(uint16_t color)
{
	for (int y = 0; y < LCD_HEIGHT; y++)
	{
	    for (int x = 0; x < LCD_WIDTH; x++)
	    {
	      lcdFillPixel(x, y, color);
	    }
	}
}

void lcdDrawLine(int x0, int y0, int x1, int y1, uint16_t color)
{
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (1)
	{
		lcdFillPixel(x0, y0, color);

	    if (x0 == x1 && y0 == y1) break;

	    int e2 = 2 * err;

	    if (e2 >= dy)
	    {
	       err += dy;
	       x0 += sx;
	    }

	    if (e2 <= dx) {
	    	err += dx;
	    	y0 += sy;
	    }
	}
}

void lcdDrawRectangle(int x, int y, int width, int height, uint16_t color)
{
	lcdDrawLine(x, y, x + width, y, color);
	lcdDrawLine(x, y, x, y + height, color);
	lcdDrawLine(x + width, y, x + width, y + height, color);
	lcdDrawLine(x, y + height, x + width, y + height, color);
}

void lcdFillRectangle(int x, int y, int width, int height, uint16_t color)
{
	for(int i=0; i < width; i++){
		for(int j = 0; j < height; j++){
			lcdFillPixel(x + i, y + j, color);
		}
	}
}

void lcdDrawCircle(int x0, int y0, int radius, uint16_t color)
{
	// Bresenham algorithm
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (y >= x) {

    	lcdFillPixel(x0 + x, y0 + y, color);
    	lcdFillPixel(x0 + y, y0 + x, color);
    	lcdFillPixel(x0 - x, y0 + y, color);
    	lcdFillPixel(x0 - y, y0 + x, color);
    	lcdFillPixel(x0 + x, y0 - y, color);
    	lcdFillPixel(x0 + y, y0 - x, color);
    	lcdFillPixel(x0 - x, y0 - y, color);
    	lcdFillPixel(x0 - y, y0 - x, color);

        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void lcdFillCircle(int x0, int y0, int radius, uint16_t color) {

	// Bresenham algorithm
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (y >= x) {

        lcdDrawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
        lcdDrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        lcdDrawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);
        lcdDrawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);

        x++;

        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void lcdDrawRoundRectangle(int x0, int y0, int width, int height, int radius, uint16_t color)
{

	int correctedWidth = width - 1;
	int correctedHeight = height - 1;

	lcdDrawLine(x0 + radius, y0, x0 + width - radius, y0, color);

	lcdDrawLine(x0 + radius, y0 + height, x0 + width - radius, y0 + height, color);

	lcdDrawLine(x0, y0 + radius, x0, y0 + height - radius, color);

	lcdDrawLine(x0 + width, y0 + radius, x0 + width, y0 + height - radius, color);


	int x = 0;
	int y = radius;
	int d = 1 - radius;

	while (y >= x)
	{

		// Lower right corner
		lcdFillPixel(x0 - (radius - correctedWidth) + x, y0 - (radius - correctedHeight) + y, color);
		lcdFillPixel(x0 - (radius - correctedWidth) + y, y0 - (radius - correctedHeight) + x, color);

		// Upper right corner
		lcdFillPixel(x0 - (radius - correctedWidth) + x, y0 + radius - y, color);
		lcdFillPixel(x0 - (radius - correctedWidth) + y, y0 + radius - x, color);

		//Lower left corner
		lcdFillPixel(x0 + radius - x, y0 - (radius - correctedHeight) + y, color);
		lcdFillPixel(x0 + radius - y, y0 - (radius - correctedHeight) + x, color);

		// Upper left corner
		lcdFillPixel(x0 + radius - x, y0 + radius - y, color);
		lcdFillPixel(x0 + radius - y, y0 + radius - x, color);

		x++;
		if(d <= 0)
		{
			d += 2 * x +1;
		}
		else
		{
			y--;
			d += 2 * (x -y) + 1;
		}
	}
}

void lcdFillRoundRectangle(int x0, int y0, int width, int height, int radius, uint16_t color)
{
	int correctedWidth = width - 1;
	int correctedHeight = height - 1;

    lcdFillRectangle(x0, y0 + radius, width, height - 2 * radius, color);


    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (y >= x)
    {
    	// Drawing corners
        lcdDrawLine((x0 + radius) - x, (y0 + radius) - y, (x0 + correctedWidth - radius) + x, (y0 + radius) - y, color); // Upper
        lcdDrawLine((x0 + radius) - y, (y0 + radius) - x, (x0 + correctedWidth - radius) + y, (y0 + radius) - x, color); // Upper

        lcdDrawLine((x0 + radius) - x, (y0 + correctedHeight - radius) + y, (x0 + correctedWidth - radius) + x, (y0 + correctedHeight - radius) + y, color); // Lower
        lcdDrawLine((x0 + radius) - y, (y0 + correctedHeight - radius) + x, (x0 + correctedWidth - radius) + y, (y0 + correctedHeight - radius) + x, color); // Lower

        x++;
        if(d <= 0)
        {
            d += 2 * x + 1;
        }
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

static void lcdDrawChar(int x0, int y0, char c, uint16_t color, uint16_t bgColor)
{
    if (c < 32 || c > 126) return;

    for (int row = 0; row < FONT_HEIGHT; row++)
    {
        uint8_t bits = font[c - 32][row];

        for (int col = FONT_WIDTH-1; col >= 0; col--)
        {
            if (bits & (1 << col))
                lcdFillPixel(x0 + col, y0 + row, color);
            else
                lcdFillPixel(x0 + col, y0 + row, bgColor);
        }
    }
}

void lcdDrawText(int x0, int y0, const char* str, uint16_t color, uint16_t bgColor)
{
	int x = x0;
	int y = y0;

	while(*str)
	{
		if(*str == '\n')
		{
			y += FONT_HEIGHT + 2;
			x = x0;
		}
		else
		{
			lcdDrawChar(x, y, *str, color, bgColor);
			x += FONT_WIDTH + 1;
		}

		if(x + FONT_WIDTH >= LCD_WIDTH)
		{	// text wrapping if go beyond lcd width
			y += FONT_HEIGHT + 2;
			x = x0;
		}

		str++;
	}
}
