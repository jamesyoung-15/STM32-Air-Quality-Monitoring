#include "uarthandle.h"


void printDebug(UART_HandleTypeDef *huart, char* message){
    HAL_UART_Transmit(huart, (uint8_t* )message, strlen(message), 5000);
}