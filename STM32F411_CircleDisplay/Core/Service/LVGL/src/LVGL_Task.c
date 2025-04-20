/*
 * LVGL_Task.c
 *
 *  Created on: Apr 20, 2025
 *      Author: Dennis
 */
#include "LVGL_Task.h"

static osThreadId_t LVGL_Task_TaskHandle;

static const osThreadAttr_t LVGL_Task_attributes ={
		.name 		= "LVGL_Task",
		.priority 	= (osPriority_t)osPriorityNormal,
		.stack_size = 4096
};

static void LVGL_task(void *arg);

void LVGL_Task_Init(void)
{
	LVGL_Task_TaskHandle = osThreadNew(LVGL_task,NULL,&LVGL_Task_attributes);
}

static void LVGL_task(void *arg)
{
//	printf("Before LCD_1in28_test, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
//	printf("After LCD_1in28_test, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));

	LVGL_Button();

	for(;;)
	{
		lv_timer_handler();
		osDelay(5);
	}
}

// 文字跑馬燈
void LVGL_Label_Marquee(void)
{
	static char* github_addr = "https://gitee.com/WRS0923";
	lv_obj_t * label = lv_label_create(lv_scr_act());
	lv_label_set_recolor(label, true);
	lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
	lv_obj_set_width(label, 120);
	lv_label_set_text_fmt(label, "#ff0000 Gitee: %s#", github_addr);
	lv_obj_set_style_bg_color(lv_scr_act(),lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 20);
}

void LVGL_Button(void)
{
	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
	lv_obj_set_size(switch_obj,100,50);
	lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
}
