/******************************************************************************
Include header files
******************************************************************************/

#include "fan.h"

/******************************************************************************
@Function: BSPInit

@Description:PB8, PB9

@Created: by Wang Zilin

@Modified: 2018-11-11 17:29 by Wang Zilin
******************************************************************************/
void Fan::BSPInit()
{   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;    			 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);	
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
   
}

/******************************************************************************
@Function: Fan

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:29 by Wang Zilin
******************************************************************************/
Fan::Fan(GPIO_TypeDef* GPIOx1, uint16_t GPIOPin1,GPIO_TypeDef* GPIOx2, uint16_t GPIOPin2)
{
    __fanStatus = OFF;
    __GPIOx1 = GPIOx1;
    __GPIOPin1 = GPIOPin1;
    __GPIOx2 = GPIOx2;
    __GPIOPin2 = GPIOPin2;
}
/******************************************************************************
@Function: SetStatus

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:30 by Wang Zilin
******************************************************************************/
void Fan::SetStatus(Status funStatus)
{
    __fanStatus = funStatus;
    if (__fanStatus == ON)
    {
        GPIO_SetBits(__GPIOx1, __GPIOPin1);
        GPIO_ResetBits(__GPIOx2, __GPIOPin2);
    }
    else
    {
        GPIO_ResetBits(__GPIOx1, __GPIOPin1);
        GPIO_ResetBits(__GPIOx2, __GPIOPin2);
    }  
}
/******************************************************************************
@Function: GetStatus

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:30 by Wang Zilin
******************************************************************************/
Fan::Status Fan::GetStatus()
{
    return __fanStatus;
}
/******************************************************************************
@Function: IsRunning

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:30 by Wang Zilin
******************************************************************************/
bool Fan::IsRunning()
{
    if (__fanStatus == ON)
        return true;
    else
        return false;
}