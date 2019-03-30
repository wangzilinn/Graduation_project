#ifndef __DHT11__
#define __DHT11__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "delay.h"//delay support
/******************************************************************************
//IO方向设置
******************************************************************************/
#define DHT11_IO_IN()  {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=0<<(12*2);}	//PB12输入模式
#define DHT11_IO_OUT() {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=1<<(12*2);} 	//PB12输出模式
/******************************************************************************
////IO操作函数	
******************************************************************************/									   
#define	DHT11_DQ_OUT(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)) //数据端口	PB12
#define	DHT11_DQ_IN     HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) //数据端口	PB12
/******************************************************************************
Function declarations
******************************************************************************/
u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(float *temp,float *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11  
#endif
