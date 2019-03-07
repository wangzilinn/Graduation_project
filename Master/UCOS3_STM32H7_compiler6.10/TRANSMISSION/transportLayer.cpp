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
}receiveStatus = RECEIVE_FIXED_PART;
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
    if (receiveStatus == RECEIVE_FIXED_PART)
    {
        if ((framePointer == 0) && (Usart2RxBuffer[0] == (u8)(LOCAL_SHORT_ADDRESS >> 8)))
        {
            fixedFrameArray[framePointer] = Usart2RxBuffer[0];
        }
        else if ((framePointer == 1) && (Usart2RxBuffer[0] == (u8)LOCAL_SHORT_ADDRESS))
        {
            fixedFrameArray[framePointer] = Usart2RxBuffer[0];
        }
        else if ((framePointer >= 2) && (framePointer <=5))
        {
            fixedFrameArray[framePointer] = Usart2RxBuffer[0];
            if (framePointer == 5)//fixed frames receive finished, check fixed frame part
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
                {
                    receiveStatus = RECEIVE_RESTART;
                }
            }
        }
        else
        {
            receiveStatus = RECEIVE_RESTART;
        }
    
        if (receiveStatus == RECEIVE_FIXED_PART)
        {
            framePointer++;
            HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
        }
        else
        {
            if (receiveStatus == RECEIVE_RESTART)
            {
                receiveStatus = RECEIVE_FIXED_PART;
                HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
            }
            framePointer = 0;
        }
    }  
    else if (receiveStatus == RECEIVE_VARIABLE_PART)
    {
        //check variable part CRC result
        u16 CRCCheck = Method::ModbusCRC16(Usart2RxBuffer, fixedFrameArray[4]);
        if ((fixedFrameArray[2] << 8 | fixedFrameArray[3]) == CRCCheck)
        { 
            printf("receive finished\r\n"); 
        }
        //no matter check success or fail, restart again
        receiveStatus = RECEIVE_FIXED_PART;
        framePointer = 0;
        HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);                       
    }
}