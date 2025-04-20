//#include "image.h"
//#include "LCD_Test.h"
//#include "LCD_1in28.h"
#include "Touch_Driver.h"
//#include "DEV_Config.h"
#include "LCD_1inch28_test.h"

void LCD_1in28_test(void)
{
	// LCD ------------------------------------------------------------
	printf("LCD_1IN28_test Demo\r\n");
	LCD_1IN28_SetBackLight(100);
	LCD_1IN28_Init(VERTICAL);

	printf("Paint_NewImage\r\n");
	Paint_NewImage(LCD_1IN28_WIDTH,LCD_1IN28_HEIGHT, 0, BLACK);

	printf("Set Clear and Display Funtion\r\n");
	Paint_SetClearFuntion(LCD_1IN28_Clear);
	Paint_SetDisplayFuntion(LCD_1IN28_DrawPaint);

	printf("Paint_Clear\r\n");
	Paint_Clear(RED);

	Paint_DrawString_EN(35, 90, "Gesture test", &Font20, BLACK, WHITE);
	Paint_DrawString_EN(10, 120, "Complete as prompted", &Font16, BLACK, WHITE);

	// Touch ------------------------------------------------------------
	XY.mode = 0;
	if(Touch_1IN28_init(XY.mode) == true)
		printf("OK!\r\n");
	else
		printf("NO!\r\n");

	// Up
	Paint_ClearWindows(10,90,230,137,WHITE);
	Paint_DrawString_EN(105, 100, "Up", &Font24, BLACK, WHITE);
	while(XY.Gesture != UP)
	{
		if (touch_interrupt_flag == TOUCH_IRQ)
		{
			uint8_t rx_data[1]={0};
			if(Touch_I2C_read_one_byte(Touch_DEV_address,GESTUREID, rx_data,1))
			{
				XY.Gesture = rx_data[0];
			}

			touch_interrupt_flag = TOUCH_DRAW;
		}
		HAL_Delay(10);
	}

	// Down
	Paint_ClearWindows(105,100,160,155,WHITE);
	Paint_DrawString_EN(85, 100, "Down", &Font24, 0X647C, WHITE);
	while(XY.Gesture != Down)
	{
		if (touch_interrupt_flag == TOUCH_IRQ)
		{
			uint8_t rx_data[1]={0};
			if(Touch_I2C_read_one_byte(Touch_DEV_address,GESTUREID, rx_data,1))
				XY.Gesture = rx_data[0];

			touch_interrupt_flag = TOUCH_DRAW;
		}
		HAL_Delay(10);
	}

	// Right
	Paint_ClearWindows(85, 100,160,155,WHITE);
	Paint_DrawString_EN(80, 100, "Right", &Font24, 0X647C, WHITE);
	while(XY.Gesture != RIGHT)
	{
		if (touch_interrupt_flag == TOUCH_IRQ)
		{
			uint8_t rx_data[1]={0};
			if(Touch_I2C_read_one_byte(Touch_DEV_address,GESTUREID, rx_data,1))
				XY.Gesture = rx_data[0];

			touch_interrupt_flag = TOUCH_DRAW;
		}
		HAL_Delay(10);
	}

	// Long Press
	Paint_ClearWindows(80, 100,165,160,WHITE);
	Paint_DrawString_EN(47, 100, "Long Press", &Font20, 0X647C, WHITE);
	while(XY.Gesture != LONG_PRESS)
	{
		if (touch_interrupt_flag == TOUCH_IRQ)
		{
			uint8_t rx_data[1]={0};
			if(Touch_I2C_read_one_byte(Touch_DEV_address,GESTUREID, rx_data,1))
				XY.Gesture = rx_data[0];

			touch_interrupt_flag = TOUCH_DRAW;
		}
		HAL_Delay(10);
	}

	// Double Click
	Paint_ClearWindows(47, 100,200,155,WHITE);
	Paint_DrawString_EN(35, 100, "Double Click", &Font20, 0X647C, WHITE);
	while(XY.Gesture != DOUBLE_CLICK)
	{
		if (touch_interrupt_flag == TOUCH_IRQ)
		{
			uint8_t rx_data[1]={0};
			if(Touch_I2C_read_one_byte(Touch_DEV_address,GESTUREID, rx_data,1))
				XY.Gesture = rx_data[0];

			touch_interrupt_flag = TOUCH_DRAW;
		}
		HAL_Delay(10);
	}


}
