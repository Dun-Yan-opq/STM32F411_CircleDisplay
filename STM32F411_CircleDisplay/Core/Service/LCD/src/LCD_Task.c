/*
 * LCD_Task.c
 *
 *  Created on: Apr 19, 2025
 *      Author: Dennis
 */
#include "LCD_Task.h"

static osThreadId_t LCD_Task_TaskHandle;

static const osThreadAttr_t LCD_Task_attributes ={
		.name 		= "LCD_Task",
		.priority 	= (osPriority_t)osPriorityNormal,
		.stack_size = 1024
};

static void LCD_task(void *arg);

void LCD_Task_Init(void)
{
	LCD_Task_TaskHandle = osThreadNew(LCD_task,NULL,&LCD_Task_attributes);
}

static void LCD_task(void *arg)
{
//	printf("Before LCD_1in28_test, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));
	LCD_1in28_test();
//	printf("After LCD_1in28_test, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));
	for(;;)
	{
		  osDelay(10000);
	}
}
