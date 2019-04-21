/******************************************************************************
Include headers
******************************************************************************/
#include "communication_protocol_stack_module.h"
/******************************************************************************
enum definition
******************************************************************************/

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
        printf("1");
        //放入接受队列
        Method::BytewiseShiftLeft(fixedFrameArray, 6);
        fixedFrameArray[5] = Usart2RxBuffer[0];
        //检查队列中的值:
        if ((fixedFrameArray[0] == (u8)(LOCAL_SHORT_ADDRESS >> 8)) && (fixedFrameArray[1] == (u8)(LOCAL_SHORT_ADDRESS)))
        {
            TogglePilotLED(5);
            u8 sum = 0;
            for (u8 i = 0; i < 5; i++)
            {
                sum += fixedFrameArray[i];
            }
            if (sum == fixedFrameArray[5])//sum check passed
            {
                if (HAL_ERROR == HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, fixedFrameArray[4]))
                {
                    printf("err1");
                }
                //start receive variable frame part
                receiveStatus = RECEIVE_VARIABLE_PART;
                return;
            }
        }
        if (HAL_ERROR == HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1))
        {
            printf("err3\r\n");
        }
    }  
    else if (receiveStatus == RECEIVE_VARIABLE_PART)
    {
        TogglePilotLED(4);
        printf("2\r\n");
        u8 structLength = fixedFrameArray[4];
        //check variable part CRC result
        u16 CRCCheck = Method::ModbusCRC16(Usart2RxBuffer, structLength);
        if ((fixedFrameArray[2] << 8 | fixedFrameArray[3]) == CRCCheck)
        { 
            memcpy(&receivedNodeDataBuffer[receivedNodeDataBufferPointer], Usart2RxBuffer, structLength);
            //发送到消息队列
            OS_ERR err;
            OSTaskQPost(&ReceiveDataTaskTCB, &receivedNodeDataBuffer[receivedNodeDataBufferPointer++], structLength, OS_OPT_POST_FIFO, &err); 
            if (receivedNodeDataBufferPointer >= RECEIVED_NODE_DATA_BUFFER_LENGTH)
            {
                receivedNodeDataBufferPointer = 0;
            }           
        }
        //no matter check success or fail, restart again
        receiveStatus = RECEIVE_FIXED_PART;
        //清空所有缓冲区，准备重新接收
        Method::ClearStringBuff(fixedFrameArray, 6);
        Method::ClearStringBuff(Usart2RxBuffer, structLength);
        if (HAL_ERROR == HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1))   
        {
            printf("err2\r\n");
        }        
    }
}
