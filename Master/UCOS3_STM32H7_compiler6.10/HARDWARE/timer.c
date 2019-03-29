/******************************************************************************
Include headers
******************************************************************************/
#include "timer.h"
/******************************************************************************
@Function: TimerInit

@Description:定时器初始化 时钟为200MHz,200分频到1MHz,即1us

@Created: by Wang Zilin

@Modified: 2019-03-29 21:25 by Wang Zilin
******************************************************************************/
TIM_HandleTypeDef modbusTimerHandler;
void ModbusTimerInit(int us)
{
    modbusTimerHandler.Instance = TIM3;
    modbusTimerHandler.Init.Prescaler = 200 - 1;
    modbusTimerHandler.Init.CounterMode = TIM_COUNTERMODE_UP;
    modbusTimerHandler.Init.Period = us-1;
    modbusTimerHandler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&modbusTimerHandler);
    //HAL_TIM_Base_Start_IT(&htimx);  //enable timer       
}
/******************************************************************************
@Function: HAL_TIM_Base_MspInit

@Description:定时器硬件初始化

@Created: by Wang Zilin

@Modified: 2019-03-22 21:47 by Wang Zilin
******************************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM3)
    {
        /* 基本定时器外设时钟使能 */
        __HAL_RCC_TIM3_CLK_ENABLE();
        /* 外设中断配置 */
        HAL_NVIC_SetPriority(TIM3_IRQn, 3, 2);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}