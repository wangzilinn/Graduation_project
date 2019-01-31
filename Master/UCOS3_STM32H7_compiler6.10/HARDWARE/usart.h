#ifndef _USART_H
#define _USART_H
/******************************************************************************
Include headers
******************************************************************************/	
#include "public.h"

#define USART1_RX_BUFFER_LENGTH  3
#define USART2_RX_BUFFER_LENGTH  3
#define USART3_RX_BUFFER_LENGTH  3

extern u8 Usart1RxBuffer[];
extern u8 Usart2RxBuffer[];
extern u8 Usart3RxBuffer[];
extern UART_HandleTypeDef UART1_Handler;
extern UART_HandleTypeDef UART2_Handler;
extern UART_HandleTypeDef UART3_Handler;

void Uart1Init(u32 baudRate);
void Uart2Init(u32 baudRate);
void Uart3Init(u32 baudrate);
#endif
