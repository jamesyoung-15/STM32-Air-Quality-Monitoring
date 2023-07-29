#include "uarthandle.h"


char esp32RxBuffer[BUFFER_SIZE] = {0};
uint8_t esp32SingleBuffer = 0;
uint8_t esp32BufferIndex = 0;


char esp01_rx_buffer[BUFFER_SIZE] = {0};
uint8_t esp01_single_buffer = 0;
uint8_t esp01_buffer_index = 0;

void sendDataESP01(char* command){
    clearESP01Buffer();
    HAL_UART_Transmit(&huart3, (uint8_t* )command, strlen(command), 5000);
    // while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX){};
}

void sendToESP32(UART_HandleTypeDef *huart, char* message){
    HAL_UART_Transmit(huart, (uint8_t* )message, strlen(message), 5000);
}

void showESP01Response(){
    char temp[BUFFER_SIZE+25];
	sprintf(temp,"Buffer response: \n%s\n", esp01_rx_buffer);
	sendToESP32(&huart2, temp);
    clearESP01Buffer();
}

void clearESP32Buffer(){
    esp32BufferIndex = 0;
    memset(esp32RxBuffer,0,sizeof(esp32RxBuffer));
}

void clearESP01Buffer(){
    esp01_buffer_index = 0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // UART for ESP01
    // if(huart->Instance==USART3){
    //     if(esp01_buffer_index >= BUFFER_SIZE){
    //         clearESP01Buffer();
    //         esp01_buffer_index=0;
    //     }
    //     else{
    //         esp01_rx_buffer[esp01_buffer_index] = esp01_single_buffer;
    //         esp01_buffer_index++;
    //     }


    //     HAL_UART_Receive_IT(&huart3, (uint8_t *)&esp01_single_buffer, 1);
    // }

    // UART for ESP32
    if(huart->Instance==USART2){
        if(esp32BufferIndex >= BUFFER_SIZE){
            clearESP32Buffer();
            esp32BufferIndex=0;
        }
        else{
            esp32RxBuffer[esp32BufferIndex] = esp32SingleBuffer;
            esp32BufferIndex++;
        }

        if(esp32BufferIndex>=4){
            if(strstr(esp32RxBuffer, "zgf")!=NULL){
                uint16_t length = strlen(esp32RxBuffer);
                for(uint8_t i=1;i<4;i++){
                    esp32RxBuffer[length-i] = '\0';
                }
                drawESP32IP(esp32RxBuffer);
                clearESP32Buffer();
            }
            else if(strstr(esp32RxBuffer,"asd")!=NULL){
                uint16_t length = strlen(esp32RxBuffer);
                for(uint8_t i=1;i<4;i++){
                    esp32RxBuffer[length-i] = '\0';
                }
                drawESP32Time(esp32RxBuffer);
                clearESP32Buffer();
            }

        }
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&esp32SingleBuffer, 1);
    }
}