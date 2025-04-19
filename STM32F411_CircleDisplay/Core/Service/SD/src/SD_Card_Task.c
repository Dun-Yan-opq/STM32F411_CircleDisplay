/*
 * SD_Card.c
 *
 *  Created on: Mar 29, 2025
 *      Author: Dennis
 */
#include "SD_Card_Task.h"
#include "fatfs.h"

FRESULT  f_res;      // FatFs 返回狀態
static BYTE work[4096];  // 工作缓冲区，至少 4KB

UINT fnum;                    /* 文件成功读写数量 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
	"Dennis使用STM32F411開發板, 透過Fatfs新建文件系統測試文件\r\n";
BYTE ReadBuffer[1024]= {0};       /* 读缓冲区 */

/* FreeRTOS parameters*/
static osThreadId_t SD_Card_Task_TaskHandle;

static const osThreadAttr_t SD_Card_Task_attributes ={
		.name 		= "SD_Card_Task",
		.priority 	= (osPriority_t)osPriorityNormal,
		.stack_size = 1024
};

static void SD_Card_task(void *arg);

void SD_Card_Task_Init(void)
{
	SD_Card_Task_TaskHandle = osThreadNew(SD_Card_task,NULL,&SD_Card_Task_attributes);
	if (SD_Card_Task_TaskHandle == NULL) {
	    // 任務創建失敗，打印錯誤或處理
	    printf("SD_Card_Task creation failed!\n");
	    while (1); // 進入錯誤處理
	}
}

static void SD_Card_task(void *arg)
{
//  SDIO_write_read_test();
	/* uxTaskGetStackHighWaterMark() 值越接近0, 代表快要溢出*/
	printf("Before SD_mount_Fatfs, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));
//	SD_mount_Fats_test_to_debug();
	printf("After SD_mount_Fatfs, stack: %lu words\r\n", uxTaskGetStackHighWaterMark(NULL));

	for(;;)
	{
		osDelay(10000);
	}
}

/* ------------------------------------------------------------------------------------------ */
// 向 SD 卡寫入數據
void SD_WriteData(uint32_t block_addr, uint8_t *data, uint32_t block_count) {
    if (HAL_SD_WriteBlocks(&hsd, data, block_addr, block_count, 5000) == HAL_OK) {
        printf("成功向 SD 卡地址 0x%08lX 寫入 %lu 個 block\r\n", block_addr, block_count);
    } else {
        printf("SD 卡寫入失敗！\r\n");
    }
//    // 等待寫入完成
//    SD_WaitForWriteComplete();
}

// 從 SD 卡讀取數據
void SD_ReadData(uint32_t block_addr, uint8_t *buffer, uint32_t block_count) {
    if (HAL_SD_ReadBlocks(&hsd, buffer, block_addr, block_count, 5000) == HAL_OK) {
        printf("成功從 SD 卡地址 0x%08lX 讀取 %lu 個 block\r\n", block_addr, block_count);
    } else {
        printf("SD 卡讀取失敗！\r\n");
    }
}

void ShowSDcardInfo(void)
{
	HAL_SD_CardInfoTypeDef cardInfo;
	if (HAL_SD_GetCardInfo(&hsd, &cardInfo) == HAL_OK) {
//	    printf("卡容量: %lu MB\r\n", (cardInfo.BlockNbr * cardInfo.BlockSize));
	    if (cardInfo.CardType == CARD_SDSC)
	      printf("SDCard type: SDSC\r\n");
	    else if (cardInfo.CardType == CARD_SDHC_SDXC)
	      printf("SDCard type: SDHC/SDXC\r\n");
	    if (cardInfo.CardVersion == CARD_V1_X)
	      printf("SDCard version: V1\r\n");
	    else if (cardInfo.CardVersion == CARD_V2_X)
	      printf("SDCard version: V2\r\n");
	    printf("SDCard block number: %lu\r\n", cardInfo.BlockNbr);
	    printf("SDCard block size: %lu\r\n", cardInfo.BlockSize);
	    printf("SDCard capacity: %lu MB\r\n", (cardInfo.BlockNbr/1048576) * (cardInfo.BlockSize)); //1048576
	} else {
	    printf("無法獲取卡信息\r\n");
	}
}

void SDIO_write_read_test(void)
{
	// 設置為 1-bit 模式
	if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_1B) != HAL_OK) {
	  printf("SDIO 1-bit Mode Config Error\r\n");
	}
	if (HAL_SD_Init(&hsd) != HAL_OK) {
	  printf("SDIO 初始化失敗，錯誤碼: %lu\r\n", hsd.ErrorCode);
	  while (1);
	}
	ShowSDcardInfo();

	uint16_t BLOCK_SIZE = 512;
	uint8_t write_data[512] = "Hello STM32 SDIO 1-bit!";
	uint8_t read_data[BLOCK_SIZE];

	// 向 SD 卡地址 0x00000000 寫入數據
	SD_WriteData(0, write_data, 1);

	// 從 SD 卡地址 0x00000000 讀取數據
	SD_ReadData(0, read_data, 1);

	// 比較數據
	if (memcmp(write_data, read_data, BLOCK_SIZE) == 0) {
	  printf("數據校驗成功！寫入和讀取數據一致。\r\n");
	} else {
	  printf("數據校驗失敗！請檢查硬體連接或 SD 卡狀態。\r\n");
	}
}

void SD_mount_Fatfs(void)
{
	/* 注册一个FatFS设备：SD卡 */
    f_mount(NULL, "0:", 0);
    HAL_Delay(1000);  // 延迟 500ms
	// 挂載 FatFs 文件系統
    f_res = f_mount(&SDFatFS, SDPath, 1);
	printf("挂载 SD 卡返回值: %d\r\n", f_res);
	if (f_res != FR_OK) {
	  printf("FatFs 挂載失敗，錯誤碼: %d\r\n", f_res);
//	  Error_Handler();
	}
	/*--------------- 格式化测试 ---------------*/
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if (f_res == FR_NO_FILESYSTEM) {
		printf(">> SD卡還沒有文件系統，即將進行格式化...\r\n");
		/* 格式化 */
		f_res=f_mkfs((TCHAR const*)SDPath, FM_FAT32, 0, work, sizeof(work));

		if (f_res == FR_OK) {
			printf(">> SD卡已成功格式化文件系統。\r\n");
			/* 格式化后，先取消挂载 */
			f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
			/* 重新挂载 */
			f_res = f_mount(&SDFatFS,(TCHAR const*)SDPath,1);
		} else {
			printf("<< 格式化失敗: %d>>\r\n",f_res);
			while (1);
		}
	} else if (f_res!=FR_OK) {
		printf("!!SD卡掛載文件系統失敗。(%d)\r\n",f_res);
		while (1);
	} else {
		printf(">> 文件系統掛載成功，可以進行讀寫測試\r\n");
		ShowSDcardInfo();
	}
}

void SD_mount_Fats_test_to_debug(void)
{
	printf("****** 1. 這是一個SD卡文件系統實驗 ******\r\n");
	SD_mount_Fatfs();

    /*---------- 文件系统测试：写测试 --------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("****** 2. 即將進行文件寫入測試... ******\r\n");
    f_res = f_open(&SDFile, "AAA.txt", FA_CREATE_ALWAYS|FA_WRITE);
    if ( f_res == FR_OK ) {
        printf(">> 打開/創建Fatfs讀寫測試文件.txt文件成功，向文件寫入數據。\r\n");
        /* 将指定存储区内容写入到文件内 */
        f_res=f_write(&SDFile,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if (f_res==FR_OK) {
            printf(">> 文件寫入成功，寫入字節數據: %d\r\n",fnum);
            printf(">> 向文件寫入的數據為: \r\n%s\r\n",WriteBuffer);
        } else {
            printf("!!文件寫入失敗:(%d)\r\n",f_res);
        }
        /* 不再读写，关闭文件 */
        f_close(&SDFile);
    } else {
        printf("!! 打開/創建文件失敗: %d\r\n",f_res);
    }

    /*----------- 文件系统测试：读测试 ---------------*/
	printf("****** 3. 即將進行文件讀取測試... ******\r\n");
	f_res = f_open(&SDFile, "AAA.txt", FA_OPEN_EXISTING | FA_READ);
	if (f_res == FR_OK) {
		printf(">>打開文件成功。\r\n");
		f_res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer),&fnum);
		if (f_res==FR_OK) {
			printf(">>文件讀取成功,讀到字節數據:%d\r\n",fnum);
			printf(">>讀取的文件數據為: \r\n%s \r\n",ReadBuffer);
		} else {
			printf("!!文件讀取失敗：(%d)\r\n",f_res);
		}
	} else {
		printf("!!打開文件失敗。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&SDFile);

	/* 不再使用，取消挂载 */
	f_res = f_mount(NULL,(TCHAR const*)SDPath,1);

}

