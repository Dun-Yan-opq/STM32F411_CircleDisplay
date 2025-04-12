#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_port, _pin, _value) HAL_GPIO_WritePin(_port, _pin, _value == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET)
#define DEV_Digital_Read(_pin) HAL_GPIO_ReadPin(_pin)

int DEV_Module_Init(void);

#ifdef __cplusplus
}
#endif

#endif
