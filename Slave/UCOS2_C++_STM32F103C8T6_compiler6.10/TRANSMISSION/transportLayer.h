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
static void ZigBeeSendPackage(USART_TypeDef * usart, u8* packageArray, u8* const data, const int dataLength);
void ZigBeeP2P(USART_TypeDef * usart,const u16 targetShortAddress, u8* const data, const int dataLength);
void ZigBeeBroadcast(USART_TypeDef * usart, u8* const data, const int dataLength);
#endif