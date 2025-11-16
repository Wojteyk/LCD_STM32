/*
 * uart_connection.h
 *
 *  Created on: Nov 12, 2025
 *      Author: wojte
 */

#pragma once

#include "usart.h"

/**
 * @brief Flag indicating if a UART transmission is currently in progress.
 * @details This flag is set when HAL_UART_Transmit_IT is called and cleared
 * in the UART transmit complete callback. It prevents starting a new transmission
 * while a previous one is ongoing.
 */
extern uint8_t transmit_it_flag;

/**
 * @brief Parses a DHT sensor data line received over UART.
 * @details The input string should be in the format "T:<temperature>;H:<humidity>".
 * This function extracts the temperature and humidity values and updates the UI.
 *
 * @param line Null-terminated string containing the sensor data.
 */
void Uart_parseDHTData(char* line);

/**
 * @brief Sends the current PC state over UART.
 * @details This function formats the PC state as a string ("PC:On" or "PC:Off")
 * and transmits it using HAL_UART_Transmit_IT. It will only start the transmission
 * if no previous transmission is in progress.
 *
 * @param state The current PC state, where 0 = Off, 1 = On.
 */
void Uart_sendPcState(uint8_t state);
