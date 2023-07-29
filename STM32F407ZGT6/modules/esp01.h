#ifndef __ESP01_H
#define __ESP01_H

#include "main.h"
#include "sgp30.h"
#include "am2320.h"
#include "stm32f4xx_hal.h"
#include "uarthandle.h"

extern uint8_t esp01_single_buffer;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;



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
 * @brief Sends sensor data through TCP
 * 
 * @param message to send
 */
void sendTCPData(char* message);

/**
 * @brief Sends sensor data through UDP to specified IP address
 * 
 * @param ip_address to send data to
 * @param message to send
 */
void sendUDPData(char* ip_address, char* message);


/**
 * @brief Disconnect to any connnected Wifi
 * 
 */
void disconnectWifi();

#endif