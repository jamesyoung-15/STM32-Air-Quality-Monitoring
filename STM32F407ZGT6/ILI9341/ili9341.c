#include "ili9341.h"

uint16_t LCD_HEIGHT = SCREEN_DIMENSION_LARGER;
uint16_t LCD_WIDTH	 = SCREEN_DIMENSION_SMALLER;

void ILI9341_WriteRegister(uint8_t command){
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_GPIO, DC_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 1000);
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_SendData(uint8_t data){
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_GPIO, DC_PIN, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi1, &data, sizeof(data), 1000);
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_SendData16Bit(uint16_t data){
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_GPIO, DC_PIN, GPIO_PIN_SET);
    uint8_t first8Bits = data>>8;
    uint8_t last8Bits = 0x00FF & data;
    HAL_SPI_Transmit(&hspi1, &first8Bits, sizeof(first8Bits), 1000);
    HAL_SPI_Transmit(&hspi1, &last8Bits, sizeof(last8Bits), 1000);
    HAL_GPIO_WritePin(SPI_CS_GPIO,SPI_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_Reset(){
    HAL_GPIO_WritePin(RESET_GPIO, RESET_PIN, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(RESET_GPIO, RESET_PIN, GPIO_PIN_SET);	
}

void ILI9341_Init(uint8_t screenOrientation){
    // Reset LCD Hardware
    ILI9341_Reset();
    // Reset LCD software
    ILI9341_WriteRegister(0x01);
    HAL_Delay(500);

    //POWER CONTROL A
    ILI9341_WriteRegister(0xCB);
    ILI9341_SendData(0x39);
    ILI9341_SendData(0x2C);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x34);
    ILI9341_SendData(0x02);

    //POWER CONTROL B
    ILI9341_WriteRegister(0xCF);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0xC1);
    ILI9341_SendData(0x30);

    //DRIVER TIMING CONTROL A
    ILI9341_WriteRegister(0xE8);
    ILI9341_SendData(0x85);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x78);

    //DRIVER TIMING CONTROL B
    ILI9341_WriteRegister(0xEA);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x00);

    //POWER ON SEQUENCE CONTROL
    ILI9341_WriteRegister(0xED);
    ILI9341_SendData(0x64);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x12);
    ILI9341_SendData(0x81);

    //PUMP RATIO CONTROL
    ILI9341_WriteRegister(0xF7);
    ILI9341_SendData(0x20);

    //POWER CONTROL,VRH[5:0]
    ILI9341_WriteRegister(0xC0);
    ILI9341_SendData(0x23);

    //POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteRegister(0xC1);
    ILI9341_SendData(0x10);

    //VCM CONTROL
    ILI9341_WriteRegister(0xC5);
    ILI9341_SendData(0x3E);
    ILI9341_SendData(0x28);

    //VCM CONTROL 2
    ILI9341_WriteRegister(0xC7);
    ILI9341_SendData(0x86);

    //MEMORY ACCESS CONTROL
    ILI9341_WriteRegister(0x36);
    ILI9341_SendData(0x48);

    //PIXEL FORMAT
    ILI9341_WriteRegister(0x3A);
    ILI9341_SendData(0x55);

    //FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteRegister(0xB1);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x18);

    //DISPLAY FUNCTION CONTROL
    ILI9341_WriteRegister(0xB6);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x82);
    ILI9341_SendData(0x27);

    //3GAMMA FUNCTION DISABLE
    ILI9341_WriteRegister(0xF2);
    ILI9341_SendData(0x00);

    //GAMMA CURVE SELECTED
    ILI9341_WriteRegister(0x26);
    ILI9341_SendData(0x01);

    //POSITIVE GAMMA CORRECTION
    ILI9341_WriteRegister(0xE0);
    ILI9341_SendData(0x0F);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0x2B);
    ILI9341_SendData(0x0C);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x4E);
    ILI9341_SendData(0xF1);
    ILI9341_SendData(0x37);
    ILI9341_SendData(0x07);
    ILI9341_SendData(0x10);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x09);
    ILI9341_SendData(0x00);

    //NEGATIVE GAMMA CORRECTION
    ILI9341_WriteRegister(0xE1);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x14);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x11);
    ILI9341_SendData(0x07);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0xC1);
    ILI9341_SendData(0x48);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x0F);
    ILI9341_SendData(0x0C);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0x36);
    ILI9341_SendData(0x0F);

    //EXIT SLEEP
    ILI9341_WriteRegister(0x11);
    HAL_Delay(120);

    //TURN ON DISPLAY
    ILI9341_WriteRegister(0x29);
    
    // Set Orientation
    ILI9341_WriteRegister(0x36);
    if(screenOrientation == 0){
        // Vertical V1, top of screen are near main pins
        ILI9341_SendData(0x88);
        LCD_HEIGHT = SCREEN_DIMENSION_LARGER;
        LCD_WIDTH = SCREEN_DIMENSION_SMALLER;
    }
    else if(screenOrientation == 1){
        // Vertical V2, bottom of screen are near main pins
        ILI9341_SendData(0x48);
        LCD_HEIGHT = SCREEN_DIMENSION_LARGER;
        LCD_WIDTH = SCREEN_DIMENSION_SMALLER;
    }
    else if(screenOrientation == 2){
        // Horizontal Orientation V1
        ILI9341_SendData(0x28);
        LCD_HEIGHT = SCREEN_DIMENSION_SMALLER;
        LCD_WIDTH = SCREEN_DIMENSION_LARGER;
    }
    else if(screenOrientation == 3){
        // Horizontal Orientation V2
        ILI9341_SendData(0xe8);
        LCD_HEIGHT = SCREEN_DIMENSION_SMALLER;
        LCD_WIDTH = SCREEN_DIMENSION_LARGER;
    }
    else{
        // Vertical V1, top of screen are near main pins
        ILI9341_SendData(0x88);
        LCD_HEIGHT = SCREEN_DIMENSION_LARGER;
        LCD_WIDTH = SCREEN_DIMENSION_SMALLER;
    }
    HAL_Delay(100);

}



void ILI9341_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    // Set column
    ILI9341_WriteRegister(0x2A);
    ILI9341_SendData(x0>>8);
	ILI9341_SendData(0x00FF&x0);		
	ILI9341_SendData(x1>>8);
	ILI9341_SendData(0x00FF&x1);

    // Set row, need to shift bits cause 
	ILI9341_WriteRegister(0x2B);	
	ILI9341_SendData(y0>>8);
	ILI9341_SendData(0x00FF&y0);		
	ILI9341_SendData(y1>>8);
	ILI9341_SendData(0x00FF&y1);

    // Write to RAM
    ILI9341_WriteRegister(0x2C);

}

void ILI9341_SetCursor(uint16_t x0, uint16_t y0){
    ILI9341_SetWindow(x0,y0,x0,y0);
}


void ILI9341_DrawPixel(uint16_t x, uint16_t y){
    // check if out of bounds
    if(x>=LCD_WIDTH || y>=LCD_HEIGHT){
        x = LCD_WIDTH;
        y = LCD_HEIGHT;
    }
    
    // set point to draw to
    ILI9341_SetCursor(x, y);
    // place dot on coordinate (black)
    ILI9341_SendData16Bit(LCD_BLACK);

}

void ILI9341_DrawArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    // out of bounds check
    if(x0>=LCD_WIDTH || y0>=LCD_HEIGHT) return;
    if(x0 + x1 >= LCD_WIDTH){
        x1 = LCD_WIDTH - x0;
    }
    if(y0 + y1 >= LCD_HEIGHT){
        y1 = LCD_HEIGHT - y0;
    }

    // set window area then fill area
    ILI9341_SetWindow(x0,y0,x1,y1);
    for(uint16_t i=0;i<x1;i++){
        for(uint16_t j=0;j<y1;j++){
            ILI9341_SendData16Bit(LCD_BLACK);
        }
    }
    
}


void ILI9341_ClearScreen(){
    // set window area then fill area
    ILI9341_SetWindow(0,0,LCD_WIDTH,LCD_HEIGHT);
    for(uint16_t i=0;i<LCD_WIDTH;i++){
        for(uint16_t j=0;j<LCD_HEIGHT;j++){
            ILI9341_SendData16Bit(LCD_WHITE);
        }
    }
}

void ILI9341_DrawChar(uint16_t x, uint16_t y, char word, FontDef font){


    ILI9341_SetWindow(x, y, x+font.width-1, y+font.height-1);

    for(uint32_t i = 0; i < font.height; i++) {
        uint32_t b = font.data[(word - 32) * font.height + i];
        for(uint32_t j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                ILI9341_SendData16Bit(LCD_BLACK);
            } 
            else {
                ILI9341_SendData16Bit(LCD_WHITE);
            }
        }
    }
}

void ILI9341_DrawString(uint16_t x, uint16_t y, char* string, FontDef font){
    while(*string) {
        if(x + font.width >= LCD_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= LCD_HEIGHT) {
                break;
            }

            if(*string == ' ') {
                // skip spaces in the beginning of the new line
                string++;
                continue;
            }
        }

        ILI9341_DrawChar(x, y, *string, font);
        x += font.width;
        string++;
    }
}