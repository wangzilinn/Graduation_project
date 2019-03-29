#ifndef __TIMER__
#define __TIMER__
#include "public.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/********************通用定时器TIM参数定义，TIM2~TIM5************/
//#define GENERAL_TIMx                     TIM2
//#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM2_CLK_ENABLE()
//#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM2_CLK_DISABLE()
//#define GENERAL_TIM_IRQ                  TIM2_IRQn
//#define GENERAL_TIM_INT_FUN              TIM2_IRQHandler

#define GENERAL_TIMx                     TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM3_CLK_DISABLE()
#define GENERAL_TIM_IRQ                  TIM3_IRQn
#define GENERAL_TIM_INT_FUN              TIM3_IRQHandler

//#define GENERAL_TIMx                     TIM4
//#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM4_CLK_ENABLE()
//#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM4_CLK_DISABLE()
//#define GENERAL_TIM_IRQ                  TIM4_IRQn
//#define GENERAL_TIM_INT_FUN              TIM4_IRQHandler

//#define GENERAL_TIMx                     TIM5
//#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM5_CLK_ENABLE()
//#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM5_CLK_DISABLE()
//#define GENERAL_TIM_IRQ                  TIM5_IRQn
//#define GENERAL_TIM_INT_FUN              TIM5_IRQHandler

#define GENERAL_TIM_PRESCALER           (200-1) 
#define GENERAL_TIM_PERIOD              50


/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;

/* 函数声明 ------------------------------------------------------------------*/

void TimerInit(int us);
#endif