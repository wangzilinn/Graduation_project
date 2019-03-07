/******************************************************************************
Include header files
******************************************************************************/
#include "task.h"
/******************************************************************************
Task stack definition 
******************************************************************************/
OS_STK GUARDIAN_TASK_STK[GUARDIAN_TASK_STK_SIZE];
__attribute__((aligned(8))) OS_STK MAIN_TASK_STK[MAIN_TASK_STK_SIZE];
/******************************************************************************
@Function: GuardianTask

@Description:

@Created: by zilin Wang

@Modified: 2018-11-04 11:50 by Wang Zilin
******************************************************************************/
void GuardianTask(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 	
 	OS_ENTER_CRITICAL();			//can not be interrupted 
    OSTaskCreate(MainTask,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_TASK_STK_SIZE-1],MAIN_TASK_PRIO);				   	
	OS_EXIT_CRITICAL();				//can be interrupted
    OSTaskSuspend(GUARDIAN_TASK_PRIO);	
}
/******************************************************************************
@Function: MainTask

@Description:

@Created: by zilin Wang

@Modified: 2018-11-04 11:52 by Wang Zilin
******************************************************************************/
void MainTask(void *pdata)
{
    LED pilotLED(GPIOC, GPIO_Pin_13, 1, LED::ON);
    USART outputUsart(USART_DEBUG);  
    USART zigBeeUsart(USART_ZIGBEE);
    
    u8 exist = 0;
    if (DHT11_Init() == 0)
    {
        exist = 1;
    }
    OSTimeDlyHMSM(0, 0, 0, 1000);
    u8 data[] = {0x00,0x01,0x02};
    while(1)
    {     
        SendDataPackage(USART_ZIGBEE, 0x01, data, 3);
        pilotLED.Toggle();
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

