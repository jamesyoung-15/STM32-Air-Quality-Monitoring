#ifndef AM2320_H
#define AM2320_H


#include "main.h"

// 0x5C>>1 (left shifted by 1 bit)
#define AM2320_ADDRESS 0xb8

/**
 * @brief Get the temperature (celsius) and humidity (percentage) data of the AM2320 through I2C
 * 
 * @param i2c_handler Pointer to I2C handle eg. hi2c1 
 * @param temperature Pointer to temperature variable to store in
 * @param humidity Pointer to humidity variable to store in
 * 
 * @return Returns 0 if no error, otherwise 1 means error transmitting and 2 means error reading
 */
uint8_t getAM2320Data(I2C_HandleTypeDef* i2c_handler, float* temperature, float* humidity);

#endif