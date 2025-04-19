/*
 * LCD_Task.h
 *
 *  Created on: Apr 19, 2025
 *      Author: Dennis
 */

#ifndef SERVICE_LCD_INC_LCD_TASK_H_
#define SERVICE_LCD_INC_LCD_TASK_H_

#include "cmsis_os2.h"
#include "main.h"
#include "LCD_1inch28_test.h"

#ifdef __cplusplus
extern "C" {
#endif

void LCD_Task_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_LCD_INC_LCD_TASK_H_ */
