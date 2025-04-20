#include "LCD_1inch28.h"

LCD_1IN28_ATTRIBUTES LCD_1IN28;

static void Spi_DMA_Write_many_bytes(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *bitmap_color)
{
	HAL_GPIO_WritePin(LCS_DC_GPIO_Port,LCS_DC_Pin, 1);

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, 0);
    // 啟動 SPI DMA 傳輸
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)bitmap_color, (Xend - Xstart + 1) * (Yend - Ystart + 1) * 2);
	// 等待傳輸完成（可選）
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, 1);
}

static void Spi_Write_byte(uint8_t Reg)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, 0);

    // 啟動 SPI DMA 傳輸
//	HAL_SPI_Transmit_DMA(&hspi1, &Reg, 1);
	HAL_SPI_Transmit(&hspi1, &Reg, 1, HAL_MAX_DELAY);
	// 等待傳輸完成（可選）
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, 1);
}

/******************************************************************************
function :	Hardware reset
parameter:
******************************************************************************/
static void LCD_1IN28_Reset(void)
{
	HAL_GPIO_WritePin(LCS_RST_GPIO_Port,LCS_RST_Pin, 0);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LCS_RST_GPIO_Port,LCS_RST_Pin, 1);
    HAL_Delay(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void LCD_1IN28_SendCommand(uint8_t Reg)
{
    HAL_GPIO_WritePin(LCS_DC_GPIO_Port,LCS_DC_Pin, 0);
    Spi_Write_byte(Reg);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_1IN28_SendData_8Bit(uint8_t Data)
{
    HAL_GPIO_WritePin(LCS_DC_GPIO_Port,LCS_DC_Pin, 1);
    Spi_Write_byte(Data);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_1IN28_SendData_16Bit(uint8_t Data)
{
	HAL_GPIO_WritePin(LCS_DC_GPIO_Port,LCS_DC_Pin, 1);
    //LCD_1IN28_CS_0;
	Spi_Write_byte(Data >> 8);
	Spi_Write_byte(Data);
    //LCD_1IN28_CS_1;
}

/********************************************************************************
function:	Set the resolution and scanning method of the screen
parameter:
		Scan_dir:   Scan direction
********************************************************************************/
static void LCD_1IN28_SetAttributes(uint8_t Scan_dir)
{
    //Get the screen scan direction
    LCD_1IN28.SCAN_DIR = Scan_dir;
    uint8_t MemoryAccessReg = 0x08;

    //Get GRAM and LCD width and height
    if(Scan_dir == HORIZONTAL) {
        LCD_1IN28.HEIGHT	= LCD_1IN28_HEIGHT;
        LCD_1IN28.WIDTH   = LCD_1IN28_WIDTH;
        MemoryAccessReg = 0XC8;
    } else {
        LCD_1IN28.HEIGHT	= LCD_1IN28_WIDTH;
        LCD_1IN28.WIDTH   = LCD_1IN28_HEIGHT;
        MemoryAccessReg = 0X68;
    }

    // Set the read / write scan direction of the frame memory
    LCD_1IN28_SendCommand(0x36); //MX, MY, RGB mode
    LCD_1IN28_SendData_8Bit(MemoryAccessReg);	//0x08 set RGB
}

/******************************************************************************
function :	Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_1IN28_InitReg(void)
{
	LCD_1IN28_SendCommand(0xEF);
	LCD_1IN28_SendCommand(0xEB);
	LCD_1IN28_SendData_8Bit(0x14);

	LCD_1IN28_SendCommand(0xFE);
	LCD_1IN28_SendCommand(0xEF);

	LCD_1IN28_SendCommand(0xEB);
	LCD_1IN28_SendData_8Bit(0x14);

	LCD_1IN28_SendCommand(0x84);
	LCD_1IN28_SendData_8Bit(0x40);

	LCD_1IN28_SendCommand(0x85);
	LCD_1IN28_SendData_8Bit(0xFF);

	LCD_1IN28_SendCommand(0x86);
	LCD_1IN28_SendData_8Bit(0xFF);

	LCD_1IN28_SendCommand(0x87);
	LCD_1IN28_SendData_8Bit(0xFF);

	LCD_1IN28_SendCommand(0x88);
	LCD_1IN28_SendData_8Bit(0x0A);

	LCD_1IN28_SendCommand(0x89);
	LCD_1IN28_SendData_8Bit(0x21);

	LCD_1IN28_SendCommand(0x8A);
	LCD_1IN28_SendData_8Bit(0x00);

	LCD_1IN28_SendCommand(0x8B);
	LCD_1IN28_SendData_8Bit(0x80);

	LCD_1IN28_SendCommand(0x8C);
	LCD_1IN28_SendData_8Bit(0x01);

	LCD_1IN28_SendCommand(0x8D);
	LCD_1IN28_SendData_8Bit(0x01);

	LCD_1IN28_SendCommand(0x8E);
	LCD_1IN28_SendData_8Bit(0xFF);

	LCD_1IN28_SendCommand(0x8F);
	LCD_1IN28_SendData_8Bit(0xFF);


	LCD_1IN28_SendCommand(0xB6);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x20);

	LCD_1IN28_SendCommand(0x36);
	LCD_1IN28_SendData_8Bit(0x08);//Set as vertical screen

	LCD_1IN28_SendCommand(0x3A);
	LCD_1IN28_SendData_8Bit(0x05);


	LCD_1IN28_SendCommand(0x90);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);

	LCD_1IN28_SendCommand(0xBD);
	LCD_1IN28_SendData_8Bit(0x06);

	LCD_1IN28_SendCommand(0xBC);
	LCD_1IN28_SendData_8Bit(0x00);

	LCD_1IN28_SendCommand(0xFF);
	LCD_1IN28_SendData_8Bit(0x60);
	LCD_1IN28_SendData_8Bit(0x01);
	LCD_1IN28_SendData_8Bit(0x04);

	LCD_1IN28_SendCommand(0xC3);
	LCD_1IN28_SendData_8Bit(0x13);
	LCD_1IN28_SendCommand(0xC4);
	LCD_1IN28_SendData_8Bit(0x13);

	LCD_1IN28_SendCommand(0xC9);
	LCD_1IN28_SendData_8Bit(0x22);

	LCD_1IN28_SendCommand(0xBE);
	LCD_1IN28_SendData_8Bit(0x11);

	LCD_1IN28_SendCommand(0xE1);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x0E);

	LCD_1IN28_SendCommand(0xDF);
	LCD_1IN28_SendData_8Bit(0x21);
	LCD_1IN28_SendData_8Bit(0x0c);
	LCD_1IN28_SendData_8Bit(0x02);

	LCD_1IN28_SendCommand(0xF0);
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x09);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x26);
 	LCD_1IN28_SendData_8Bit(0x2A);

 	LCD_1IN28_SendCommand(0xF1);
 	LCD_1IN28_SendData_8Bit(0x43);
 	LCD_1IN28_SendData_8Bit(0x70);
 	LCD_1IN28_SendData_8Bit(0x72);
 	LCD_1IN28_SendData_8Bit(0x36);
 	LCD_1IN28_SendData_8Bit(0x37);
 	LCD_1IN28_SendData_8Bit(0x6F);


 	LCD_1IN28_SendCommand(0xF2);
 	LCD_1IN28_SendData_8Bit(0x45);
 	LCD_1IN28_SendData_8Bit(0x09);
 	LCD_1IN28_SendData_8Bit(0x08);
 	LCD_1IN28_SendData_8Bit(0x08);
 	LCD_1IN28_SendData_8Bit(0x26);
 	LCD_1IN28_SendData_8Bit(0x2A);

 	LCD_1IN28_SendCommand(0xF3);
 	LCD_1IN28_SendData_8Bit(0x43);
 	LCD_1IN28_SendData_8Bit(0x70);
 	LCD_1IN28_SendData_8Bit(0x72);
 	LCD_1IN28_SendData_8Bit(0x36);
 	LCD_1IN28_SendData_8Bit(0x37);
 	LCD_1IN28_SendData_8Bit(0x6F);

	LCD_1IN28_SendCommand(0xED);
	LCD_1IN28_SendData_8Bit(0x1B);
	LCD_1IN28_SendData_8Bit(0x0B);

	LCD_1IN28_SendCommand(0xAE);
	LCD_1IN28_SendData_8Bit(0x77);

	LCD_1IN28_SendCommand(0xCD);
	LCD_1IN28_SendData_8Bit(0x63);


	LCD_1IN28_SendCommand(0x70);
	LCD_1IN28_SendData_8Bit(0x07);
	LCD_1IN28_SendData_8Bit(0x07);
	LCD_1IN28_SendData_8Bit(0x04);
	LCD_1IN28_SendData_8Bit(0x0E);
	LCD_1IN28_SendData_8Bit(0x0F);
	LCD_1IN28_SendData_8Bit(0x09);
	LCD_1IN28_SendData_8Bit(0x07);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x03);

	LCD_1IN28_SendCommand(0xE8);
	LCD_1IN28_SendData_8Bit(0x34);

	LCD_1IN28_SendCommand(0x62);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x0D);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xED);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x0F);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xEF);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x70);

	LCD_1IN28_SendCommand(0x63);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x11);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x13);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xF3);
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x70);

	LCD_1IN28_SendCommand(0x64);
	LCD_1IN28_SendData_8Bit(0x28);
	LCD_1IN28_SendData_8Bit(0x29);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x01);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x07);

	LCD_1IN28_SendCommand(0x66);
	LCD_1IN28_SendData_8Bit(0x3C);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0xCD);
	LCD_1IN28_SendData_8Bit(0x67);
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);

	LCD_1IN28_SendCommand(0x67);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x3C);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x01);
	LCD_1IN28_SendData_8Bit(0x54);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x32);
	LCD_1IN28_SendData_8Bit(0x98);

	LCD_1IN28_SendCommand(0x74);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x85);
	LCD_1IN28_SendData_8Bit(0x80);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x4E);
	LCD_1IN28_SendData_8Bit(0x00);

	LCD_1IN28_SendCommand(0x98);
	LCD_1IN28_SendData_8Bit(0x3e);
	LCD_1IN28_SendData_8Bit(0x07);

	LCD_1IN28_SendCommand(0x35);
	LCD_1IN28_SendCommand(0x21);

	LCD_1IN28_SendCommand(0x11);
	HAL_Delay(120);
	LCD_1IN28_SendCommand(0x29);
	HAL_Delay(20);
}

/*******************************************************************************
function:
	Setting backlight
parameter	:
	  value : Range 0~1000   Duty cycle is value/1000
*******************************************************************************/
void LCD_1IN28_SetBackLight(uint16_t pwmVal)
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwmVal);
}

/********************************************************************************
function :	Initialize the lcd
parameter:
********************************************************************************/
void LCD_1IN28_Init(uint8_t Scan_dir)
{
	// for dummy, no reason, if do not have this, first byte transmit will wrong
	LCD_1IN28_SendCommand(0x00);

    //Hardware reset
    LCD_1IN28_Reset();

    //Set the resolution and scanning method of the screen
    LCD_1IN28_SetAttributes(Scan_dir);

    //Set the initialization register
    LCD_1IN28_InitReg();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void LCD_1IN28_Clear(uint16_t Color)
{
	uint16_t i,j;
    LCD_1IN28_SetWindows(0, 0, LCD_1IN28_WIDTH-1, LCD_1IN28_HEIGHT-1);

    HAL_GPIO_WritePin(LCS_DC_GPIO_Port,LCS_DC_Pin, 1);
	for(i = 0; i < LCD_1IN28_WIDTH; i++){
		for(j = 0; j < LCD_1IN28_HEIGHT; j++){
			Spi_Write_byte(Color>>8);
			Spi_Write_byte(Color);
		}
	 }
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
		Xstart 	:   X direction Start coordinates
		Ystart  :   Y direction Start coordinates
		Xend    :   X direction end coordinates
		Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_1IN28_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    //set the X coordinates
    LCD_1IN28_SendCommand(0x2A);
    LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Xstart);
	LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Xend);

    //set the Y coordinates
    LCD_1IN28_SendCommand(0x2B);
    LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(Ystart);
	LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Yend);

    LCD_1IN28_SendCommand(0X2C);
}

/******************************************************************************
function: Draw a point
parameter	:
	    X	: 	Set the X coordinate
	    Y	:	Set the Y coordinate
	  Color :	Set the color
******************************************************************************/
void LCD_1IN28_DrawPaint(uint16_t x, uint16_t y, uint16_t Color)
{
	LCD_1IN28_SetWindows(x,y,x,y);
	LCD_1IN28_SendData_16Bit(Color);
}

void LCD_1IN28_Paint_LVGL_Wu(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *bitmap_color)
{
	LCD_1IN28_SetWindows(Xstart, Ystart, Xend, Yend);

	Spi_DMA_Write_many_bytes(Xstart, Ystart, Xend, Yend, bitmap_color);
}
