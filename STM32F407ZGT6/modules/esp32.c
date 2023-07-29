#include "esp32.h"

void drawESP32IP(char* ipAddr){
    ILI9341_DrawString(40,120, ipAddr, Font_7x10);
}

void drawESP32Time(char* time){
    ILI9341_DrawString(15,70, time, Font_7x10);
}