/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"


void Ui_DrawButton(const Button_menu *btn)
{
	//draw button
	lcdFillRoundRectangle(btn->x,
						  btn->y,
						  btn->width,
						  btn->height,
						  btn->radius,
						  btn->bgColor);
	//draw text center aligned
	lcdDrawText(btn->x + (BTN_DEFAULT_WIDTH - ((FONT_WIDTH+1)*strlen(btn->text)))/2,
				btn->y + (BTN_DEFAULT_HEIGHT - FONT_HEIGHT)/2,
				btn->text,
				btn->textColor,
				btn->bgColor);

}

