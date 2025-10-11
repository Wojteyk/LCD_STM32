/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"

static const Page *currentPage = NULL;
static int currentButtonIndex = 0;
static int currentThemeIndex = 0;

//   ------- Function declarations ------

static void Action_GoToSettings(Button *self);

static void Action_GoBack(Button *self);

static void Action_ChangeTheme(Button *self);

//   ------- Themes ------

static const Theme themes[] = {
		{.textColor = BLACK, .bgColor = BLUE},
		{.textColor = BLACK, .bgColor = RED},
		{.textColor = BLACK, .bgColor = GREEN},
};

#define Num_Of_Themes (sizeof(themes) / sizeof(themes[0]))

//   ------- Return Button ------

static Button returnButton ={
	.x = 7,
	.y = 105,
	.width = BTN_RETURN_WIDTH,
	.height = BTN_RETURN_HEIGHT,
	.radius = BTN_RETURN_RADIUS,
	.text = "<",
	.textColor = BLACK,
	.bgColor = RED,
	.onClick = Action_GoBack
};

//   ------- HOME PAGE ------

static Button homeButton1 ={
	.x = 30,
	.y = 15,
	.width = BTN_DEFAULT_WIDTH,
	.height = BTN_DEFAULT_HEIGHT,
	.radius = BTN_DEFAULT_RADIUS,
	.text = "Ustawienia",
	.textColor = BLACK,
	.bgColor = BLUE,
	.onClick = Action_GoToSettings
};

static Button homeButton2={
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

static Button homeButton3 = {
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


static Button* const menuButtons[] = {
	  &homeButton1,
	  &homeButton2,
	  &homeButton3,
};

#define Num_Of_Menu_Buttons (sizeof(menuButtons) / sizeof(menuButtons[0]))

const Page homePage = {
		  .buttons = menuButtons,
		  .buttonCount = Num_Of_Menu_Buttons
};

//   ------- SETTINGS PAGE ------

static Button settingsButton1 ={
	.x = 30,
	.y = 15,
	.width = BTN_DEFAULT_WIDTH,
	.height = BTN_DEFAULT_HEIGHT,
	.radius = BTN_DEFAULT_RADIUS,
	.text = "Zmaiana motywu",
	.textColor = BLACK,
	.bgColor = BLUE,
	.onClick = Action_ChangeTheme
};

static Button settingsButton2={
	  .x = 30,
	  .y = 50,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Zmiana jasnosci",
	  .textColor = BLACK,
	  .bgColor = BLUE,
	  .onClick = NULL
};

static Button settingsButton3 = {
	  .x = 30,
	  .y = 85,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Place holder",
	  .textColor = BLACK,
	  .bgColor = BLUE,
	  .onClick = NULL,
};


static Button* const settingsButtons[] = {
	  &settingsButton1,
	  &settingsButton2,
	  &settingsButton3,
	  &returnButton
};

#define Num_Of_Settings_Buttons (sizeof(settingsButtons) / sizeof(settingsButtons[0]))

const Page settingsPage = {
		  .buttons = settingsButtons,
		  .buttonCount = Num_Of_Settings_Buttons
};

// ------------------------------------------------------

static void Action_ChangeTheme(Button *self)
{
	currentThemeIndex = (currentThemeIndex + 1) % Num_Of_Themes;

	Ui_ChangeMenuTheme(themes[currentThemeIndex].textColor, themes[currentThemeIndex].bgColor);
}

static void Action_GoToSettings(Button *self)
{
	Ui_SetCurrentPage(&settingsPage);
}

static void Action_GoBack(Button *self)
{
	Ui_SetCurrentPage(&homePage);
}

static void Ui_ExecuteAction()
{
	if(currentPage == NULL || currentPage->buttonCount == 0) return;

	// choose highlithed button
	Button *btn = currentPage->buttons[currentButtonIndex];

	if(btn->onClick != NULL){
		btn->onClick(btn);
	}
}

static void Ui_DrawButton(const Button *btn, uint8_t isHighlited)
{

	if(isHighlited)
	{
		//draw highlithed button
		lcdFillRoundRectangle(btn->x,
							  btn->y,
							  btn->width,
							  btn->height,
							  btn->radius,
							  HIGHLIGHT_COLOR);

		//draw text center aligned
			lcdDrawText(btn->x + (btn->width - ((FONT_WIDTH+1)*strlen(btn->text)))/2,
						btn->y + (btn->height - FONT_HEIGHT)/2,
						btn->text,
						btn->textColor,
						HIGHLIGHT_COLOR);
	}
	else
	{
	//draw button
	lcdFillRoundRectangle(btn->x,
						  btn->y,
						  btn->width,
						  btn->height,
						  btn->radius,
						  btn->bgColor);

	//draw text center aligned
	lcdDrawText(btn->x + (btn->width - ((FONT_WIDTH+1)*strlen(btn->text)))/2,
				btn->y + (btn->height - FONT_HEIGHT)/2,
				btn->text,
				btn->textColor,
				btn->bgColor);
	}
}

void Ui_DrawPage(){

	if(currentPage == NULL) return;

	lcdFillBackground(BACKGROUND_COLOR);

	for(size_t i =0; i < currentPage->buttonCount; i++)
	{
		uint8_t isHihglithed  = (i == currentButtonIndex);
		Ui_DrawButton(currentPage->buttons[i], isHihglithed);
	}
	lcdCopy();
}

void Ui_SetCurrentPage(const Page *newPage)
{
	if(newPage == NULL) return;

	currentPage = newPage;
	currentButtonIndex = 0;

	Ui_DrawPage();
}

void Ui_ChangeMenuTheme( uint16_t changedTextColor, uint16_t changedBgColor)
{

	for(int i = 0; i < Num_Of_Menu_Buttons; i++ )
	{
		menuButtons[i]->textColor = changedTextColor;
		menuButtons[i]->bgColor = changedBgColor;
	}

	for(int i = 0; i < Num_Of_Settings_Buttons; i++ )
		{
			settingsButtons[i]->textColor = changedTextColor;
			settingsButtons[i]->bgColor = changedBgColor;
		}

	Ui_DrawPage();
}

void Ui_MoveHighlightDown()
{
	if(currentPage == NULL || currentPage->buttonCount == 0) return;

	currentButtonIndex++;

	if(currentButtonIndex == currentPage->buttonCount) currentButtonIndex = 0;

	Ui_DrawPage();
}

void Ui_FSM_ShortPressActionDetected()
{
	Ui_MoveHighlightDown();
}


void Ui_FSM_LongPressActionDetected()
{
	Ui_ExecuteAction();
}






