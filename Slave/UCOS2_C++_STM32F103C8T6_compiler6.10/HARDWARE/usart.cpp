/******************************************************************************
Include header files
******************************************************************************/
#include "usart.h"

/******************************************************************************
@Function:BSPInit

@Description:BSP initialzation, static functions

@Created: by Wang Zilin

@Modified: 2018-11-04 16:27 by Wang Zilin
******************************************************************************/
void USART::BSPInit(USART_TypeDef* USARTx)
{
    if (USARTx == USART1)
    {
        USART1IOInit();
        USART1Init();
    }
    else if (USARTx == USART2)
    {
        USART2IOInit();
        USART2Init();
    }
    else if (USARTx == USART3)
    {
        USART3IOInit();
        USART3Init();
    }        
}
/******************************************************************************
@Function: SendByte

@Description:send one byte

@Created: by Wang Zilin

@Modified: 2018-11-04 16:27 by Wang Zilin
******************************************************************************/
void USART::SendByte(u8 byte) const
{
    while((__USARTx->SR & USART_SR_TC) != USART_SR_TC);		//wait for the last byte to be send
	__USARTx->DR = byte;	
}

/******************************************************************************
@Function: SendArray

@Description:send an array

@Created: by Wang Zilin

@Modified: 2018-11-04 16:28 by Wang Zilin
******************************************************************************/
void USART::SendArray(u8* array, int length) const
{
	while(length-- != 0)
	{
		while((__USARTx->SR & USART_SR_TC) != USART_SR_TC);	//wait for the last byte to be send
		__USARTx->DR = *array++;
	}
}


/******************************************************************************
@Function: SendString

@Description:send a string

@Created: by Wang Zilin

@Modified: 2018-11-04 16:28 by Wang Zilin
******************************************************************************/
void USART::SendString(const char* string)
{
	while(*string != '\0')
	{
		while((__USARTx->SR & USART_SR_TC) != USART_SR_TC);	//wait for the last byte to be send
		__USARTx->DR = *string++;
	}
}

/******************************************************************************
@Function: USART1IOInit

@Description:USART1 pins initialization

@Created: by Wang Zilin

@Modified: 2018-11-04 16:29 by Wang Zilin
******************************************************************************/
void USART1IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/******************************************************************************
@Function: USART1Init

@Description:USART1 registers config

@Created: by Wang Zilin

@Modified: 2018-11-04 16:35 by Wang Zilin
******************************************************************************/
void USART1Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//enable receive interrupt
	
	USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
@Function: USART2IOInit

@Description:USART2 pins initialization

@Created: by Wang Zilin

@Modified: 2018-11-04 16:37 by Wang Zilin
******************************************************************************/
void USART2IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/******************************************************************************
@Function: USART2Init

@Description:USART2 registers config

@Created: by Wang Zilin

@Modified: 2018-11-04 16:39 by Wang Zilin
******************************************************************************/
void USART2Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				
	
	USART_Cmd(USART2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
@Function: USART3IOInit

@Description:USART3 pins initialization

@Created: by Wang Zilin

@Modified: 2018-11-04 16:38 by Wang Zilin
******************************************************************************/
void USART3IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
@Function: USART3Init

@Description:USART3 registers config

@Created: by Wang Zilin

@Modified: 2018-11-04 16:40 by Wang Zilin
******************************************************************************/
void USART3Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				
	
	USART_Cmd(USART3, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

