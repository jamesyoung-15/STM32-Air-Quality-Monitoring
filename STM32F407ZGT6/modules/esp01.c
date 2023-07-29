#include "esp01.h"




void checkAT(){
    sendDataESP01("AT\r\n");
    HAL_Delay(100);
    showESP01Response();
}

void resetESP01(){
    sendDataESP01("AT+CIPCLOSE=0\r\n");
    HAL_Delay(500);
    sendDataESP01("AT+CIPSERVER=0\r\n");
    HAL_Delay(500);
    sendDataESP01("AT+RST\r\n");
    HAL_Delay(12000);
    clearESP01Buffer();
}

void echoOff(){
    sendDataESP01("ATE0\r\n");
	HAL_Delay(100);
	clearESP01Buffer();
}

void checkESP01Version(){
    sendDataESP01("AT+GMR\r\n");
	HAL_Delay(200);
	showESP01Response();
}

void connectWifi(char* ssid, char* passwd){
	// set to station mode
	sendDataESP01("AT+CWMODE=1\r\n");
	// HAL_Delay(1000);
    showESP01Response();
    HAL_Delay(1000);

    // clearESP01Buffer();
    // Enabled by default no need 
    // enable station dhcp, so router can give it a ip
    // sendDataESP01("AT+CWDHCP=1,1\r\n");
    // HAL_Delay(200);
    // showESP01Response();

    // connect to wifi
    char wifi_info[100]={0};
    sprintf(wifi_info,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,passwd);
    sendDataESP01(wifi_info);
    HAL_Delay(5000);
    showESP01Response();
}

void disconnectWifi(){
    sendDataESP01("AT+CWQAP\r\n");
    HAL_Delay(200);
    showESP01Response();

}

void checkIPAddr(){
    sendDataESP01("AT+CIFSR\r\n");
    char temp[50] = {0};
    HAL_Delay(100);
    if(strstr(esp01_rx_buffer,"STAIP")!=NULL)
	{
		char *find = strstr(esp01_rx_buffer,"STAIP");
		int pos = find - esp01_rx_buffer;
		pos += 7;
		int i =0;
		while(esp01_rx_buffer[pos]!='"')
		{
			temp[i] = esp01_rx_buffer[pos];
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
	sprintf(esp01_rx_buffer,"IP: %s", temp);
	showESP01Response();
}


void createTCPServer()
{
    // allow multiple connections
    sendDataESP01("AT+CIPMUX=1\r\n");
    HAL_Delay(500);
    showESP01Response();
	// create tcp server, listening at port 80
	sendDataESP01("AT+CIPSERVER=1,80\r\n");
	HAL_Delay(200);
	showESP01Response();
}

void createUDPServer(){
    // allow multiple connections
    sendDataESP01("AT+CIPMUX=1\r\n");
    HAL_Delay(200);
    showESP01Response();


    sendToESP32(&huart2, "AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",4545,4545,2\n");
    // create udp server at port 4545
    sendDataESP01("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",4545,4545,2\r\n");
	HAL_Delay(3000);
	showESP01Response();
}

void sendUDPData(char* ip_address, char* message){
    sendToESP32(&huart2, "Sending data through UDP");

	char at_command[70]={0};
	sprintf(at_command,"AT+CIPSEND=0,%i,\"%s\",4545\r\n",strlen(message),ip_address);
	sendDataESP01(at_command);
	HAL_Delay(100);
	sendDataESP01(message);
	HAL_Delay(100);
    clearESP01Buffer();
}

void sendTCPData(char* message){
	char http_response[200] = {0};
	char at_command[50] = {0};
	// Create HTTP response, append relevant data to response
	sprintf(http_response,"HTTP/1.1 200 OK\r\nContent-Length: %i\r\nContent-Type: text/plain\r\n\r\n%s",strlen(message),message);
	// Send data through ESP8266
	sprintf(at_command,"AT+CIPSEND=0,%i\r\n",strlen(http_response));
	sendDataESP01(at_command);
	HAL_Delay(100);
	sendDataESP01(http_response);
	HAL_Delay(200);
	showESP01Response();
	sendDataESP01("AT+CIPCLOSE=0\r\n");
	clearESP01Buffer();
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//    if(huart->Instance==USART3){
//       if(buffer_index >= ESP01_BUFFER_SIZE){
//         clearESP01Buffer();
//         buffer_index=0;
//       }
//       else{
//         esp01_rx_buffer[buffer_index] = esp01_single_buffer;
//         buffer_index++;
//       }

//       if(buffer_index>6){
//         if(strstr(esp01_rx_buffer, "HTTP")){
//             sendToESP32(&huart2, "Requested website\n");
//             sendTCPData("Hello!\r\n");
//         }
//       }

//       HAL_UART_Receive_IT(&huart3, (uint8_t *)&esp01_single_buffer, 1);
//    }
// }