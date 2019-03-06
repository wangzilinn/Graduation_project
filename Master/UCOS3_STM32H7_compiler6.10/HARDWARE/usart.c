/******************************************************************************
*  Include headers
******************************************************************************/
#include "usart.h"

/******************************************************************************
*  Variable definitions
******************************************************************************/
UART_HandleTypeDef UART1_Handler;
UART_HandleTypeDef UART2_Handler;
UART_HandleTypeDef UART3_Handler;
u8                 Usart1RxBuffer[USART1_RX_BUFFER_LENGTH];
u8                 Usart2RxBuffer[USART2_RX_BUFFER_LENGTH];
u8                 Usart3RxBuffer[USART3_RX_BUFFER_LENGTH];

/******************************************************************************
*  @Function: Uart1Init
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-17 20:36 by Wang Zilin
******************************************************************************/
void Uart1Init(u32 baudRate)
{
    //UART 初始化设置
    UART1_Handler.Instance        = USART1;                                             //USART1
    UART1_Handler.Init.BaudRate   = baudRate;                                           //波特率
    UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;                                 //字长为8位数据格式
    UART1_Handler.Init.StopBits   = UART_STOPBITS_1;                                    //一个停止位
    UART1_Handler.Init.Parity     = UART_PARITY_NONE;                                   //无奇偶校验位
    UART1_Handler.Init.HwFlowCtl  = UART_HWCONTROL_NONE;                                //无硬件流控
    UART1_Handler.Init.Mode       = UART_MODE_TX_RX;                                    //收发模式
    HAL_UART_Init(&UART1_Handler);                                                      //HAL_UART_Init()会使能UART1

    HAL_UART_Receive_IT(&UART1_Handler, (u8 *)Usart1RxBuffer, USART1_RX_BUFFER_LENGTH); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}


/******************************************************************************
*  @Function: Usart2Init
*
*  @Description:串口2初始化
*
*  @Created: by Wang Zilin
*
*  @Modified: 2019-01-31 20:34 by Wang Zilin
******************************************************************************/
void Uart2Init(u32 baudRate)
{
    //UART 初始化设置
    UART2_Handler.Instance        = USART2;              //USART2
    UART2_Handler.Init.BaudRate   = baudRate;            //波特率
    UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;  //字长为8位数据格式
    UART2_Handler.Init.StopBits   = UART_STOPBITS_1;     //一个停止位
    UART2_Handler.Init.Parity     = UART_PARITY_NONE;    //无奇偶校验位
    UART2_Handler.Init.HwFlowCtl  = UART_HWCONTROL_NONE; //无硬件流控
    UART2_Handler.Init.Mode       = UART_MODE_TX_RX;     //收发模式
    HAL_UART_Init(&UART2_Handler);                       //enable UART2

    
}


/******************************************************************************
*  @Function: Usart3Init
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified: 2019-01-31 21:10 by Wang Zilin
******************************************************************************/
void Uart3Init(u32 baudRate)
{
    //UART 初始化设置
    UART3_Handler.Instance        = USART3;              //USART
    UART3_Handler.Init.BaudRate   = baudRate;            //波特率
    UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;  //字长为8位数据格式
    UART3_Handler.Init.StopBits   = UART_STOPBITS_1;     //一个停止位
    UART3_Handler.Init.Parity     = UART_PARITY_NONE;    //无奇偶校验位
    UART3_Handler.Init.HwFlowCtl  = UART_HWCONTROL_NONE; //无硬件流控
    UART3_Handler.Init.Mode       = UART_MODE_TX_RX;     //收发模式
    HAL_UART_Init(&UART3_Handler);                       //enable UART

    HAL_UART_Receive_IT(&UART3_Handler, (u8 *)Usart3RxBuffer, USART3_RX_BUFFER_LENGTH);
}


/******************************************************************************
*  @Function: HAL_UART_MspInit
*
*  @Description:UART底层初始化，时钟使能，引脚配置，中断配置
*           此函数会被HAL_UART_Init()调用
*           uart:串口句柄
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStruct;

    if (huart->Instance == USART1)                            //如果是串口1，进行串口1 MSP初始化
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                         //使能GPIOA时钟
        __HAL_RCC_USART1_CLK_ENABLE();                        //使能USART1时钟

        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10; //PA9
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;          //复用推挽输出
        GPIO_InitStruct.Pull      = GPIO_PULLUP;              //上拉
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;     //高速
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;          //复用为USART1
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);               //初始化PA9

        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);              //抢占优先级0，子优先级0
        HAL_NVIC_EnableIRQ(USART1_IRQn);                      //使能USART1中断通道
    }
    if (huart->Instance == USART2)                            //start USART2 init
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                         //使能GPIOA时钟
        __HAL_RCC_USART2_CLK_ENABLE();                        //使能USART1时钟

        GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;      //复用推挽输出
        GPIO_InitStruct.Pull      = GPIO_PULLUP;          //上拉
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH; //高速
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;      //复用为USART1
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);           //初始化PA9

        HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    if (huart->Instance == USART3)// start USART3 init
    {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}
/******************************************************************************
@Function: UartSendString

@Description:发送字符串

@Created: by Wang Zilin

@Modified: 2019-02-28 19:37 by Wang Zilin
******************************************************************************/
void UartSendString(UART_HandleTypeDef *huart, const char* str, uint32_t Timeout)
{
    //calcualte string length
    HAL_UART_Transmit(huart,(uint8_t*)str, strlen(str),Timeout);   
}
