/*
 * uart_connection.c
 *
 *  Created on: Nov 12, 2025
 *      Author: wojte
 */
#include "uart_connection.h"
#include <stdio.h>
#include "ui.h"

static float temperature = 0;
static float humidity = 0;

void parseDHTData(char* line){

	if(sscanf(line, "T:%f;H:%f", &temperature, &humidity) ==2){
		Ui_UpdateDHTData(temperature, humidity);
	}
}

