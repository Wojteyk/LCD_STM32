/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"

  static Button_menu firstButton ={
	    .x = 30,
		.y = 15,
		.width = BTN_DEFAULT_WIDTH,
		.height = BTN_DEFAULT_HEIGHT,
		.radius = BTN_DEFAULT_RADIUS,
		.text = "Ustawienia",
		.textColor = BLACK,
		.bgColor = BLUE,
		.onClick = NULL
  };

  static Button_menu secondButton ={
	      .x = 30,
	      .y = 50,
	      .width = BTN_DEFAULT_WIDTH,
	      .height = BTN_DEFAULT_HEIGHT,
	      .radius = BTN_DEFAULT_RADIUS,
	      .text = "Telefon",
	      .textColor = BLACK,
	      .bgColor = BLUE,
	      .onClick = NULL
  };

  static Button_menu thirdButton = {
	      .x = 30,
	      .y = 85,
	      .width = BTN_DEFAULT_WIDTH,
	      .height = BTN_DEFAULT_HEIGHT,
	      .radius = BTN_DEFAULT_RADIUS,
	      .text = "Temperatura",
	      .textColor = BLACK,
	      .bgColor = BLUE,
	      .onClick = NULL,
  };

  static Button_menu* const menuButtons[] = {
		  &firstButton,
		  &secondButton,
		  &thirdButton
  };

#define Num_Of_Menu_Buttons (sizeof(menuButtons) / sizeof(menuButtons[0]))

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
	for(int i = 0; i < Num_Of_Menu_Buttons; i++)
	{
		Ui_DrawButton(menuButtons[i]);
	}
}

void Ui_ChangeMenuTheme( uint16_t changedTextColor, uint16_t changedBgColor)
{

	for(int i = 0; i < Num_Of_Menu_Buttons; i++ )
	{
		menuButtons[i]->textColor = changedTextColor;
		menuButtons[i]->bgColor = changedBgColor;
	}
}







