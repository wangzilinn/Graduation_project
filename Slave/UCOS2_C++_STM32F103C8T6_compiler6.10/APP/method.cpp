/******************************************************************************
Include header files
******************************************************************************/
#include "method.h"


/******************************************************************************
@Function: ClearStringBuff

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 19:57 by Wang Zilin
******************************************************************************/
void Method::ClearStringBuff(u8* str, int length)
{
    if (str == NULL)
        return;
	for(u8 i = 0; i < length; i++)
	{
		str[i] = '\0';
	}
}

/******************************************************************************
@Function: ClearStringBuff

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 19:57 by Wang Zilin
******************************************************************************/
void Method::ClearStringBuff(char* str, int length)
{
    if (str == NULL)
        return;
	for(u8 i = 0; i < length; i++)
	{
		str[i] = '\0';
	}
}
/****************************************************.;**************************
@Function: ByteToInt

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 19:57 by Wang Zilin
******************************************************************************/
int Method::ByteToInt(u8 * headPointer)
{
    intOr4ByteTypeDef byteToInt;
    for (u8 i = 0; i < 4; i++,headPointer++)
    {
        byteToInt.u8Data[3 - i] = *headPointer;         
    }
    return byteToInt.idata;
}

/******************************************************************************
@Function: DelayUs

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-05 16:39 by Wang Zilin
******************************************************************************/
void Method::DelayUs(u32 nTime)
{
	while(nTime--)
    {
		u16 i = 10 ;
		while(i--)
        {
            __ASM("nop");//Prevent "while" loop from being optimized, Optimization level1(O1) checked
        }
	}
}
/*****************************************************************************/
// FUNCTION NAME: void Method::DelayMs(u32 time)
//
// DESCRIPTION:
//  
//
// CREATED:  by zilin Wang
// MODIFIED:2018-11-05 16:39 by Wang Zilin
//
/*****************************************************************************/
void Method::DelayMs(u32 time)
{
    for (int i = 0; i < time; i++)
        Method::DelayUs(1000);
}

