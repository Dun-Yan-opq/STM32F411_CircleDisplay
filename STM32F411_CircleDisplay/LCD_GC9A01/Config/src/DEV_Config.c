#include "DEV_Config.h"




int DEV_Module_Init(void)
{
	DEV_Digital_Write(LCS_DC_GPIO_Port,LCS_DC_Pin, 1);
    DEV_Digital_Write(SPI1_CS_GPIO_Port,SPI1_CS_Pin, 1);
    DEV_Digital_Write(LCS_RST_GPIO_Port,LCS_RST_Pin, 1);
//	DEV_Digital_Write(DEV_INT_PIN, 1);
    DEV_Digital_Write(TP_RST_GPIO_Port,TP_RST_Pin, 1);
//    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
//
//	DEV_I2C_Init(0x15 << 1);
	return 0;
}
