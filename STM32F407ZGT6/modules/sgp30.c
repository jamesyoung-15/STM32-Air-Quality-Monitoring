#include "sgp30.h"




uint8_t initSGP30(I2C_HandleTypeDef* i2c_handler){
    // Reset the module
    uint8_t cmd = 0x06;
    if(HAL_I2C_Master_Transmit(i2c_handler, 0x00, &cmd, 1, 0xFFFF)!=HAL_OK){
        return 1;
    };

    HAL_Delay(100);

    // Init the sensor to air quality
    uint8_t inithex[2] = {0x20, 0x03};
    if(HAL_I2C_Master_Transmit(i2c_handler, SGP30_ADDRESS_WRITE, (uint8_t*)inithex, 2, 0xFFFF)!=HAL_OK){
        return 3;
    }
    HAL_Delay(100);

    return 0;
}


uint8_t getSGP30Data(I2C_HandleTypeDef* i2c_handler, uint16_t* co2, uint16_t* tvoc){

    uint8_t readhex[2] = {0x20, 0x08};
    uint8_t data[6] = {0};
    
    // send command to measure air quality
    if(HAL_I2C_Master_Transmit(i2c_handler, SGP30_ADDRESS_WRITE, (uint8_t*)readhex, 2, 0xFFFF)!=HAL_OK){
        return 1;
    }

    HAL_Delay(100);

    // receive response frame
    if(HAL_I2C_Master_Receive(i2c_handler, SGP30_ADDRESS_READ, (uint8_t*)data, 6, 0xFFFF)!=HAL_OK){
        return 2;
    }

    // combine data to 16 bit and store in variable
    *co2 = (data[0] << 8) | data[1];
    *tvoc = (data[3] << 8) | data[4];

    return 0;
}



// sgp30_data_t sgp30_data;


// static uint8_t sgp30_send_cmd(sgp30_cmd_t cmd)
// {
//     uint8_t cmd_buffer[2];
//     cmd_buffer[0] = cmd >> 8;
//     cmd_buffer[1] = cmd;
//     return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, SGP30_ADDR_WRITE, (uint8_t*) cmd_buffer, 2, 0xFFFF);
// }


// static int sgp30_soft_reset(void)
// {
//     uint8_t cmd = 0x06;
//     return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, 0x00, &cmd, 1, 0xFFFF);
// }


// int sgp30_init(void)
// {
//     int status;
    
//     status = sgp30_soft_reset();
//     if (status != HAL_OK) {
//         return -1;
//     }
    
//     HAL_Delay(100);
    
//     status = sgp30_send_cmd(INIT_AIR_QUALITY);
    
//     HAL_Delay(100);
    
//     return status == 0 ? 0 : -1;
// }


// static int sgp30_start(void)
// {
//     return sgp30_send_cmd(MEASURE_AIR_QUALITY);
// }

// #define CRC8_POLYNOMIAL 0x31

// static uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
// {
//     uint8_t  remainder;	    //余数
//     uint8_t  i = 0, j = 0;  //循环变量

//     /* 初始化 */
//     remainder = initial_value;

//     for(j = 0; j < 2;j++)
//     {
//         remainder ^= message[j];

//         /* 从最高位开始依次计算  */
//         for (i = 0; i < 8; i++)
//         {
//             if (remainder & 0x80)
//             {
//                 remainder = (remainder << 1)^CRC8_POLYNOMIAL;
//             }
//             else
//             {
//                 remainder = (remainder << 1);
//             }
//         }
//     }

//     /* 返回计算的CRC码 */
//     return remainder;
// }


// int sgp30_read(void)
// {
//     int status;
//     uint8_t recv_buffer[6]={0};
    
//     /* 启动测量 */
//     status = sgp30_start();
//     if (status != 0) {
//         printf("sgp30 start fail\r\n");
//         return -1;
//     }
    
//     HAL_Delay(100);
    
//     /* 读取测量数据 */
//     status = HAL_I2C_Master_Receive(&SGP30_I2C_Handle_Name, SGP30_ADDR_READ, (uint8_t*)recv_buffer, 6, 0xFFFF);
//     if (status != HAL_OK) {
//         printf("I2C Master Receive fail\r\n");
//         return -1;
//     }
    
//     /* 校验接收的测量数据 */
//     if (CheckCrc8(&recv_buffer[0], 0xFF) != recv_buffer[2]) {
//         printf("co2 recv data crc check fail\r\n");
//         return -1;
//     }
//     if (CheckCrc8(&recv_buffer[3], 0xFF) != recv_buffer[5]) {
//         printf("tvoc recv data crc check fail\r\n");
//         return -1;
//     }
    
    
//     /* 转换测量数据 */
//     sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
//     sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];
    
//     return 0;
// }