/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"

// --- Static Global Variables ---

/// @brief Pointer to the currently displayed page.
static const Page *currentPage = NULL;
/// @brief Index of the currently highlighted button on the currentPage.
static int currentButtonIndex = 0;
/// @brief Index of the currently active color theme.
static int currentThemeIndex = 0;
/// @brief Index of the currently selected brightness level.
static int currentBrightnesIndex = 0;

//   ------- Function declarations ------

/**
 * @brief Draws a single button on the screen.
 * @details Renders a rounded rectangle for the button body and centers the text.
 * The button is drawn with a different background color if it is highlighted.
 * @param btn Pointer to the constant Button object to draw.
 * @param isHighlited Flag indicating if the button is currently selected (1) or not (0).
 */
static void Ui_DrawButton(const Button *btn, uint8_t isHighlited);

/**
 * @brief Draws a text label on the screen.
 * @details Displays static text at a fixed position with specified text and background colors.
 * This function is used for informational text elements that do not respond to user input.
 * @param label Pointer to a constant Label object containing position, text, and color data.
 * @retval None
 */
static void Ui_DrawLabel(const Label *label);

/**
 * @brief Executes the action associated with the currently highlighted button.
 * @details This function is typically called in response to a long press event.
 * It retrieves the highlighted button from the current page and, if an
 * `onClick` callback is assigned, it invokes that function.
 * @retval None
 */
static void Ui_ExecuteAction();

/**
 * @brief Navigates the user interface to the sensors page.
 * @details This is a callback function assigned to a button's `onClick` handler.
 * It calls `Ui_SetCurrentPage` to display the `sensorsPage`.
 * @retval None
 */
static void Action_GoToSensors(Button *self);

/**
 * @brief Navigates the user interface to the settings page.
 * @details This is a callback function assigned to a button's `onClick` handler.
 * It calls `Ui_SetCurrentPage` to display the `settingsPage`.
 * @retval None
 */
static void Action_GoToSettings();

/**
 * @brief Navigates the user interface back to the home page.
 * @details This is a callback function for a "return" or "back" button.
 * It calls `Ui_SetCurrentPage` to display the `homePage`.
 * @retval None
 */
static void Action_GoBack();

/**
 * @brief Cycles to the next available color theme and applies it.
 * @details This function increments the theme index, wrapping around if necessary.
 * It then calls `Ui_ChangeMenuTheme` to apply the new colors to all UI elements
 * and redraw the screen.
 * @retval None
 */
static void Action_ChangeTheme();

/**
 * @brief Changes the system brightness to the next level.
 * @details Cycles through a predefined array of brightness levels
 * and calls the hardware abstraction layer to apply the new setting.
 */
static void Action_ChangeBrightness();

//   ------- Brightness ------

static const uint8_t brightnessLevels[] = {
		25,
		50,
		75,
		100
};

#define Num_Of_Brightness_Levels (sizeof(brightnessLevels) / sizeof(brightnessLevels[0]))

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
	.bgColor = BLUE,
	.onClick = Action_GoBack
};

//   ------- Sensors PAGE ------

static Label sensorsLabel1 ={
		.x = 7,
		.y = 15,
		.text = "Temperatura",
		.textColor = WHITE,
		.bgColor = BLACK,
};

static Label sensorsLabel2 ={
		.x = 7,
		.y = 40,
		.text = "Wilgotnosc",
		.textColor = WHITE,
		.bgColor = BLACK,
};

static Label* const sensorsLabels[] = {
	  &sensorsLabel1,
	  &sensorsLabel2,
};

static Button* const sensorsButtons[] = {
	  &returnButton,
};

#define Num_Of_Sensors_Labels (sizeof(sensorsLabels) / sizeof(sensorsLabels[0]))
#define Num_Of_Sensors_Buttons (sizeof(sensorsButtons) / sizeof(sensorsButtons[0]))

const Page sensorsPage = {
		.buttons = sensorsButtons,
		.buttonCount = Num_Of_Sensors_Buttons,

		.labels = sensorsLabels,
		.labelCount =Num_Of_Sensors_Labels,
};


//   ------- HOME PAGE ------

static Button homeButton1 ={
	.x = 25,
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
	  .x = 25,
	  .y = 50,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Temperatura",
	  .textColor = BLACK,
	  .bgColor = BLUE,
	  .onClick = Action_GoToSensors
};

static Button homeButton3 = {
	  .x = 25,
	  .y = 85,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Place holder",
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
	.x = 25,
	.y = 15,
	.width = BTN_DEFAULT_WIDTH,
	.height = BTN_DEFAULT_HEIGHT,
	.radius = BTN_DEFAULT_RADIUS,
	.text = "Motyw",
	.textColor = BLACK,
	.bgColor = BLUE,
	.onClick = Action_ChangeTheme
};

static Button settingsButton2={
	  .x = 25,
	  .y = 50,
	  .width = BTN_DEFAULT_WIDTH,
	  .height = BTN_DEFAULT_HEIGHT,
	  .radius = BTN_DEFAULT_RADIUS,
	  .text = "Jasnosc",
	  .textColor = BLACK,
	  .bgColor = BLUE,
	  .onClick = Action_ChangeBrightness
};

static Button settingsButton3 = {
	  .x = 25,
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

static void Action_ChangeBrightness(Button *self)
{
	currentBrightnesIndex = (currentBrightnesIndex + 1) % Num_Of_Brightness_Levels;

		HW_setBacklightBrightness(brightnessLevels[currentBrightnesIndex]);
}

static void Action_ChangeTheme(Button *self)
{
	currentThemeIndex = (currentThemeIndex + 1) % Num_Of_Themes;

	Ui_ChangeMenuTheme(themes[currentThemeIndex].textColor, themes[currentThemeIndex].bgColor);
}

static void Action_GoToSensors(Button *self){
	Ui_SetCurrentPage(&sensorsPage);
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

static void Ui_DrawLabel(const Label *label){
	lcdDrawText(label->x,
				label->y,
				label->text,
				label->textColor,
				label->bgColor);
}

void Ui_DrawPage(){

	if(currentPage == NULL) return;

	lcdFillBackground(BACKGROUND_COLOR);

	for(size_t i = 0; i < currentPage->labelCount; i++){
		Ui_DrawLabel(currentPage->labels[i]);
	}

	for(size_t i = 0; i < currentPage->buttonCount; i++)
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

void Ui_MoveHighlight(uint8_t dirDown)
{
    if (currentPage == NULL || currentPage->buttonCount == 0) return;

    if (dirDown)
    {
        currentButtonIndex++;
        if (currentButtonIndex >= currentPage->buttonCount)
            currentButtonIndex = 0;
    }
    else
    {
        if (currentButtonIndex == 0)
            currentButtonIndex = currentPage->buttonCount - 1;
        else
            currentButtonIndex--;
    }

    Ui_DrawPage();
}


void Ui_FSM_ShortPressActionDetected()
{
	Ui_ExecuteAction();
}

void Ui_FSM_LongPressActionDetected()
{
	Ui_ExecuteAction();
}

void Ui_MoveActionDetected(uint8_t dirDown)
{
	Ui_MoveHighlight(dirDown);
}






