#ifndef __TRANSPORTLAYER__
#define __TRANSPORTLAYER__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "usart.h"
/******************************************************************************
Function declaration
******************************************************************************/
void SendDataPackage(USART_TypeDef * usart,u16 targetShortAddress,u8* const data, u8 dataLength);
#endif