/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/////////////2019-03-22 22:17 by Wang Zilin
#include "public.h"//hal
#include "timer.h"//包含定时器初始化函数

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    TimerInit(usTim1Timerout50us * 50);
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    //2019-03-22 22:20 by Wang Zilin
//    __HAL_TIM_CLEAR_IT(&htimx,TIM_IT_UPDATE);
//    __HAL_TIM_SetCounter(&htimx,0);
  
  /* 在中断模式下启动定时器 */
    HAL_TIM_Base_Start_IT(&htimx);
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
    //2019-03-22 22:20 by Wang Zilin
    HAL_TIM_Base_Stop_IT(&htimx);
  
//  __HAL_TIM_SetCounter(&htimx,0);
//  __HAL_TIM_CLEAR_IT(&htimx,TIM_IT_UPDATE);  
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
/******************************************************************************
@Function: 

@Description:定时器中断回掉函数

@Created: by Wang Zilin

@Modified: 2019-03-22 22:21 by Wang Zilin
******************************************************************************/
u8 test1 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        pxMBPortCBTimerExpired();
        //test1 = (test1 == 0 ? 1 : 0); 
    }
}
