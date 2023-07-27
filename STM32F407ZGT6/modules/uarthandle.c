#include "uarthandle.h"


void sendToESP32(UART_HandleTypeDef *huart, char* message){
    HAL_UART_Transmit(huart, (uint8_t* )message, strlen(message), 5000);
}


