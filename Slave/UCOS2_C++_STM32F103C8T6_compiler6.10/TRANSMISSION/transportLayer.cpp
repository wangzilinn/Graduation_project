/******************************************************************************
Include headers
******************************************************************************/
#include "transportLayer.h"
/******************************************************************************
@Function: void ZigBeeP2P(USART_TypeDef * usart,u16 targetShortAddress, u8* data, int dataLength)

@Description:ZigBee点对点传输
注意:传入数据量需小于等于248Byte

@Created: by Wang Zilin

@Modified: 2019-03-06 15:53 by Wang Zilin
******************************************************************************/
void SendDataPackage(USART_TypeDef * usart,u16 targetShortAddress,u8* const data, u8 dataLength)
{
    u8 frameLength = dataLength + 6;
    u16 CRCResult = Method::ModbusCRC16(data, dataLength);
    //load fixed frame part:
    u8 fixedFrameArray[8] = {0};
    fixedFrameArray[0] = 0x9e;//0x9e is frame's prefix
    fixedFrameArray[1] = frameLength;
    fixedFrameArray[2] = (u8)(targetShortAddress >> 8);
    fixedFrameArray[3] = (u8)(targetShortAddress);
    fixedFrameArray[4] = (u8)(CRCResult >> 8);
    fixedFrameArray[5] = (u8)(CRCResult);
    fixedFrameArray[6] = dataLength;
    for (u8 i = 2; i < 7; i++)
        fixedFrameArray[7] += fixedFrameArray[i];
    USART ZigBeeSender(usart);
    ZigBeeSender.SendArray(fixedFrameArray,8);
    ZigBeeSender.SendArray(data,dataLength);
}