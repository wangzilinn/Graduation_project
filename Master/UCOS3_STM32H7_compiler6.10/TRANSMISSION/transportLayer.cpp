/******************************************************************************
Include headers
******************************************************************************/
#include "transportLayer.h"
enum
{
    RECEIVE_FIXED_PART,
    RECEIVE_VARIABLE_PART,
    RECEIVE_RESTART
}receiveStatus = RECEIVE_FIXED_PART;
u8 fixedFrameArray[6] = {0};
struct nodeData
{
    u16 localShortAddress;
    float temperature;
    float humidity;
    u8 controlWord;
};
/******************************************************************************
*  @Function: HAL_UART_RxCpltCallback
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-06 16:02 by Wang Zilin
******************************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
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
        //check variable part CRC result
        u16 CRCCheck = Method::ModbusCRC16(Usart2RxBuffer, fixedFrameArray[4]);
        if ((fixedFrameArray[2] << 8 | fixedFrameArray[3]) == CRCCheck)
        { 
            u8 a[fixedFrameArray[4]];
            for (u8 i = 0; i < fixedFrameArray[4]; i++)
            {
                a[i] = Usart2RxBuffer[i];
            }
            struct nodeData *NodeData = (struct nodeData*)a;
            printf("t:%d, h:%d\r\n", (int)NodeData->temperature, (int)NodeData->humidity);
        }
        //no matter check success or fail, restart again
        receiveStatus = RECEIVE_FIXED_PART;
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);                       
    }
}