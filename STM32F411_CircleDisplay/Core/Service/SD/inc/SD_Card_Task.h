/*
 * SD_Card.h
 *
 *  Created on: Mar 29, 2025
 *      Author: Dennis
 */

#ifndef SERVICE_SD_INC_SD_CARD_TASK_H_
#define SERVICE_SD_INC_SD_CARD_TASK_H_
#include "main.h"
#include "sdio.h"

void SD_Card_Task_Init(void);
void SD_WriteData(uint32_t block_addr, uint8_t *data, uint32_t block_count);
void SD_ReadData(uint32_t block_addr, uint8_t *buffer, uint32_t block_count);
void ShowSDcardInfo(void);
void SDIO_write_read_test(void);
void SD_mount_Fatfs(void);
void SD_mount_Fats_test_to_debug(void);

#endif /* SERVICE_SD_INC_SD_CARD_TASK_H_ */
