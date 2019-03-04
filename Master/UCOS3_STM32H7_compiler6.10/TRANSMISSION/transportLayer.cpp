/******************************************************************************
Include headers
******************************************************************************/
#include "transportLayer.h"
/******************************************************************************
Global variable definition
******************************************************************************/
bool receiveFinished = false;
bool isValidPackage = false;
u8 totalSliceNumber = 0;
u8 currentSliceNumber = 0;
u8 uplevelDataBufferCnt = 0;
/******************************************************************************
@Function: bool ZigBeeReceive(u8* const receivedData, u8* const uplevelDataBuffer, int const uplevelDataBufferLength)

@Description:
receivedData:ZigBee receive data's head pointer
uplevelDataBuffer:received uplevel data's buffer
uplevelDataBufferLength:buffer length
return:
receive finished:true
not finish:false
@Created: by Wang Zilin

@Modified: 2019-03-04 21:19 by Wang Zilin
******************************************************************************/
bool ZigBeeReceive(u8* const receivedData, u8* const uplevelDataBuffer, int const uplevelDataBufferLength)
{
    //CRC check first:
    if ((receivedData[16] << 8 | receivedData[17]) == Method::ModbusCRC16(&receivedData[2], 14))//CRC check pass
    {
        u8 receivedCurrentSliceNumber = receivedData[15];
        u8 receivedTotalSliceNumber = receivedData[14];
        if (receivedCurrentSliceNumber == 1)//the first slice
        {
            currentSliceNumber = receivedCurrentSliceNumber;
            totalSliceNumber = receivedTotalSliceNumber;
            uplevelDataBufferCnt = 0;
            isValidPackage = true;
        }
        else
        {
            currentSliceNumber++;//if not the first slice, then check received slice number
            if ((currentSliceNumber != receivedCurrentSliceNumber) || 
                (totalSliceNumber != receivedTotalSliceNumber))// check if lose packages
            {
                u8* earserPointer = uplevelDataBuffer;//earse uplevelDataBuffer
                do
                {
                    *earserPointer++ = 0;
                }while(earserPointer != &uplevelDataBuffer[uplevelDataBufferCnt]);
                isValidPackage = false;
            }
        }
        if (isValidPackage == true)
        {
            for (u8 i = 2; i < 14 && uplevelDataBufferCnt < uplevelDataBufferLength; i++)
            {
                uplevelDataBuffer[uplevelDataBufferCnt++] = receivedData[i];
            }
            if (uplevelDataBufferCnt == uplevelDataBufferLength)//data receive finished
                return true;
        }
    }
    else
    {
        isValidPackage = false;        
    }
    return false;
}