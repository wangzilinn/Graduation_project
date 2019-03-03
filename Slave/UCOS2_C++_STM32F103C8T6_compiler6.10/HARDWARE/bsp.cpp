/******************************************************************************
Include header files
******************************************************************************/
#include "bsp.h"
/******************************************************************************
@Function: BSPInit

@Description:Initialize hardware

@Created: by Wang Zilin

@Modified: 2018-11-04 11:53 by Wang Zilin
******************************************************************************/
void BSPInit()
{
    SystemInit();	 
    delay_init();//init delay and systick
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	//Set NVIC interrupt packet 3: 3 bit preemption priority, 1 bit response priority
    LED::BSPInit(GPIOC, GPIO_Pin_13);
    USART::BSPInit(USART1);
    USART::BSPInit(USART2);   
}
