#ifndef __TRANSPORTLAYER__
#define __TRANSPORTLAYER__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "method.h"
/******************************************************************************
Function declaraction 
******************************************************************************/
bool ZigBeeReceive(u8* const receivedData, u8* const uplevelDataBuffer, int const uplevelDataBufferLength);
#endif
