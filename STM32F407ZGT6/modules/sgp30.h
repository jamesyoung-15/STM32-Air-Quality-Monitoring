#ifndef SGP30_H
#define SGP30_H


#include "main.h"

#define SGP30_ADDRESS 0x58
// write address is 0x58 << 1 = 0xb0, read address is 0x58 << 1 + 1 = 0xb1
#define SGP30_ADDRESS_WRITE (SGP30_ADDRESS << 1) // 0xb0
#define SGP30_ADDRESS_READ ((SGP30_ADDRESS << 1) + 1) //0xb1
#define CRC8_POLYNOMIAL 0x31

/**
 * @brief Init sensor by resetting module then initializing to air quality mode
 * 
 * @param i2c_handler Pointer to I2C handle eg. hi2c1 
 * @return Returns 0 if no error, otherwise 1 means error transmitting and 2 means error reading
 */
uint8_t initSGP30(I2C_HandleTypeDef* i2c_handler);

/**
 * @brief Reads
 * 
 * @param i2c_handler Pointer to I2C handle eg. hi2c1 
 * @param co2 Pointer to float variable to store CO2 data
 * @param tvoc Pointer to float variable to store TVOC data
 * 
 * 
 * @return Returns 0 if no error, otherwise 1 means error transmitting and 2 means error reading
 */
uint8_t getSGP30Data(I2C_HandleTypeDef* i2c_handler, uint16_t* co2, uint16_t* tvoc);


#endif