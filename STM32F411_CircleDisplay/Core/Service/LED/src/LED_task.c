/*
 * LED_task.c
 *
 *  Created on: Apr 19, 2025
 *      Author: Dennis
 */
#include "LED_task.h"

static osThreadId_t LED_Task_TaskHandle;

static const osThreadAttr_t LED_Task_attributes ={
		.name 		= "LED_Task",
		.priority 	= (osPriority_t)osPriorityNormal,
		.stack_size = 128
};

static void LED_task(void *arg);

void LED_Task_Init(void)
{
	LED_Task_TaskHandle = osThreadNew(LED_task,NULL,&LED_Task_attributes);
}

static void LED_task(void *arg)
{
	for(;;)
	{
		  HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, GPIO_PIN_RESET);
		  osDelay(100);
		  HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, GPIO_PIN_SET);
		  osDelay(100);
	}
}
