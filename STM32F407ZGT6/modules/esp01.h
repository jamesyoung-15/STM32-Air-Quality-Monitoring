#ifndef __ESP01_H
#define __ESP01_H

#include "main.h"
#include "sgp30.h"
#include "am2320.h"
#include "stm32f4xx_hal.h"
#include "uarthandle.h"

extern uint8_t single_buffer;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Sends an AT command to ESP01 through UART
 * 
 * @param String of command to be sent to ESP01 
 */
void sendData(char* command);


/**
 * @brief Sets up stored buffer into a message to be printed. 
 * 
 */
void showResponse();

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
 * @brief Creates a TCP server, useful for HTTP server 
 *  
 */
void createTCPServer();


/**
 * @brief Creates a UDP server
 * 
 */
void createUDPServer();


/**
 * @brief Sends sensor data through UDP to specified IP address
 * 
 * @param ip_address to send data to
 * @param temperature from AM2320
 * @param humidity from AM2320
 * @param co2 from AM2320
 * @param tvoc from sgp30
 */
void sendUDPData(char* ip_address, float temperature,float humidity, uint16_t co2, uint16_t tvoc);


/**
 * @brief Disconnect to any connnected Wifi
 * 
 */
void disconnectWifi();

void sendTCPData(float temperature, float humidity);


#endif