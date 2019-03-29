#include "timer.h"
/******************************************************************************
@Function: 

@Description:定时器初始化

@Created: by Wang Zilin

@Modified: 2019-03-22 21:39 by Wang Zilin
******************************************************************************/
TIM_HandleTypeDef htimx;
void TimerInit(int us)
{

  htimx.Instance = TIM3;
  htimx.Init.Prescaler = 200 - 1;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = us-1;
  htimx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimx);
    //HAL_TIM_Base_Start_IT(&htimx);  //enable timer
       
}
/******************************************************************************
@Function: 

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
/******************************************************************************
@Function: 

@Description:定时器默认初始化

@Created: by Wang Zilin

@Modified: 2019-03-22 21:48 by Wang Zilin
******************************************************************************/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

//  if(htim_base->Instance==GENERAL_TIMx)
//  {
//    /* 基本定时器外设时钟禁用 */
//    GENERAL_TIM_RCC_CLK_DISABLE();

//    /* 关闭外设中断 */
//    HAL_NVIC_DisableIRQ(GENERAL_TIM_IRQ);
//  }
} 