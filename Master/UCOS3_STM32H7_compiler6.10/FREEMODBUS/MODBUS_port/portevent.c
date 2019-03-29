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

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
//2019-03-23 11:53 by Wang Zilin
//#include "includes.h"//UCOS3支持文件
/* ----------------------- Variables ----------------------------------------*/
static eMBEventType eQueuedEvent;//不使用模拟的事件标志组,使用操作系统带的事件标志组
static BOOL     xEventInQueue;
//OS_FLAG_GRP xSlaveOsEvent;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    xEventInQueue = FALSE;
    
     ////////////////////////////
//    OS_ERR err;
//    OSFlagCreate(&xSlaveOsEvent, "modbus slave flag", 0, &err);
    return TRUE;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
//    OS_ERR err;
//    OSFlagPost(&xSlaveOsEvent, eEvent, OS_OPT_POST_FLAG_SET, &err);
    
     ////////////////////////////
    xEventInQueue = TRUE;
    eQueuedEvent = eEvent;
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
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
    
    
    ////////////////////////////
    BOOL            xEventHappened = FALSE;

    if( xEventInQueue )
    {
        *eEvent = eQueuedEvent;
        xEventInQueue = FALSE;
        xEventHappened = TRUE;
    }
    return xEventHappened;
}
