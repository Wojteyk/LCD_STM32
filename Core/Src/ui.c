/*
 * ui.c
 *
 *  Created on: Oct 10, 2025
 *      Author: wojte
 */
#include "ui.h"
#include "uart_connection.h"

// --- Static Global Variables ---

/// @brief Pointer to the currently displayed page.
static const Page *currentPage = NULL;
/// @brief Index of the currently highlighted button on the currentPage.
static int currentButtonIndex = 0;
/// @brief Index of the currently active color theme.
static int currentThemeIndex = 0;
/// @brief Index of the currently selected brightness level.
static int currentBrightnesIndex = 0;

static char bufTemperature[8] = "25.4";
static char bufHumidity[8] = "30";
static char bufPc[8] = "Off";

static uint8_t pcState = 0;

//   ------- Function declarations ------

/**
 * @brief Toggles the PC state between ON and OFF.
 *
 * This function is intended to be used as a callback for a button press.
 * When executed, it switches the PC state, updates the corresponding dynamic
 * label on the UI, and sends the new state over UART.
 *
 * @param self Pointer to the Button structure that triggered this action.
 */
static void Action_TogglePc(Button *self);

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
static void Ui_DrawLabel_Const(const Label_Const *label);

/**
 * @brief Draws a text label on the screen.
 * @details Displays static text at a fixed position with dynamic text and background colors.
 * This function is used for dynamic text elements.
 * @param label Pointer to a constant Label object containing position, text, and color data.
 * @retval None
 */
static void Ui_DrawLabel_Dynamic(Label_Dynamic *label);

/**
 * @brief Executes the action associated with the currently highlighted button.
 * @details This function is typically called in response to a long press event.
 * It retrieves the highlighted button from the current page and, if an
 * `onClick` callback is assigned, it invokes that function.
 * @retval None
 */
static void Ui_ExecuteAction();

/**
 * @brief Navigates the user interface to the controls page.
 * @details This is a callback function assigned to a button's `onClick` handler.
 * It calls `Ui_SetCurrentPage` to display the `sensorsPage`.
 * @retval None
 */
static void Action_GoToControls(Button *self);

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

//   ------- Controls PAGE ------
static const Label_Const controlsLabelConst1 ={
		.x = 5,
		.y = 15,
		.text = "Komputer",
		.textColor = WHITE,
		.bgColor = BLACK,
};

static Label_Dynamic controlsLabelDynamic1 ={
		.x = 108,
		.y = 15,
		.text = "1",
		.textColor = WHITE,
		.bgColor = BLACK,
		.dataPtr = bufPc,
};

static Button controlsButton1 ={
	.x = 25,
	.y = 35,
	.width = BTN_DEFAULT_WIDTH,
	.height = BTN_DEFAULT_HEIGHT,
	.radius = BTN_DEFAULT_RADIUS,
	.text = "Przelacz",
	.textColor = BLACK,
	.bgColor = BLUE,
	.onClick = Action_TogglePc
};

static const Label_Const* const controlsLabelsConst[] = {
	  &controlsLabelConst1,
};

static Label_Dynamic* const controlsLabelsDynamic[] = {
	  &controlsLabelDynamic1,
};

static Button* const controlsButtons[] = {
		&controlsButton1,
		&returnButton,
};

#define Num_Of_Controls_Const_Labels (sizeof(controlsLabelsConst) / sizeof(controlsLabelsConst[0]))
#define Num_Of_Controls_Dynamic_Labels (sizeof(controlsLabelsDynamic) / sizeof(controlsLabelsDynamic[0]))
#define Num_Of_Controls_Buttons (sizeof(controlsButtons) / sizeof(controlsButtons[0]))

const Page controlsPage = {
		.buttons = controlsButtons,
		.buttonCount = Num_Of_Controls_Buttons,

		.labels_Const = controlsLabelsConst,
		.label_Const_Count =Num_Of_Controls_Const_Labels,

		.labels_Dynamic = controlsLabelsDynamic,
		.label_Dynamic_Count =Num_Of_Controls_Dynamic_Labels,
};

//   ------- Sensors PAGE ------

static const Label_Const sensorsLabelConst1 ={
		.x = 5,
		.y = 15,
		.text = "Temperatura",
		.textColor = WHITE,
		.bgColor = BLACK,
};

static const Label_Const sensorsLabelConst2 ={
		.x = 5,
		.y = 40,
		.text = "Wilgotnosc",
		.textColor = WHITE,
		.bgColor = BLACK,
};

static Label_Dynamic sensorsLabelDynamic1 ={
		.x = 108,
		.y = 15,
		.text = "1",
		.textColor = WHITE,
		.bgColor = BLACK,
		.dataPtr = bufTemperature,
};

static  Label_Dynamic sensorsLabelDynamic2 ={
		.x = 105,
		.y = 40,
		.text = "2",
		.textColor = WHITE,
		.bgColor = BLACK,
		.dataPtr = bufHumidity,
};

static const Label_Const* const sensorsLabelsConst[] = {
	  &sensorsLabelConst1,
	  &sensorsLabelConst2,
};

static Label_Dynamic* const sensorsLabelsDynamic[] = {
	  &sensorsLabelDynamic1,
	  &sensorsLabelDynamic2,
};

static Button* const sensorsButtons[] = {
	  &returnButton,
};

#define Num_Of_Sensors_Const_Labels (sizeof(sensorsLabelsConst) / sizeof(sensorsLabelsConst[0]))
#define Num_Of_Sensors_Dynamic_Labels (sizeof(sensorsLabelsDynamic) / sizeof(sensorsLabelsDynamic[0]))
#define Num_Of_Sensors_Buttons (sizeof(sensorsButtons) / sizeof(sensorsButtons[0]))

const Page sensorsPage = {
		.buttons = sensorsButtons,
		.buttonCount = Num_Of_Sensors_Buttons,

		.labels_Const = sensorsLabelsConst,
		.label_Const_Count =Num_Of_Sensors_Const_Labels,

		.labels_Dynamic = sensorsLabelsDynamic,
		.label_Dynamic_Count =Num_Of_Sensors_Dynamic_Labels,
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
	  .text = "Sterowanie",
	  .textColor = BLACK,
	  .bgColor = BLUE,
	  .onClick = Action_GoToControls
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

//   ------- PAGES ------

const Page* const pages[] = {
		&settingsPage,
		&controlsPage,
		&homePage,
		&sensorsPage,
};

#define Num_Of_Pages (sizeof(pages) / sizeof(pages[0]))

// ------------------------------------------------------

static void Action_ChangeBrightness(Button *self)
{
	currentBrightnesIndex = (currentBrightnesIndex + 1) % Num_Of_Brightness_Levels;

		HW_setBacklightBrightness(brightnessLevels[currentBrightnesIndex]);
}

static void Action_TogglePc(Button *self)
{
	pcState = ! pcState;
	snprintf(bufPc, sizeof(bufPc), "%s", pcState ? "On " : "Off");
	Uart_sendPcState(pcState);
	Ui_DrawLabel_Dynamic(&controlsLabelDynamic1);
	lcdCopy();
}

static void Action_ChangeTheme(Button *self)
{
	currentThemeIndex = (currentThemeIndex + 1) % Num_Of_Themes;

	Ui_ChangeMenuTheme(themes[currentThemeIndex].textColor, themes[currentThemeIndex].bgColor);
}

static void Action_GoToSensors(Button *self){
	Ui_SetCurrentPage(&sensorsPage);
}

static void Action_GoToControls(Button *self){
	Ui_SetCurrentPage(&controlsPage);
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

static void Ui_DrawLabel_Const(const Label_Const *label){
	lcdDrawText(label->x,
				label->y,
				label->text,
				label->textColor,
				label->bgColor);
}

static void Ui_DrawLabel_Dynamic(Label_Dynamic *label){

    const char *textToDraw = label->dataPtr ? label->dataPtr : label->text;
    lcdDrawText(label->x, label->y, textToDraw, label->textColor, label->bgColor);
}

void Ui_DrawPage(){

	if(currentPage == NULL) return;

	lcdFillBackground(BACKGROUND_COLOR);

	for(size_t i = 0; i < currentPage->label_Const_Count; i++){
		Ui_DrawLabel_Const(currentPage->labels_Const[i]);
	}

	for(size_t i = 0; i < currentPage->label_Dynamic_Count; i++){
		Ui_DrawLabel_Dynamic(currentPage->labels_Dynamic[i]);
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

	for(int page = 0; page < Num_Of_Pages; page++){
		for(int btn = 0; btn < Num_Of_Menu_Buttons; btn++ )
		{
			pages[page]->buttons[btn]->textColor = changedTextColor;
			pages[page]->buttons[btn]->bgColor = changedBgColor;
		}
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

void Ui_UpdateDHTData(float temperature, float humidity)
{
    snprintf(bufTemperature, sizeof(bufTemperature), "%.1fC", temperature);
    snprintf(bufHumidity, sizeof(bufHumidity), "%.1f%%", humidity);

    if(currentPage == &sensorsPage){
        Ui_DrawLabel_Dynamic(&sensorsLabelDynamic1);
        Ui_DrawLabel_Dynamic(&sensorsLabelDynamic2);
        lcdCopy();
    }
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






