/******************************************************************************
Include headers
******************************************************************************/
#include "ITM.h"
/******************************************************************************
本页提供STLINK内建的串口调试功能
******************************************************************************/
/******************************************************************************
Interface definition
******************************************************************************/
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000
//struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
/******************************************************************************
@Function: fputc

@Description:重定义fputc,实现printf

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
int fputc(int ch, FILE *f) 
{
    if (DEMCR & TRCENA) 
    {
        while (ITM_Port32(0) == 0);
        ITM_Port8(0) = ch;
    }
    return(ch);
}