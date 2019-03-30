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
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"//standard C library and HAL library
#include "timer.h"//包含定时器初始化函数
/*-----------------------------------------------------------------------------
modbus support
-----------------------------------------------------------------------------*/
#include "port.h"//Platform includes
#include "mb.h"//Modbus includes
/******************************************************************************
Function declaration
******************************************************************************/
static void prvvTIMERExpiredISR( void );//this function needs to be overwriten
/******************************************************************************
Start implementation
******************************************************************************/
/******************************************************************************
@Function: xMBPortTimersInit

@Description:定时器初始化,参数以50us为单位

@Created: by Wangzilin

@Modified: 2019-03-29 21:46 by Wang Zilin
******************************************************************************/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    ModbusTimerInit(usTim1Timerout50us * 50);
    return TRUE;
}
/******************************************************************************
@Function: vMBPortTimersEnable

@Description:启动定时器

@Created: by Wangzilin

@Modified: 2019-03-29 21:47 by Wang Zilin
******************************************************************************/
inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    HAL_TIM_Base_Start_IT(&modbusTimerHandler);
}
/******************************************************************************
@Function: vMBPortTimersDisable

@Description:停止定时器

@Created: by Wangzilin

@Modified: 2019-03-29 21:48 by Wang Zilin
******************************************************************************/
inline void
vMBPortTimersDisable(  )
{
    HAL_TIM_Base_Stop_IT(&modbusTimerHandler);
}
/******************************************************************************
@Function: prvvTIMERExpiredISR

@Description:
Create an ISR which is called whenever the timer has expired. This function
must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
the timer has expired.
这个函数仅是一个示例,需要重新被实现

@Created: by Wangzilin

@Modified: 2019-03-29 21:49 by Wang Zilin
******************************************************************************/
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
