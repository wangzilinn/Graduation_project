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
#include "port.h"
#include "mb.h"//Modbus includes
#include "public.h" // hal support
#include "usart.h"//usart support
/******************************************************************************
static functions declarations 这两个函数仅仅是示例,需要被重新实现
******************************************************************************/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
/******************************************************************************
Start implementation
******************************************************************************/
/******************************************************************************
@Function: vMBPortSerialEnable

@Description:使能串口

@Created: by Wangzilin

@Modified: 2019-03-30 10:06 by Wang Zilin
******************************************************************************/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    if (xRxEnable)
	{
		__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);
	}
	else
	{
		__HAL_UART_DISABLE_IT(&UART1_Handler,UART_IT_RXNE);
	}
	if (xTxEnable)
	{
		__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_TXE);
	}
	else
	{
		__HAL_UART_DISABLE_IT(&UART1_Handler,UART_IT_TXE);
	}
}
/******************************************************************************
@Function: xMBPortSerialInit

@Description:初始化串口,其具体实现被放在了外部,这里仅仅返回真值

@Created: by Wangzilin

@Modified: 2019-03-30 10:07 by Wang Zilin
******************************************************************************/
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    return TRUE;
}
/******************************************************************************
@Function: xMBPortSerialPutByte

@Description:发送一个字节

@Created: by Wangzilin

@Modified: 2019-03-30 10:08 by Wang Zilin
******************************************************************************/
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    if(HAL_UART_Transmit(&UART1_Handler ,(uint8_t *)&ucByte,1,0x01) != HAL_OK )
        return FALSE ;
    else
        return TRUE;
}
/******************************************************************************
@Function: xMBPortSerialGetByte

@Description:接收一个字节

@Created: by Wangzilin

@Modified: 2019-03-30 10:08 by Wang Zilin
******************************************************************************/
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    if(HAL_UART_Receive (&UART1_Handler ,(uint8_t *)pucByte,1,0x01) != HAL_OK )
        return FALSE ;
    else
        return TRUE;
}
/******************************************************************************
@Function: prvvUARTTxReadyISR

@Description:
Create an interrupt handler for the transmit buffer empty interrupt
(or an equivalent) for your target processor. This function should then
call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
a new character can be sent. The protocol stack will then call 
xMBPortSerialPutByte( ) to send the character.
这个函数只是一个示例,并无作用,实际的调用在stm32h7xx_it.c中

@Created: by Wangzilin

@Modified: 2019-03-30 10:09 by Wang Zilin
******************************************************************************/
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}
/******************************************************************************
@Function: prvvUARTRxISR

@Description:
Create an interrupt handler for the receive interrupt for your target
processor. This function should then call pxMBFrameCBByteReceived( ). The
protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
character.
这个函数只是一个示例,并无作用,实际的调用在stm32h7xx_it.c中

@Created: by Wangzilin

@Modified: 2019-03-30 10:10 by Wang Zilin
******************************************************************************/
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}