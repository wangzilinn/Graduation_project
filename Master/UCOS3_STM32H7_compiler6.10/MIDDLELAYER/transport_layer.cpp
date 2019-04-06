/******************************************************************************
Include headers
******************************************************************************/
#include "transport_layer.h"
/******************************************************************************
enum definition
******************************************************************************/
enum receiveStatusEnum
{
    RECEIVE_FIXED_PART = 0,
    RECEIVE_VARIABLE_PART,
};
enum receiveStatusEnum receiveStatus = RECEIVE_FIXED_PART;
/******************************************************************************
Global variable definition
******************************************************************************/
/*-----------------------------------------------------------------------------
帧固定部分缓冲验证区
-----------------------------------------------------------------------------*/
u8 fixedFrameArray[6] = {0};
/******************************************************************************
*  @Function: ZigBeeReceiveCallBack
*
*  @Description:ZigBee接收中断回调函数
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-06 16:02 by Wang Zilin
******************************************************************************/
void ZigBeeReceiveCallBack()
{
    if (receiveStatus == RECEIVE_FIXED_PART)
    {
        //放入接受队列
        Method::BytewiseShiftLeft(fixedFrameArray, 6);
        fixedFrameArray[5] = Usart2RxBuffer[0];
        //检查队列中的值:
        if (fixedFrameArray[0] == (u8)(LOCAL_SHORT_ADDRESS >> 8) && fixedFrameArray[1] == (u8)(LOCAL_SHORT_ADDRESS))
        {
            u8 sum = 0;
            for (u8 i = 0; i < 5; i++)
            {
                sum += fixedFrameArray[i];
            }
            if (sum == fixedFrameArray[5])//sum check passed
            {
                HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, fixedFrameArray[4]);//start receive variable frame part
                receiveStatus = RECEIVE_VARIABLE_PART;
                return;
            }
        }
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
    }  
    else if (receiveStatus == RECEIVE_VARIABLE_PART)
    {
        u8 structLength = fixedFrameArray[4];
        //check variable part CRC result
        u16 CRCCheck = Method::ModbusCRC16(Usart2RxBuffer, structLength);
        if ((fixedFrameArray[2] << 8 | fixedFrameArray[3]) == CRCCheck)
        { 
            memcpy(&nodeDataBuffer[nodeDataBufferPointer], Usart2RxBuffer, structLength);
            //发送到消息队列
            OS_ERR err;
            OSTaskQPost(&ReceiveDataTaskTCB, &nodeDataBuffer[nodeDataBufferPointer++], structLength, OS_OPT_POST_FIFO, &err); 
            if (nodeDataBufferPointer >= RECEIVED_NODE_DATA_BUFFER_LENGTH)
            {
                nodeDataBufferPointer = 0;
            }           
        }
        //no matter check success or fail, restart again
        receiveStatus = RECEIVE_FIXED_PART;
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);                       
    }
}