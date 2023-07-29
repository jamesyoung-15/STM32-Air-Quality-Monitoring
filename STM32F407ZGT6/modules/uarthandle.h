#ifndef UARTHANDLE_H
#define UARTHANDLE_H

#include "main.h"
#include "esp01.h"
#include "esp32.h"

#define BUFFER_SIZE 1024


extern uint8_t esp32SingleBuffer;
extern UART_HandleTypeDef huart2;
extern char esp01_rx_buffer[BUFFER_SIZE];
extern uint8_t esp01_buffer_index;


/**
 * @brief Clears buffer
 * 
 */
void clearESP01Buffer();

/**
 * @brief Clears buffer
 * 
 */
void clearESP32Buffer();


/**
 * @brief Sends an AT command to ESP01 through UART
 * 
 * @param String of command to be sent to ESP01 
 */
void sendDataESP01(char* command);


/**
 * @brief Sets up stored buffer from ESP01 UART into a message to be printed. 
 * 
 */
void showESP01Response();

// /**
//  * @brief Shows buffer message to Serial monitor through another UART
//  * 
//  * @param UART used for debug logging
//  * @param message to be sent to debug UART
//  */
// void printDebug(UART_HandleTypeDef *huart, char* message);

/**
 * @brief Clears buffer
 * 
 */
void clearESP01Buffer();


#endif