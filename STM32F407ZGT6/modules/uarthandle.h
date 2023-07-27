#ifndef UARTHANDLE_H
#define UARTHANDLE_H

#include "main.h"


/**
 * @brief Shows buffer message to Serial monitor through another UART
 * 
 * @param UART used for debug logging
 * @param message to be sent to debug UART
 */
void printDebug(UART_HandleTypeDef *huart, char* message);


#endif