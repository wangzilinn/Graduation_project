/******************************************************************************
Include header files
******************************************************************************/
#include "pedal.h"

/******************************************************************************
@Function: BSPInit

@Description:
    The two pins PA3, PA4 act as external interrupts for the two foot pedals

@Created: by Wang Zilin

@Modified: 2018-11-05 17:20 by Wang Zilin
******************************************************************************/
void Pedal::BSPInit()
{
    //Clock configuration
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    //General pin configuration
    GPIO_InitTypeDef GPIO_initStructure;
    GPIO_initStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_initStructure.GPIO_Mode = GPIO_Mode_IPU;//Pull up input
    //Specified pins
    GPIO_initStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_initStructure);
    GPIO_initStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_initStructure);
    //Configuring interrupt lines
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);//Define a pin as an external interrupt
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
    
    //General pin configuration
    EXTI_InitTypeDef EXTI_initStructure; 
    EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//Rising edge and down edge interrupt
    EXTI_initStructure.EXTI_LineCmd = ENABLE;
    //Specified pins
    EXTI_initStructure.EXTI_Line = EXTI_Line3;
    EXTI_Init(&EXTI_initStructure);
    EXTI_initStructure.EXTI_Line = EXTI_Line4;
    EXTI_Init(&EXTI_initStructure);
    //PA3, PA4
    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_initStructure);
    NVIC_initStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_initStructure);
}


/******************************************************************************
@Function: GetLabel

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:17 by Wang Zilin
******************************************************************************/
int Pedal::GetLabel()
{
    return __label;
}

/******************************************************************************
@Function: IsDepressed

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:17 by Wang Zilin
******************************************************************************/
bool Pedal::IsDepressed()
{
    return __pedalStatus;
}

/******************************************************************************
@Function: IsDepressed

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:18 by Wang Zilin
******************************************************************************/
void Pedal::IsDepressed(bool status)
{
    __pedalStatus = status;
}