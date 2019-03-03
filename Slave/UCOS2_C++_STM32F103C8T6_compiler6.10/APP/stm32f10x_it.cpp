/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */



#ifdef __cplusplus                                                                                 
extern "C"        {
#endif	
/******************************************************************************
Include header files
******************************************************************************/
#include "stm32f10x_it.h" 
/******************************************************************************
@Function: IRQ Functions

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:38 by Wang Zilin
******************************************************************************/
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}
/******************************************************************************
@Function: SysTick_Handler

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:25 by Wang Zilin
******************************************************************************/
void SysTick_Handler(void)
{				   
	OS_CPU_SR  cpu_sr = 0;  
    OS_ENTER_CRITICAL();
    OSIntEnter();//This line must be surrounded by a critical section
    OS_EXIT_CRITICAL();   	
    
    OSTimeTick();  
    
    OSIntExit();        
}
/******************************************************************************
@Function: USART1_IRQHandler

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:02 by Wang Zilin
******************************************************************************/
void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr = 0;  
    OS_ENTER_CRITICAL();
    OSIntEnter();
    OS_EXIT_CRITICAL();   
    
	if((USART_GetITStatus(USART1, USART_IT_RXNE) == SET))
	{
		auto temp = USART_ReceiveData(USART1); 
        USART test(USART1);
        test.SendByte(temp);       
	} 
    OSIntExit();
}
/******************************************************************************
@Function: USART2_IRQHandler

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:30 by Wang Zilin
******************************************************************************/
void USART2_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr = 0;
    OS_ENTER_CRITICAL();
    OSIntEnter();
    OS_EXIT_CRITICAL();
    
	if((USART_GetITStatus(USART2, USART_IT_RXNE) == SET))
	{
		u8 temp = USART_ReceiveData(USART2);
	} 

    OSIntExit();
}
/******************************************************************************
@Function: EXTI3_IRQHandler

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:30 by Wang Zilin
******************************************************************************/
void EXTI3_IRQHandler()
{
    
}
/******************************************************************************
@Function: EXTI4_IRQHandler

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 20:31 by Wang Zilin
******************************************************************************/
void EXTI4_IRQHandler()
{
   
}
/******************************************************************************
@Function: EXTI15_10_IRQHandler

@Description:Photoelectric switch 

@Created: by Wang Zilin

@Modified: 2018-11-11 20:31 by Wang Zilin
******************************************************************************/
void EXTI15_10_IRQHandler()
{
    
}
#ifdef __cplusplus                                 
}

#endif

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
