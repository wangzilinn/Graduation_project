/******************************************************************************
Include header file
******************************************************************************/
#include "photoelectric_switch.h"

/******************************************************************************
@Function: BSPInit

@Description:
    The photoelectric switch pin is PB10

@Created: by Wang Zilin

@Modified: 2018-11-11 17:19 by Wang Zilin
******************************************************************************/
void PhotoElectricSwitch::BSPInit()
{
    //Clock configuration
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    //General pin configuration
    GPIO_InitTypeDef GPIO_initStructure;
    GPIO_initStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_initStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_initStructure.GPIO_Pin = GPIO_Pin_10;//This pin can tolerate 5v input
    GPIO_Init(GPIOB, &GPIO_initStructure);

    //Configuring interrupt lines
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);//Define a pin as an external interrupt
    
    //General pin configuration
    EXTI_InitTypeDef EXTI_initStructure; 
    EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//Rising edge and down edge interrupt
    EXTI_initStructure.EXTI_LineCmd = ENABLE;
    //Specified pins
    EXTI_initStructure.EXTI_Line = EXTI_Line10;
    EXTI_Init(&EXTI_initStructure);
    //NVIC
    NVIC_InitTypeDef NVIC_initStructure;
    NVIC_initStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_initStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_initStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_initStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_initStructure);
}

/******************************************************************************
@Function: IsDetected(bool status)

@Description:
    Input status
@Created: by Wang Zilin

@Modified: 2018-11-11 17:20 by Wang Zilin
******************************************************************************/
void PhotoElectricSwitch::IsDetected(bool status)
{
    if (status == true)
    {
        __detected = true;
    }
    else
    {
        __detected = false;
    }
}
/******************************************************************************
@Function: bool IsDetected

@Description:
    Output status
@Created: by Wang Zilin

@Modified: 2018-11-11 17:20 by Wang Zilin
******************************************************************************/
bool PhotoElectricSwitch::IsDetected()
{
    return __detected;
}