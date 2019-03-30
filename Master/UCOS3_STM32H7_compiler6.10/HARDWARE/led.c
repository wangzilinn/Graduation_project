/******************************************************************************
Include headers
******************************************************************************/
#include "led.h"
/******************************************************************************
@Function: LEDInit

@Description:LED IO初始化

@Created: by Wangzilin

@Modified: 2019-03-30 10:31 by Wang Zilin
******************************************************************************/
void LEDInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();					//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
	
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;         			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    
    //PB1
    GPIO_Initure.Pin=GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     		//初始化GPIOB.1
    //PC13 D1
    GPIO_Initure.Pin=GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    //PD3 D2, PD7 D3
    GPIO_Initure.Pin=GPIO_PIN_3 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
    //PG10 D4
	GPIO_Initure.Pin=GPIO_PIN_10;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    //PH8 D5
	GPIO_Initure.Pin=GPIO_PIN_8;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);   
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1 
}
/******************************************************************************
@Function: TogglePilotLED

@Description:翻转LED

@Created: by Wangzilin

@Modified: 2019-03-30 10:31 by Wang Zilin
******************************************************************************/
void TogglePilotLED(int LEDNumber)
{
    switch (LEDNumber)
    {
        case 1:
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
            break;
        case 2:
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3); 
            break;
        case 3:
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_7); 
            break;
        case 4:
            HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_10);
            break;
        case 5:
            HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_8); 
    }        
}
