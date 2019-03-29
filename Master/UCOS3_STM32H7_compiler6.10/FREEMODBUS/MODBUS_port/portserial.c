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

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
//2019-03-22 22:30 by Wang Zilin
#include "public.h" // hal support
#include "usart.h"//usart support

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
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

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    return TRUE;//2019-03-22 22:28 by Wang Zilin 串口初始化在外部实现
}

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
u8 testReceiveBuffer[20];
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    static u8 testcnt = 0;
    HAL_StatusTypeDef err;
    HAL_UART_Receive (&UART1_Handler ,(uint8_t *)&testReceiveBuffer[testcnt],1,0x01);
    *pucByte = testReceiveBuffer[testcnt++];
    if(testcnt == 8)
        testcnt = 0;
    if (err == HAL_OK)
        return TRUE;
    else
        return FALSE;
//    if(HAL_UART_Receive (&UART1_Handler ,(uint8_t *)pucByte,1,0x01) != HAL_OK )
//        return FALSE ;
//    else
//        return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
