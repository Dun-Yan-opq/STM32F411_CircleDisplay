/*
 * LVGL_Task.h
 *
 *  Created on: Apr 20, 2025
 *      Author: Dennis
 */

#ifndef SERVICE_LVGL_INC_LVGL_TASK_H_
#define SERVICE_LVGL_INC_LVGL_TASK_H_

#include "cmsis_os2.h"
#include "lv_port_disp.h"

#ifdef __cplusplus
extern "C" {
#endif

void LVGL_Task_Init(void);
void LVGL_Label_Marquee(void);
void LVGL_Button(void);


#ifdef __cplusplus
}
#endif

#endif /* SERVICE_LVGL_INC_LVGL_TASK_H_ */
