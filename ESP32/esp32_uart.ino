#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "time.h"
#include <WebServer.h>
#include <ESPmDNS.h>

// UART2 pins
#define UART_RX2 16
#define UART_TX2 17

// Interval to test connection
#define INTERVAL 60000

// For interval tracking
unsigned long previousMillis = 0; 

// Wifi credentials.
const char *ssid = "YangFamily";
const char *password = "yang27764892";

// Port to listen on for HTTP server
WebServer server(80);


// For getting current NTP time, using HKT (GMT+8)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;

// Message to send
String message;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // UART Setup
  Serial2.begin(115200, SERIAL_8N1, UART_RX2, UART_TX2);

  // Wifi Setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("\nIP Address: ");
  Serial.println(WiFi.localIP());


  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }


  // Configure HTTP server
  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // configure NTP time server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {
  // put your main code here, to run repeatedly:

  float temperature = 25.5;
  float humidity = 55.5;
  uint16_t co2 = 404;
  uint16_t tvoc = 0; 
  String time = "Failed to obtain time";

  // Reading UART data
  while(Serial2.available()){
    String sensorData = Serial2.readString();
    // Serial.println(sensorData);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, sensorData);
    temperature = doc["Temperature"];
    humidity = doc["Humidity"];
    co2 = doc["CO2"];
    tvoc = doc["TVOC"];
    char timeNow[50];
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      sprintf(timeNow, "Failed to obtain time");
    }
    else{
        strftime(timeNow,50, "%A, %B %d %Y %H:%M:%S", &timeinfo);
    }
    doc["time"] = timeNow;
    message = "";
    serializeJsonPretty(doc, message);

    Serial.println(message);

  }

  // Periodically check if ESP32 is connected to Wifi
  unsigned long currentMillis = millis();
  checkConnection(currentMillis);

  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}


/**
 * @brief Sends the JSON message to web server on root HTTP request
 * 
 */
void handleRoot() {
  Serial.println("Sending website");

  server.send(200, "text/plain", message);
}

/**
 * @brief Handles 404 error
 * 
 */
void handleNotFound(){
    String message = "404 Error!";
    server.send(404, "text/plain", message);
}

/**
 * @brief Periodically check if ESP32 is still connected to Wifi, if not then attempt to reconnect
 * 
 * @param currentMillis is the counter in the while loop
 */
void checkConnection(unsigned long currentMillis){
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=INTERVAL)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}
