/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"

  Button_menu firstButton = {
	  .x = 30,
	  .y = 15,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Ustawienia",
	  .textColor = BLACK,
	  .bgColor = BLUE
  };

  Button_menu secondButton = {
  	  .x = 30,
  	  .y = 50,
  	  .width = BTN_DEFAULT_WIDTH,
  	  .height = BTN_DEFAULT_HEIGHT,
  	  .radius = BTN_DEFAULT_RADIUS,
  	  .text = "Telefon",
  	  .textColor = BLACK,
  	  .bgColor = BLUE
    };

  Button_menu thirdButton = {
    	  .x = 30,
    	  .y = 85,
    	  .width = BTN_DEFAULT_WIDTH,
    	  .height = BTN_DEFAULT_HEIGHT,
    	  .radius = BTN_DEFAULT_RADIUS,
    	  .text = "Temperatura",
    	  .textColor = BLACK,
    	  .bgColor = BLUE
      };

static void Ui_DrawButton(const Button_menu *btn)
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

void Ui_DrawMenu(){
	Ui_DrawButton(&firstButton);
	Ui_DrawButton(&secondButton);
	Ui_DrawButton(&thirdButton);
}
