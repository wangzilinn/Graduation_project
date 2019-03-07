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
/******************************************************************************
@Function: int Method::U8ToInt(u8 * headPointer)

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-04 21:49 by Wang Zilin
******************************************************************************/
int Method::U8ToInt(u8 * headPointer)
{
    intOr4ByteTypeDef byteToInt;
    for (u8 i = 0; i < 4; i++,headPointer++)
    {
        byteToInt.u8Data[3 - i] = *headPointer;         
    }
    return byteToInt.idata;
}
/******************************************************************************
@Function: float Method::U8ToFloat(u8 * headPointer)

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-04 21:49 by Wang Zilin
******************************************************************************/
float Method::U8ToFloat(u8 * headPointer)
{
    floatOr4ByteTypeDef u8ToFloat;
    for (u8 i = 0; i < 4; i++,headPointer++)
    {
        u8ToFloat.u8Data[3 - i] = *headPointer;         
    }
    return u8ToFloat.fdata;
}
/******************************************************************************
@Function: static void FloatToU8(float floatNumber, u8* u8Array);

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-03 21:01 by Wang Zilin
******************************************************************************/
void Method::FloatToU8(float floatNumber, u8* u8Array)
{
    floatOr4ByteTypeDef floatToU8;
    floatToU8.fdata = floatNumber;
    for (u8 i = 0; i < 4; i++)
    {
        u8Array[i] = floatToU8.u8Data[i];
    }
}
/******************************************************************************
@Function: u16 U8ToU16(u8 * headPointer)

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-04 21:57 by Wang Zilin
******************************************************************************/
u16 Method::U8ToU16(u8 * headPointer)
{
    return (headPointer[1] << 8 ) | (*headPointer);
}
/******************************************************************************
@Function: static void U16ToU8(u16 u16Number, u8* u8Array);

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-04 22:01 by Wang Zilin
******************************************************************************/
void Method::U16ToU8(u16 u16Number, u8* u8Array)
{
    u8Array[0] = (u8)u16Number;
    u8Array[1] = (u8)(u16Number >> 8);
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
/******************************************************************************
@Function: void Method::DelayMs(u32 time)

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-05 16:39 by Wang Zilin
******************************************************************************/
void Method::DelayMs(u32 time)
{
    for (int i = 0; i < time; i++)
        Method::DelayUs(1000);
}
/******************************************************************************
@Function: uint16_t Method::ModbusCRC16(uint8_t *puchMsg, uint8_t usDataLen)

@Description:函数功能：逐位计算法CRC16校验，在Modbus中CRC结果要进行高低字节交换，即低字节在前，高字节在后
//入口参数：puchMsg是要进行CRC校验的消息；usDataLen是消息中字节数
//出口参数：计算出来的CRC校验码，16位长度

@Created: by Xiao Haihui 

@Modified: 2019-03-03 20:18 by Wang Zilin
******************************************************************************/
uint16_t Method::ModbusCRC16(uint8_t *puchMsg, uint8_t usDataLen)
{
	uint16_t CRC_Cal = 0xFFFF;
	uint8_t CRC_High, CRC_Low;
	uint8_t i, j;
	
	for(j = 0; j < usDataLen; j++)
	{
		CRC_Cal = CRC_Cal ^ *puchMsg++;
		
		for (i = 0; i < 8; i++)
		{
			if((CRC_Cal & 0x0001) == 0x0001)
			{
				CRC_Cal = CRC_Cal >> 1;
				CRC_Cal = CRC_Cal ^ 0xA001;
			}
			else
			{
				CRC_Cal = CRC_Cal >> 1;
			}
		}
	}
	
	CRC_High = (uint8_t)(CRC_Cal >> 8);
	CRC_Low = (uint8_t)(CRC_Cal & 0x00FF);
	
	return (CRC_Low << 8 | CRC_High);
}
/******************************************************************************
@Function: void Method::BytewiseShiftLeft(u8* array, int arrayLength)

@Description:将数组元素左移一个Byte, 最右边一位补0

@Created: by Wang Zilin

@Modified: 2019-03-07 11:40 by Wang Zilin
******************************************************************************/
void Method::BytewiseShiftLeft(u8* array, int arrayLength)
{
    for (int i = 0; i < arrayLength - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[arrayLength - 1] = 0;
}