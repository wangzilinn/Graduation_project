#ifndef __METHOD_H
#define __METHOD_H
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"
/******************************************************************************
@Class: Method

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 19:59 by Wang Zilin
******************************************************************************/
typedef union
{
    float fdata;
    u8 u8Data[4];
}floatOr4ByteTypeDef;
typedef union
{
    int idata;
    u8 u8Data[4];
}intOr4ByteTypeDef;
struct Method
{
    static void ClearStringBuff(u8* str, int length);
    static void ClearStringBuff(char* str, int length);
    static int U8ToInt(u8 * headPointer);
    static float U8ToFloat(u8 * headPointer);
    static void FloatToU8(float floatNumber, u8* u8Array);
    static void U16ToU8(u16 u16Number, u8* u8Array);
    static u16 U8ToU16(u8 * headPointer);
    static void DelayUs(u32 time);
    static void DelayMs(u32 time);
    static uint16_t ModbusCRC16(uint8_t *puchMsg, uint8_t usDataLen);
    static void BytewiseShiftLeft(u8* array, int arrayLength);
};

#endif
