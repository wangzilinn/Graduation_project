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
在源代码中实现了两个变量实现的消息邮箱,或者称长度为1的消息队列,而在操作系统中最
好使用事件标志组来实现消息的传递,但是我的实现(被注释掉的)并不能正常运行,所以暂时
还是使用自带的消息邮箱
******************************************************************************/
/******************************************************************************
Include headers
******************************************************************************/
#include "mb.h"
#include "includes.h"//mutex support
/******************************************************************************
Global variable definition
******************************************************************************/
/*-----------------------------------------------------------------------------
使用两个变量模拟实现消息邮箱
-----------------------------------------------------------------------------*/
OS_MUTEX xModbusEventMutex;
static eMBEventType eQueuedEvent;
static BOOL     xEventInQueue;
/*-----------------------------------------------------------------------------
不使用模拟的消息邮箱,使用操作系统带的事件标志组
-----------------------------------------------------------------------------*/
//OS_FLAG_GRP xSlaveOsEvent;
/* ----------------------- Start implementation -----------------------------*/
/******************************************************************************
@Function: xMBPortEventInit

@Description:事件队列初始化

@Created: by Wangzilin

@Modified: 2019-03-29 22:04 by Wang Zilin
******************************************************************************/
BOOL
xMBPortEventInit( void )
{
    OS_ERR err;
    OSMutexCreate(&xModbusEventMutex, (char*)"access modbus event Mutex", &err);
    //使用模拟消息邮箱
    OSMutexPend(&xModbusEventMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        xEventInQueue = FALSE;
    OSMutexPost(&xModbusEventMutex, OS_OPT_POST_NONE, &err);
    //使用事件标志组:
//    OS_ERR err;
//    OSFlagCreate(&xSlaveOsEvent, "modbus slave flag", 0, &err);
    return TRUE;
}
/******************************************************************************
@Function: xMBPortEventPost

@Description:向事件队列中发送一个事件

@Created: by Wangzilin

@Modified: 2019-03-29 22:05 by Wang Zilin
******************************************************************************/
BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    //使用模拟消息邮箱
    OS_ERR err;
    OSMutexPend(&xModbusEventMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        xEventInQueue = TRUE;
        eQueuedEvent = eEvent;
    OSMutexPost(&xModbusEventMutex, OS_OPT_POST_NONE, &err);
    //使用事件标志组
//    OS_ERR err;
//    OSFlagPost(&xSlaveOsEvent, eEvent, OS_OPT_POST_FLAG_SET, &err);   
    return TRUE;
}
/******************************************************************************
@Function: xMBPortEventGet

@Description:从事件队列中获取事件

@Created: by Wangzilin

@Modified: 2019-03-29 22:05 by Wang Zilin
******************************************************************************/
BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    OS_ERR err;
    OSMutexPend(&xModbusEventMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        //使用模拟消息邮箱
        BOOL xEventHappened = FALSE;

        if( xEventInQueue )
        {
            *eEvent = eQueuedEvent;
            xEventInQueue = FALSE;
            xEventHappened = TRUE;
        }
    OSMutexPost(&xModbusEventMutex, OS_OPT_POST_NONE, &err);         
        
    return xEventHappened;
    //使用事件标志组
//    OS_ERR err;
//    OS_FLAGS recvedEvent;
//    recvedEvent = OSFlagPend(&xSlaveOsEvent, EV_READY | EV_FRAME_RECEIVED | EV_EXECUTE | EV_FRAME_SENT, 
//							 OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_BLOCKING, 0, NULL, &err);
//    switch (recvedEvent)
//	{
//	case EV_READY:
//		*eEvent = EV_READY;
//		break;
//	case EV_FRAME_RECEIVED:
//		*eEvent = EV_FRAME_RECEIVED;
//		break;
//	case EV_EXECUTE:
//		*eEvent = EV_EXECUTE;
//		break;
//	case EV_FRAME_SENT:
//		*eEvent = EV_FRAME_SENT;
//		break;
//    }
//    return TRUE;
}
