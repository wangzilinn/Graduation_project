#ifndef __DEBUG__
#define __DEBUG__
/******************************************************************************
包含头文件
******************************************************************************/
#include "public.h"
#include "usart.h"
/*****************************************************************************/
// CLASS NAME: 
//
// DESCRIPTION:
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
#ifdef DEBUGMOD
class Debug
{
private:
    USART __debugUsart;
    char __debugInformation[100];
public:
    Debug(USART_TypeDef* debugUsart):__debugUsart(debugUsart)
    {
        Method::ClearStringBuff(__debugInformation, 100);
    }
    void Output(char* information);
};
#endif
#endif