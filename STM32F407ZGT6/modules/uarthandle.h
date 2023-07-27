#ifndef UARTHANDLE_H
#define UARTHANDLE_H

#include "main.h"


/**
 * @brief Sends string to ESP32 through UART
 * 
 * @param UART used to connect to ESP32
 * @param message string to be sent through UART
 */
void sendToESP32(UART_HandleTypeDef *huart, char* message);


#endif