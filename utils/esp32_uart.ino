#define RX2 16
#define TX2 17

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
}

void loop() {
  // put your main code here, to run repeatedly:
    while(Serial2.available()){
        Serial.println(Serial2.readString());
    }
}
