#include "esp01.h"


#define BUFFER_SIZE 1024

char rx_buffer[BUFFER_SIZE] = {0};
uint8_t single_buffer = 0;
uint8_t buffer_index = 0;

void sendData(char* command){
    clearBuffer();
    HAL_UART_Transmit(&huart3, (uint8_t* )command, strlen(command), 5000);
    // while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX){};
}


void showResponse(){
    char temp[BUFFER_SIZE+25];
	sprintf(temp,"Buffer response: \n%s\n", rx_buffer);
	printDebug(&huart2, temp);
    clearBuffer();
}

// void printDebug(UART_HandleTypeDef *huart, char* message){
//     HAL_UART_Transmit(huart, (uint8_t* )message, strlen(message), 5000);
//     clearBuffer();
// }


void clearBuffer(){
    buffer_index = 0;
    memset(rx_buffer,0,sizeof(rx_buffer));
}


void checkAT(){
    sendData("AT\r\n");
    HAL_Delay(100);
    showResponse();
}

void resetESP01(){
    sendData("AT+RST\r\n");
    HAL_Delay(12000);
    clearBuffer();
}

void echoOff(){
    sendData("ATE0\r\n");
	HAL_Delay(100);
	clearBuffer();
}

void checkESP01Version(){
    sendData("AT+GMR\r\n");
	HAL_Delay(200);
	showResponse();
}

void connectWifi(char* ssid, char* passwd){
	// set to station mode
	sendData("AT+CWMODE=1\r\n");
	// HAL_Delay(1000);
    showResponse();
    HAL_Delay(1000);

    // clearBuffer();
    // Enabled by default no need 
    // enable station dhcp, so router can give it a ip
    // sendData("AT+CWDHCP=1,1\r\n");
    // HAL_Delay(200);
    // showResponse();

    // connect to wifi
    char wifi_info[100]={0};
    sprintf(wifi_info,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,passwd);
    sendData(wifi_info);
    HAL_Delay(5000);
    showResponse();
}

void disconnectWifi(){
    sendData("AT+CWQAP\r\n");
    HAL_Delay(200);
    showResponse();

}

void checkIPAddr(){
    sendData("AT+CIFSR\r\n");
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


void createTCPServer()
{
    // allow multiple connections
    sendData("AT+CIPMUX=1\r\n");
    HAL_Delay(500);
    showResponse();
	// create tcp server, listening at port 80
	sendData("AT+CIPSERVER=1,80\r\n");
	HAL_Delay(200);
	showResponse();
}

void createUDPServer(){
    printDebug(&huart2, "AT+CIPMUX=1\n");
    // allow multiple connections
    sendData("AT+CIPMUX=1\r\n");
    HAL_Delay(200);
    showResponse();


    printDebug(&huart2, "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",4545,4545,2\n");
    // create udp server at port 4545
    sendData("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",4545,4545,2\r\n");
	HAL_Delay(3000);
	showResponse();
}

void sendUDPData(char* ip_address, float temperature, float humidity, uint16_t co2, uint16_t tvoc){
    printDebug(&huart2, "Sending data through UDP");
	// float temperature, humidity;
	char data[80] = {0};

    // uint8_t am2320_status = getAM2320Data(&hi2c1, &temperature, &humidity);
    sprintf(data,"\nTemperature: %.2f °C\nHumidity: %.2f %%\nCO2: %i ppm\nTVOC: %i ppb\n",temperature, humidity, co2, tvoc);
    printDebug(&huart2, data);

	char at_command[70]={0};
	sprintf(at_command,"AT+CIPSEND=0,%i,\"%s\",4545\r\n",strlen(data),ip_address);
	sendData(at_command);
	HAL_Delay(100);
	sendData(data);
	HAL_Delay(100);
    clearBuffer();
}

void sendTCPData(float temperature, float humidity){
    printDebug(&huart2, "Sending data through TCP\n");
	// float temperature, humidity;
	char data[512] = {0};

    // uint8_t am2320_status = getAM2320Data(&hi2c1, &temperature, &humidity);
    sprintf(data,"H");
    

	char at_command[70]={0};
	sprintf(at_command,"AT+CIPSEND=0,%i\r\n",strlen(data)-1);
	sendData(at_command);
    printDebug(&huart2, at_command);
	HAL_Delay(100);
	sendData(data);
    printDebug(&huart2, data);
	HAL_Delay(100);
    clearBuffer();
    HAL_Delay(100);
	sendData("AT+CIPCLOSE=0\r\n");
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance==USART3){
      if(buffer_index >= BUFFER_SIZE){
        clearBuffer();
        buffer_index=0;
      }
      else{
        rx_buffer[buffer_index] = single_buffer;
        buffer_index++;
      }

      if(buffer_index>4){
        if(strstr(rx_buffer, "html")){
            sendTCPData(25.5,55.5);
        }
      }

    
   }
   HAL_UART_Receive_IT(&huart3, (uint8_t *)&single_buffer, 1);
}