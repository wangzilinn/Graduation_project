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
void SendDataPackage(USART_TypeDef * usart,const u16 targetShortAddress, u8* const data, const int dataLength);
#endif