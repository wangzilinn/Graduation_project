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
Struct definition
******************************************************************************/
NodeDataStruct NodeData;
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
    DHT11_Init();
    OSTimeDlyHMSM(0, 0, 0, 1000);
    OSTimeDlyHMSM(0, 0, 0, LOCAL_SHORT_ADDRESS*1000);//防止同时上电链路冲突
    NodeData.localShortAddress = LOCAL_SHORT_ADDRESS;
    while(1)
    {           
        if (DHT11_Read_Data(&NodeData.temperature, &NodeData.humidity) == 0)
        {
            SendDataPackage(USART_ZIGBEE, 0x0001, (u8*)&NodeData, sizeof(NodeData));
            pilotLED.Toggle(); 
            OSTimeDlyHMSM(0, 0, 0, 5000);            
        }                
        OSTimeDlyHMSM(0, 0, 0, 20);       //
    }
}

