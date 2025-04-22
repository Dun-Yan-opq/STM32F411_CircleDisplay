#include "Touch_Driver.h"

volatile uint8_t touch_interrupt_flag=0;

Touch_1IN28_XY XY = {
    .mode = 0,
    .Gesture = 0,
    .color = 0xFFFF,     // 例如白色
    .x_point = 0,
    .y_point = 0
};

/******************************************************************************
function :	screen initialization
parameter:
******************************************************************************/
uint8_t Touch_1IN28_init(uint8_t mode)
{
	uint8_t bRet,Rev;
    Touch_1IN28_Reset();
	while(1)
	{
		bRet = Touch_1IN28_WhoAmI();

		if (bRet)
		{
			printf("Success:Detected CST816T.\r\n");
			Rev = Touch_1IN28_Read_Revision();
			printf("CST816T Revision = %d\r\n",Rev);
			Touch_1IN28_Stop_Sleep();
			break;
		}
		else
		{
			HAL_Delay(10);
			printf("Error: Not Detected CST816T.\r\n");
			return false;
		}
	}

    XY.mode = mode;
    Touch_1IN28_Set_Mode(mode);

    XY.x_point = 0;
    XY.y_point = 0;
    return true;
}

/******************************************************************************
function :	reset touch
parameter:
******************************************************************************/
void Touch_1IN28_Reset(void)
{
    HAL_GPIO_WritePin(TP_RST_GPIO_Port,TP_RST_Pin, 0);
    HAL_Delay(100);
    HAL_GPIO_WritePin(TP_RST_GPIO_Port,TP_RST_Pin, 1);
    HAL_Delay(100);
}

/******************************************************************************
function :	read ID
parameter:  CST816T : 0xA7
******************************************************************************/
uint8_t Touch_1IN28_WhoAmI(void)
{
	uint8_t reg_addr = ChipID; // register 地址
	uint8_t rx_data[1]={0};

	if(HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, &reg_addr, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		HAL_I2C_Master_Receive(&hi2c1, Touch_DEV_address, rx_data, sizeof(rx_data), HAL_MAX_DELAY);
		printf("CST816T:%d\r\n",rx_data[0]);
	    if (rx_data[0] == 0xB5)
	        return true;
	    else
	        return false;
	}else{
		printf("XXX\r\n");
		return false;
	}
}

/******************************************************************************
function :	Read software version number
parameter:
******************************************************************************/
uint8_t Touch_1IN28_Read_Revision(void)
{
	uint8_t reg_addr = FwVersion; // register 地址
	uint8_t rx_buf[1];

	HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, &reg_addr, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, Touch_DEV_address, rx_buf, sizeof(rx_buf), HAL_MAX_DELAY);
    return rx_buf[0];
}

/******************************************************************************
function :	exit sleep mode 退出休眠模式
parameter:
******************************************************************************/
void Touch_1IN28_Stop_Sleep(void)
{
    uint8_t tx_data[2] = {DisAutoSleep, 0x01}; // Register + 資料
    HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
    if (ret == HAL_OK) {
        // 傳送成功
    } else {
        // 傳送失敗
    }
}

/******************************************************************************
function :	Set touch mode 設置觸摸模式
parameter:
        mode = 0 gestures mode
        mode = 1 point mode
        mode = 2 mixed mode
******************************************************************************/
void Touch_1IN28_Set_Mode(uint8_t mode)
{
	uint8_t tx_data[2]; // Register + 資料
	HAL_StatusTypeDef ret;
    if (mode == 1)
    {
    	tx_data[0]=IrqCtl;tx_data[1]=0X41;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);

		//Normal fast detection cycle unit 10ms
		tx_data[0]=NorScanPer;tx_data[1]=0X01;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);

		//Interrupt low pulse output width 1.5MS  0x1c
		tx_data[0]=IrqPluseWidth;tx_data[1]=0x0f;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
    }
    else if(mode == 2)
    {
		tx_data[0]=IrqCtl;tx_data[1]=0X71;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
    }
    else
	{
		tx_data[0]=IrqCtl;tx_data[1]=0X11;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
		tx_data[0]=NorScanPer;tx_data[1]=0X01;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
		//中斷低脈衝輸出寬度 1.5MS  0x1c
		tx_data[0]=IrqPluseWidth;tx_data[1]=0x01;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
		//Enable double-tap mode
		tx_data[0]=MotionMask;tx_data[1]=EnDClick;
		ret = HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, tx_data, 2, HAL_MAX_DELAY);
	}
}

/******************************************************************************
function :	Get the corresponding point coordinates 獲取對應的點座標
parameter:
******************************************************************************/
Touch_1IN28_XY Touch_1IN28_Get_Point(void)
{
	XY.x_point = 0;

	uint8_t reg_addr = XposH; // register 地址
	uint8_t rx_data[4]={0};

	if(HAL_I2C_Master_Transmit(&hi2c1, Touch_DEV_address, &reg_addr, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		HAL_I2C_Master_Receive(&hi2c1, Touch_DEV_address, rx_data, sizeof(rx_data), HAL_MAX_DELAY);
	    XY.x_point = ((rx_data[0] & 0x0f)<<8) + rx_data[1];
	    XY.y_point = ((rx_data[2] & 0x0f)<<8) + rx_data[3];
//		printf("X:%d\r\n",XY.x_point);
//		printf("Y:%d\r\n",XY.y_point);
	}
    return XY;
}

uint8_t Touch_I2C_read_one_byte(uint8_t device_address,uint8_t reg_addr, uint8_t* rx_data, uint8_t response_size)
{
	if(HAL_I2C_Master_Transmit(&hi2c1, device_address, &reg_addr, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		HAL_I2C_Master_Receive(&hi2c1, device_address, rx_data, response_size, HAL_MAX_DELAY);
		return true;
	}
	return false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_10)
  {
	  if(XY.mode == 1) // 0: gestures mode ,1: point mode ,2: mixed mode
	  {
		  XY = Touch_1IN28_Get_Point();
	  }
	  touch_interrupt_flag = TOUCH_IRQ;
  }
}
