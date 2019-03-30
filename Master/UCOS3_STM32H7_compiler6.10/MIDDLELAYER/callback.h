#ifndef __CALLBACK__
#define __CALLBACK__
#ifdef __cplusplus
extern "C" { 
#endif
/******************************************************************************
Include headers
******************************************************************************/
#include "transport_layer.h"//ZIgBee接收串口回调support
#include "mb.h"//Modbus定时器回调
/******************************************************************************
Function declarations
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif
#endif