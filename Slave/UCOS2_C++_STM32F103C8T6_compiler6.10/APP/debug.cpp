/******************************************************************************
包含头文件
******************************************************************************/
#include "debug.h"
/*****************************************************************************/
// FUNCTION NAME: |
//
// DESCRIPTION:
//  
//
// CREATED:  by zilin Wang
//
/*****************************************************************************/
#ifdef DEBUGMOD
void Debug::Output(char* theInformation)
{
    char* pointer = theInformation;
    for (int i = 0; i < 100; i++, pointer++)
    {
        __debugInformation[i] = *pointer;
        if (*pointer == '\0')
            break;       
    }
    __debugUsart.SendString(__debugInformation);
    __debugUsart.SendString("\n");
}
#endif