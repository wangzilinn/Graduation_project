#ifndef __USART__
#define __USART__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"

/******************************************************************************
@Class: USART

@Description:
    warning: peripheral initialization of the USART class is not configured in 
    the class constructor, it configured by static functions in bsp.cpp before 
    this class is initializated

@Created: by Wang Zilin

@Modified: 2018-3-7 21:36:38 by zilin Wang
******************************************************************************/
class USART
{
    private:
        USART_TypeDef * __USARTx;
    public: 
        static void BSPInit(USART_TypeDef* USARTx);
        USART(USART_TypeDef * USARTx):__USARTx(USARTx){};
        void SendByte(u8 byte) const ;
        void SendArray(u8* array, int length) const ;
        void SendString(const char* string);
        //TODO:operator overloading <<
};
/******************************************************************************
Function declaration
******************************************************************************/
/******************************************************************************
static functions hidden in usart.cpp
******************************************************************************/
static void USART1IOInit(void);
static void USART1Init(void);
static void USART2IOInit(void);
static void USART2Init(void);
static void USART3IOInit(void);
static void USART3Init(void);
#endif


