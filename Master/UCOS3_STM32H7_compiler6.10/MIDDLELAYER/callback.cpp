/******************************************************************************
本页实现工程内的所有回调函数
******************************************************************************/
/******************************************************************************
Include headers
******************************************************************************/
#include "callback.h"
/******************************************************************************
*  @Function:HAL_UART_RxCpltCallback
*
*  @Description:串口接收中断回调函数
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 11:17 by Wang Zilin
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        ZigBeeReceiveCallBack();
    }
}
/******************************************************************************
*  @Function: HAL_TIM_PeriodElapsedCallback
*
*  @Description:定时器中断回调函数
*
*  @Created: by Wang Zilin
*
*  @Modified: 2019-03-22 22:21 by Wang Zilin
******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        pxMBPortCBTimerExpired();//调用Modbus的超时回调函数
    }
}