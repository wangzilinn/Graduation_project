/******************************************************************************
Include header files
******************************************************************************/
#include "led.h"   
/******************************************************************************
@Function: BSPInit

@Description:LED hardware initialization, static function

@Created: by Wang Zilin

@Modified: 2018-11-04 17:00 by Wang Zilin
******************************************************************************/
void LED::BSPInit(GPIO_TypeDef* GPIOx, uint16_t GPIOPin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //TODO:Need to be generalized here
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIOPin;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOx, &GPIO_InitStructure);					 
    GPIO_SetBits(GPIOx,GPIOPin);						 					 
}
/******************************************************************************
@Function: void LED::__ChangeToStatus()

@Description:Modify LED to the specified state

@Created: by Wang Zilin

@Modified: 2018-11-04 17:01 by Wang Zilin
******************************************************************************/
void LED::__ChangeToStatus()
{
    if(__LEDStatus == ON)
    {
        TurnOn();
    }
    else
    {
        TurnOff();
    }
}


/******************************************************************************
@Function: void LED::TurnOn()

@Description:

@Created: by Wang Zilin

@Modified: 2018-3-14 17:18:48 by zilin Wang
******************************************************************************/
void LED::TurnOn()
{
    GPIO_ResetBits(__GPIOx,__GPIOPin);			
}


/******************************************************************************
@Function: void LED::TurnOff()

@Description:

@Created: by Wang Zilin

@Modified: 2018-3-14 17:19:13 by zilin Wang
******************************************************************************/
void LED::TurnOff()
{
    GPIO_SetBits(__GPIOx,__GPIOPin);			
}


/******************************************************************************
@Function: void LED::Toggle(void)

@Description:

@Created: by Wang Zilin

@Modified: 2018-3-14 17:19:31 by zilin Wang
******************************************************************************/
void LED::Toggle(void)
{
    //GPIOC->ODR ^= GPIO_Pin_13;
    if (__LEDStatus == ON)
    {
        TurnOff();
        __LEDStatus = OFF;
    }
    else
    {
        TurnOn();
        __LEDStatus = ON;
    }
}
