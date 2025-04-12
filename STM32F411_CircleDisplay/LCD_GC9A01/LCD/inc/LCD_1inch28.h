#ifndef _LCD_1INCH28_H_
#define _LCD_1INCH28_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include "spi.h"
#include "LCD_1inch28.h"

#define HORIZONTAL 0
#define VERTICAL   1

#define LCD_1IN28_HEIGHT 240
#define LCD_1IN28_WIDTH 240

typedef struct{
	uint16_t WIDTH;
	uint16_t HEIGHT;
	uint8_t SCAN_DIR;
}LCD_1IN28_ATTRIBUTES;
extern LCD_1IN28_ATTRIBUTES LCD_1IN28;

void LCD_1IN28_SetBackLight(uint16_t Value);
void LCD_1IN28_Init(uint8_t Scan_dir);
void LCD_1IN28_Clear(uint16_t Color);
void LCD_1IN28_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void LCD_1IN28_DrawPaint(uint16_t x, uint16_t y, uint16_t Color);


#ifdef __cplusplus
}
#endif

#endif
