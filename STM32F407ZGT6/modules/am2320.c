#include "am2320.h"




uint8_t getAM2320Data(I2C_HandleTypeDef* i2c_handler, float* temperature, float* humidity){
    // 0x03 to read register, 0x00 for reading registers, 0x04 for returning register
    uint8_t registers[3] = { 0x03, 0x00, 0x04 }; 
    uint8_t data[8] = {0};
    
    // init sensor
    HAL_I2C_Master_Transmit(i2c_handler, AM2320_ADDRESS, 0x00, 0, 100);
    HAL_Delay(1);
    // send data frame
    if(HAL_I2C_Master_Transmit(i2c_handler, AM2320_ADDRESS, registers, 3, 100)!=HAL_OK){
        // *temperature = 444;
        // *humidity = 444;
        return 1;
    }
    // receive response frame 
    if(HAL_I2C_Master_Receive(i2c_handler, AM2320_ADDRESS, data, 8, 100)!=HAL_OK){
        // *temperature = 555;
        // *humidity = 555;
        return 2;
    }

    // convert data to temp and humidity in celsius
    *temperature = (float) (data[5] | data[4] << 8) / 10.0;
	*humidity = (float) (data[3] | data[2] << 8) / 10.0;

    return 0;

}