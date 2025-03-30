/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights f_reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//FIL fil;          // 文件對象
//FRESULT  f_res;      // FatFs 返回狀態
//BYTE work[4096];  // 工作缓冲区，至少 4KB
//
//UINT fnum;                    /* 文件成功读写数量 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//    "欢迎使用野火STM32 F103开发板 今天是个好日子，新建文件系统测试文件\r\n";
//BYTE ReadBuffer[1024]= {0};       /* 读缓冲区 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//char SDPath[4];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

//  SDIO_write_read_test();
  SD_mount_Fats_test_to_debug();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, GPIO_PIN_RESET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, GPIO_PIN_SET);
	  HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void SD_mount_Fats(void)
//{
//    printf("****** 这是一个SD卡文件系统实验 ******\r\n");
//	/* 注册一个FatFS设备：SD卡 */
//    f_mount(NULL, "0:", 0);
//    HAL_Delay(500);  // 延迟 500ms
//	// 挂載 FatFs 文件系統
//    f_res = f_mount(&SDFatFS, SDPath, 1);
//	printf("挂载 SD 卡返回值: %d\r\n", f_res);
//	if (f_res != FR_OK) {
//	  printf("FatFs 挂載失敗，錯誤碼: %d\r\n", f_res);
////	  Error_Handler();
//	}
//	/*--------------- 格式化测试 ---------------*/
//	/* 如果没有文件系统就格式化创建创建文件系统 */
//	if (f_res == FR_NO_FILESYSTEM) {
//		printf(">> SD卡還沒有文件系統，即將進行格式化...\r\n");
//		/* 格式化 */
//		// 自动选择文件系统类型 (FM_ANY)
//		f_res=f_mkfs((TCHAR const*)SDPath, FM_FAT32, 0, work, sizeof(work));
//
//		if (f_res == FR_OK) {
//			printf(">> SD卡已成功格式化文件系統。\r\n");
//			/* 格式化后，先取消挂载 */
//			f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
//			/* 重新挂载 */
//			f_res = f_mount(&SDFatFS,(TCHAR const*)SDPath,1);
//		} else {
//			printf("<< 格式化失敗: %d>>\r\n",f_res);
//			while (1);
//		}
//	} else if (f_res!=FR_OK) {
//		printf("!!SD卡掛載文件系統失敗。(%d)\r\n",f_res);
//		while (1);
//	} else {
//		printf(">> 文件系統掛載成功，可以進行讀寫測試\r\n");
//		ShowSDcardInfo();
//	}
//
//    /*---------- 文件系统测试：写测试 --------------*/
//    /* 打开文件，如果文件不存在则创建它 */
//    printf("****** 即将进行文件写入测试... ******\r\n");
//    f_res = f_open(&SDFile, "AAA.txt", FA_CREATE_ALWAYS|FA_WRITE);
//    if ( f_res == FR_OK ) {
//        printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
//        /* 将指定存储区内容写入到文件内 */
//        f_res=f_write(&SDFile,WriteBuffer,sizeof(WriteBuffer),&fnum);
//        if (f_res==FR_OK) {
//            printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
//            printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
//        } else {
//            printf("！！文件写入失败：(%d)\r\n",f_res);
//        }
//        /* 不再读写，关闭文件 */
//        f_close(&SDFile);
//    } else {
//        printf("！！打开/创建文件失败:%d\r\n",f_res);
//    }
//
//    /*----------- 文件系统测试：读测试 ---------------*/
//	printf("****** 即将进行文件读取测试... ******\r\n");
//	f_res = f_open(&SDFile, "AAA.txt", FA_OPEN_EXISTING | FA_READ);
//	if (f_res == FR_OK) {
//		printf("》打开文件成功。\r\n");
//		f_res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer),&fnum);
//		if (f_res==FR_OK) {
//			printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
//			printf("》读取得的文件数据为：\r\n%s \r\n",ReadBuffer);
//		} else {
//			printf("！！文件读取失败：(%d)\r\n",f_res);
//		}
//	} else {
//		printf("！！打开文件失败。\r\n");
//	}
//	/* 不再读写，关闭文件 */
//	f_close(&SDFile);
//
//	/* 不再使用，取消挂载 */
//	f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
//
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
