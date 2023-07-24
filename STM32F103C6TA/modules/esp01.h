#ifndef __ESP01_H
#define __ESP01_H

#include "main.h"

extern uint8_t single_buffer;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


/**
 * @brief Sends an AT command to ESP01 through UART
 * 
 * @param UART handle that is connected to ESP01
 * @param String of command to be sent to ESP01 
 */
void sendCommand(UART_HandleTypeDef* huart, char* command);


/**
 * @brief Sets up stored buffer into a message to be printed. 
 * 
 */
void showResponse();

/**
 * @brief Shows buffer message to Serial monitor through another UART
 * 
 * @param UART used for debug logging
 * @param message to be sent to debug UART
 */
void printDebug(UART_HandleTypeDef *huart, char* message);


/**
 * @brief Clears buffer
 * 
 */
void clearBuffer();


/**
 * @brief Resets the ESP-01 module
 * 
 */
void resetESP01();


/**
 * @brief Check if module is working by sending basic "AT", expects response of "OK"
 * 
 */
void checkAT();

/**
 * @brief Command to turn off echo in ESP01 response
 * 
 */
void echoOff();

/**
 * @brief Checks ESP01 firmware version
 * 
 */
void checkESP01Version();

/**
 * @brief Connect to a WiFi
 * 
 * @param ssid of WiFi 
 * @param password of WiFi 
 */
void connectWifi(char* ssid, char* passwd);

/**
 * @brief Checks the IP address of device if connected to Wi-fi
 *
 */
void checkIPAddr();


/**
 * @brief Disconnect to any connnected Wifi
 * 
 */
void disconnectWifi();


#endif