/* Arduino Libraries */
#include "ArduinoJson.h"
#include "base64.h"
#include <WiFiClient.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "WiFi.h"
#include <ESPmDNS.h>
#include "string.h"
#include <HTTPClient.h>

// sensor libs
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include <DHT_U.h>
#include "Adafruit_SGP30.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* My includes */
#include "html.h"
#include "secrets.h"

// Instantiate class variables
AsyncWebServer server(80);
WiFiClientSecure client;

// UART2 pins
#define UART_RX2 16
#define UART_TX2 17

// wifi check and send data interval
unsigned long intervalMillis = 10*60000;
unsigned long previousMillis = 0;

String api_url = "https://8cdgfq0r3c.execute-api.us-east-1.amazonaws.com/TestV01/sensor-data";

// DHT config
// const int DHTPIN = 26;
// #define DHTTYPE    DHT11 
// DHT_Unified dht(DHTPIN, DHTTYPE);

// structs for sensor data
struct dhtData{
    float temperature;
    float humidity;
    bool readSucess = false;
    dhtData(float temperature,float humidity,bool readSucess):temperature(temperature), humidity(humidity), readSucess(readSucess){}
};

// // sgp30 config
// Adafruit_SGP30 sgp;

struct sgp30Data{
    uint16_t tvoc; // ppb
    uint16_t eco2; // ppm
    bool readSuccess = false;
    sgp30Data(uint16_t tvoc, uint16_t eco2, bool readSuccess):tvoc(tvoc), eco2(eco2), readSuccess(readSuccess){}
};

// // 0.96 i2c display config
// #define WIRE Wire
// Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);



// time config, using HKT (GMT+8)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;
// interval to update time
unsigned long timePreviousMillis = 0;
unsigned long timeIntervalMillis = 60000;

// dhtData readDHT(){
//     bool readSucess = false;
//     float temperature = 0;
//     float humidity = 0;
//     sensors_event_t event;
//     dht.temperature().getEvent(&event);
//     if (isnan(event.temperature)) {
//         Serial.println(F("Error reading temperature!"));
//     }
//     else {
//         Serial.print(F("Temperature: "));
//         Serial.print(event.temperature);
//         Serial.println(F("°C"));
//         temperature = event.temperature;
//     }
//     // Get humidity event and print its value.
//     dht.humidity().getEvent(&event);
//     if (isnan(event.relative_humidity)) {
//         Serial.println(F("Error reading humidity!"));
//     }
//     else {
//         Serial.print(F("Humidity: "));
//         Serial.print(event.relative_humidity);
//         Serial.println(F("%"));
//         humidity = event.relative_humidity;
//     }

//     return {temperature, humidity, false};
// }

// sgp30Data readSGP30(){
//     bool readSucess = false;
//     uint16_t tvoc = 0;
//     uint16_t eco2 = 0;
//     if (! sgp.IAQmeasure()) {
//         Serial.println("SGP30 Measurement failed");
//     }
//     else{
//         tvoc = sgp.TVOC;
//         eco2 = sgp.eCO2;
//         Serial.print("TVOC "); Serial.print(tvoc); Serial.print(" ppb\t");
//         Serial.print("eCO2 "); Serial.print(eco2); Serial.println(" ppm");
//     }
//     return {tvoc, eco2, readSucess};
// }

// void printToDisplay(){
//     // read sensors
//     dhtData dhtdata = readDHT(); 
//     sgp30Data sgp30Data = readSGP30();
//     // retrieve time
//     // char timeNow[50];
//     // struct tm timeinfo;
//     // if(!getLocalTime(&timeinfo)){
//     //     sprintf(timeNow, "Failed to obtain time");
//     // }
//     // else{
//     //     strftime(timeNow,50, "%H:%M", &timeinfo);
//     // }

//     char tempBuf[50];
//     // display info
//     display.clearDisplay();
//     display.setTextColor(1);
//     // display time
//     // display.setCursor(0,0);
//     // display.print(timeNow);

//     // display temperature
//     display.setCursor(0,0);
//     sprintf(tempBuf, "Temperature: %.1f C", dhtdata.temperature);
//     display.print(tempBuf);
    
//     // display humidity
//     display.setCursor(0, 15);
//     sprintf(tempBuf, "Humidity: %.1f %%", dhtdata.humidity);
//     display.print(tempBuf);

//     // display tvoc


//     // display eco2
    
//     display.display();
// }

// send data to AWS API through HTTP
void sendDataAWS(){
    // check wifi
    if(WiFi.status() != WL_CONNECTED){
            Serial.println("Cannot send: Not connected to Wifi!");
            return;
    }
    else{
        
        // dhtData dhtdata = readDHT();
        // sgp30Data sgp30data = readSGP30();
        float temperature = 25.5;
        float humidity = 55.5;
        uint16_t co2 = 404;
        uint16_t tvoc = 0; 
        String time = "Failed to obtain time";

        // Reading UART data
        while(Serial2.available()){
            // store uart data to string
            String stmData = Serial2.readString();
            // if data is json form, it means it contains sensor data, deconstruct it
            if(stmData.indexOf("{")!=-1){
                Serial.println("\nData received from STM32: \n"+stmData+"\n");
                DynamicJsonDocument doc(1024);
                deserializeJson(doc, stmData);
                temperature = doc["Temperature"];
                humidity = doc["Humidity"];
                co2 = doc["CO2"];
                tvoc = doc["TVOC"];
                message = "";
                Serial.println("Reformatted data in JSON: \n");
                serializeJsonPretty(doc, message);

                Serial.println(message);

                Serial.print("\nGo to IP address: "); Serial.print(WiFi.localIP()); Serial.println(" to view data.\n");
            }
            // otherwise just some debug or other message
            else{
                Serial.println("Debug message: \n"+stmData);
            }
            
        }



        // create json with data
        StaticJsonDocument<200> doc;
        doc["Temperature"] = temperature;
        doc["Humidity"] = humidity;
        doc["TVOC"] = tvoc;
        doc["eCO2"] = eco2;
        doc["Location"] = "James' Room";
        // convert arduino json to string
        String jsonData;
        serializeJson(doc, jsonData);

        Serial.println(jsonData);

        // create http instance, connect to aws
        HTTPClient http;
        Serial.println("Sending to AWS...");
        http.begin(client, api_url);

        // send put request
        http.addHeader("Content-Type", "application/json");
        // get http response
        int httpResponseCode = http.PUT(jsonData);
        if(httpResponseCode>0){

            String response = http.getString();  //Get the response to the request
        
            Serial.print("Response code: "); Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);  
            Serial.println("Send complete!");       //Print request answer
        }
        else{
            Serial.print("Error on sending POST: ");
            Serial.print("Response code: "); Serial.println(httpResponseCode); 
        }
    }
}

void connectWifi(){
    // set to station mode
    WiFi.mode(WIFI_STA);
    // disconnect any previous connection
    WiFi.disconnect();

    // begin wifi, connect to wifi with ssid and password defined
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.print("Connecting to WiFi with SSID of: "); Serial.println(WIFI_SSID);
    Serial.print("Connecting ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("Success!");
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

// local http web server
void beginServer(){

}


void setup(){
    Serial.begin(115200);
    Serial.println("Configuring...");
    connectWifi();
    client.setInsecure(); // Todo: change to use certificate/key
    // configure NTP time server
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // UART Setup
    Serial2.begin(115200, SERIAL_8N1, UART_RX2, UART_TX2);

    // init sensors
    // dht.begin();
    // sgp.begin();

    // init display
    // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // display.display();
    // delay(1000);
    // display.clearDisplay(); // clear buffer
    // display.display();

    // printToDisplay();
}

void loop(){

    // periodically check wifi
    unsigned long currentMillis = millis();
    if(currentMillis-previousMillis>= intervalMillis){
        // check wifi
        if(WiFi.status() != WL_CONNECTED){
            Serial.print(millis());
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
        // send data if connected
        else{
            sendDataAWS();
        }
        previousMillis = currentMillis;
    }
    
    // if(currentMillis-timePreviousMillis>=timeIntervalMillis){
    //     printToDisplay();
    //     timePreviousMillis = currentMillis;
    // }
    // readDHT();
    // readSGP30();

    // delay(10000);
    delay(1);
}