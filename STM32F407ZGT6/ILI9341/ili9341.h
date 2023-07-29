#ifndef ILI9341_H
#define ILI9341_H

#include "fonts.h"
#include "stm32f4xx_hal.h"
#include "main.h"


/* Using GPIO C4 as CS, GPIO C5 as reset, GPIO B0 as DC */

extern SPI_HandleTypeDef hspi1;

#define DC_GPIO GPIOB
#define DC_PIN GPIO_PIN_0
#define RESET_GPIO GPIOC
#define RESET_PIN GPIO_PIN_5
#define SPI_CS_GPIO GPIOC
#define SPI_CS_PIN GPIO_PIN_4

#define LCD_WHITE       0xFFFF
#define LCD_BLACK      	0x0000	 
#define SCREEN_DIMENSION_SMALLER 240
#define SCREEN_DIMENSION_LARGER 320

/**
 * @brief Write an 8 bit command to LCD screen, mainly for initializing LCD. Check documentation for command list.
 * 
 * @param 8 bit hex command value that corresponds to command function
 */
void ILI9341_WriteRegister(uint8_t command);

/**
 * @brief Sends an 8 bit data to LCD screen
 * 
 * @param data to be sent (8bit)
 */
void ILI9341_SendData(uint8_t data);

/**
 * @brief Sends a 16 bit data to LCD screen
 * 
 * @param 16 bit data to be sent
 */
void ILI9341_SendData16Bit(uint16_t data);

/**
 * @brief Resets the LCD screen
 * 
 */
void ILI9341_Reset();

/**
 * @brief Initializes the LCD screen, such as screen orientation
 * 
 * @param Orientation to set the screen with number corresponding to option: 
 * 0 means vertical orientation with top of screen near main pins (default) 
 * 1 means vertical orientation with bottom of screen near main pins
 * 2 means horizontal orientation
 * 3 means another horizontal orientation
 */
void ILI9341_Init(uint8_t screenOrientation);


/**
 * @brief Clears LCD screen and fills screen with white background
 * 
 */
void ILI9341_ClearScreen();

/**
 * @brief 
 * 
 * @param x0 
 * @param y0 
 */
void ILI9341_SetCursor(uint16_t x0, uint16_t y0);

void ILI9341_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void ILI9341_DrawPixel(uint16_t x, uint16_t y);

/**
 * @brief 
 * 
 * @param x0 starting x coordinate 
 * @param y0 starting y coordinate
 * @param x1 final x coordinate
 * @param y1 final y coordinate
 */
void ILI9341_DrawArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief Draws a single character to screen
 * 
 * @param x coordinate
 * @param y coordinate
 * @param word char to draw
 * @param font to use
 */
void ILI9341_DrawChar(uint16_t x, uint16_t y, char word, FontDef font);

/**
 * @brief Draws a whole string message to screen
 * 
 * @param x coordinate
 * @param y coordinate
 * @param string message to write
 * @param font to use
 */
void ILI9341_DrawString(uint16_t x, uint16_t y, char* string, FontDef font);

#endif