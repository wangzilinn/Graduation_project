/******************************************************************************
Include headers
******************************************************************************/
#include "transportLayer.h"
/******************************************************************************
@Function: void ZigBeeP2P(USART_TypeDef * usart,u16 targetShortAddress, u8* data, int dataLength)

@Description:ZigBee点对点传输
注意:传入数据量需小于等于3060Byte

@Created: by Wang Zilin

@Modified: 2019-03-03 19:30 by Wang Zilin
******************************************************************************/
void ZigBeeP2P(USART_TypeDef * usart,u16 targetShortAddress,u8* const data, int dataLength)
{
    if (dataLength >= 3060)//12Byte(max length of single slice) * 255(max number of slices)
        return;      
    u8 packageArray[20] = {[0] = 0x9e, [1] = 0x18};//the first Byte is 0x9E, the second Byte is data length(include short address), aka 18
    packageArray[2] = (u8)targetShortAddress >> 8;
    packageArray[3] = (u8)targetShortAddress;    
    ZigBeeSendPackage(usart, packageArray, data, dataLength);
}
/******************************************************************************
@Function: void ZigBeeBroadcast(USART_TypeDef * usart, u8* const data, const int dataLength);

@Description:ZigBee 广播

@Created: by Wang Zilin

@Modified: 2019-03-03 20:29 by Wang Zilin
******************************************************************************/
void ZigBeeBroadcast(USART_TypeDef * usart, u8* const data, const int dataLength)
{
    if (dataLength >= 3060)//12Byte(max length of single slice) * 255(max number of slices)
        return;      
    u8 packageArray[20] = {[0] = 0x9e, [1] = 0x18, [2] = 0x00, [3] = 0x00};//the first Byte is 0x9E, the second Byte is data length(include short address), aka 18
    ZigBeeSendPackage(usart, packageArray, data, dataLength);
}
/******************************************************************************
@Function: 

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void ZigBeeSendPackage(USART_TypeDef * usart, u8* packageArray, u8* const data, const int dataLength)
{
    u8 totalSliceNumber = (dataLength % 12 == 0 ? packageArray[16] = dataLength / 12 : packageArray[16] = dataLength / 12 + 1);//the 16 Byte is total number of slice
    u8* dataPoninter = data;
    int sliceNumber = 1;
    do
    {
        for (u8 packageArrayPointer = 4; packageArrayPointer <= 15;  packageArrayPointer++)
        {
            packageArray[packageArrayPointer] = *dataPoninter++;
        }
        packageArray[17] = sliceNumber++;
        u16 CRCResult = Method::ModbusCRC16(&packageArray[4], 14);//CRC from the 5 byte to the 18 byte,aka [4...17]
        packageArray[18] = (u8)(CRCResult >> 8);
        packageArray[19] = (u8)CRCResult;
        USART ZigBeeUsart(usart);
        ZigBeeUsart.SendArray(packageArray, 20);
    }
    while(sliceNumber < totalSliceNumber);
}