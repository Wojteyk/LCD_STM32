/*
 * uart_connection.c
 *
 *  Created on: Nov 12, 2025
 *      Author: wojte
 */
#include "uart_connection.h"
#include <stdio.h>
#include "ui.h"
#include "usart.h"

uint8_t transmit_it_flag = 0;

static float temperature = 0;
static float humidity = 0;

static char txData[8];

void Uart_parseDHTData(char* line){

	if(sscanf(line, "T:%f;H:%f", &temperature, &humidity) ==2){
		Ui_UpdateDHTData(temperature, humidity);
	}
}

void Uart_sendPcState(uint8_t state)
{
	snprintf(txData, sizeof(txData), "PC:%s\r\n", state ? "On" : "Off");

	if(!transmit_it_flag){
		transmit_it_flag = 1;
		HAL_UART_Transmit_IT(&huart3, (uint8_t*)txData, strlen(txData));
	}
}
