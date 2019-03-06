/******************************************************************************
Include headers
******************************************************************************/
#include "transportLayer.h"
u8 framePointer = 0;
enum
{
    RECEIVE_FIXED_PART,
    RECEIVE_VARIABLE_PART,
    RECEIVE_RESTART
}receiveStatus;
u8 fixedFrameArray[6] = {0};
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
    if ((framePointer == 0) && (Usart2RxBuffer[0] == (u8)(LOCAL_SHORT_ADDRESS >> 8)))
    {
        fixedFrameArray[framePointer++] = Usart2RxBuffer[0];
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
        receiveStatus = RECEIVE_FIXED_PART;
    }
    else
        receiveStatus = RECEIVE_RESTART;
    if ((framePointer == 1) && (Usart2RxBuffer[0] == (u8)LOCAL_SHORT_ADDRESS))
    {
        fixedFrameArray[framePointer++] = Usart2RxBuffer[0];
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
    }
    else
        receiveStatus = RECEIVE_RESTART;
    if (framePointer >= 2 && framePointer <=5)
    {
        fixedFrameArray[framePointer++] = Usart2RxBuffer[0];
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
    }
    //fixed frames receive finished, check fixed frame part
    if (framePointer > 5 && receiveStatus == RECEIVE_FIXED_PART)
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
        }
        else
            receiveStatus = RECEIVE_RESTART;
    }
    else if (receiveStatus == RECEIVE_VARIABLE_PART)
    {
        //check variable part CRC result
        u16 CRCCheck = Method::ModbusCRC16(Usart2RxBuffer, fixedFrameArray[4]);
        if ((fixedFrameArray[2] << 8 | fixedFrameArray[3]) == CRCCheck)
        { 
            printf("receive finished");          
        }
        receiveStatus = RECEIVE_RESTART;
    }
    if (receiveStatus == RECEIVE_RESTART)
    {
        framePointer = 0;  
        receiveStatus = RECEIVE_FIXED_PART;
    }
}