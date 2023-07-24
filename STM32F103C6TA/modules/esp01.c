#include "esp01.h"


#define BUFFER_SIZE 128

char rx_buffer[BUFFER_SIZE] = {0};
uint8_t single_buffer = 0;
uint8_t buffer_index = 0;

void sendCommand(UART_HandleTypeDef *huart, char* command){
    clearBuffer();
    HAL_UART_Transmit(huart, (uint8_t* )command, strlen(command), 5000);
}


void showResponse(){
    char temp[BUFFER_SIZE+20];
	sprintf(temp,"Buffer response: \n%s", rx_buffer);
	printDebug(&huart2, temp);
    clearBuffer();
}

void printDebug(UART_HandleTypeDef *huart, char* message){
    HAL_UART_Transmit(huart, (uint8_t* )message, strlen(message), 5000);
    clearBuffer();
}


void clearBuffer(){
    buffer_index = 0;
    memset(rx_buffer,0,sizeof(rx_buffer));
}


void checkAT(){
    sendCommand(&huart1, "AT\r\n");
    HAL_Delay(100);
    showResponse();
}

void resetESP01(){
    sendCommand(&huart1,"AT+RST\r\n");
    HAL_Delay(4000);
    clearBuffer();
}

void echoOff(){
    sendCommand(&huart1, "ATE0\r\n");
	HAL_Delay(100);
	clearBuffer();
}

void checkESP01Version(){
    sendCommand(&huart1,"AT+GMR\r\n");
	HAL_Delay(200);
	showResponse();
}

void connectWifi(char* ssid, char* passwd){
	// set to station mode
	sendCommand(&huart1,"AT+CWMODE=1\r\n");
	HAL_Delay(200);
    //	showResponse();

    clearBuffer();
    // Enabled by default no need 
    // enable station dhcp, so router can give it a ip
    //	sendCommand("AT+CWDHCP=1,1\r\n");
    //	HAL_Delay(200);

    // connect to wifi
    char wifi_info[100]={0};
    sprintf(wifi_info,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,passwd);
    sendCommand(&huart1,wifi_info);
    HAL_Delay(5000);
    showResponse();
}

void disconnectWifi(){
    sendCommand(&huart1,"AT+CWQAP\r\n");
    HAL_Delay(200);
    showResponse();

}

void checkIPAddr(){
    sendCommand(&huart1, "AT+CIFSR\r\n");
    char temp[50] = {0};
    HAL_Delay(100);
    if(strstr(rx_buffer,"STAIP")!=NULL)
	{
		char *find = strstr(rx_buffer,"STAIP");
		int pos = find - rx_buffer;
		pos += 7;
		int i =0;
		while(rx_buffer[pos]!='"')
		{
			temp[i] = rx_buffer[pos];
			pos++;
			i++;
		}
	}
	else
	{
		sprintf(temp,"Error Reading IP.");
	}
	if(strstr(temp,"0.0.0.0")!=NULL)
	{
		sprintf(temp,"Error connecting.");
	}
	sprintf(rx_buffer,"IP: %s", temp);
	showResponse();
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance==USART1){
      if(buffer_index >= 128){
        clearBuffer();
      }
      else{
        rx_buffer[buffer_index] = single_buffer;
        buffer_index++;
      }

   }
   HAL_UART_Receive_IT(&huart1, (uint8_t *)&single_buffer, 1);
}