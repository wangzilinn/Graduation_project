#ifndef _APP_H
#define _APP_H
#ifdef __cplusplus
extern "C" { 
#endif
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"  
#include "includes.h"   //OS support
#include "hardware_init.h"
#include "graph.h"
/******************************************************************************
Define task parameters
******************************************************************************/
#define START_TASK_PRIO		3
#define START_STK_SIZE 		256//任务堆栈大小	

#define RECEIVE_DATA_TASK_PRIO 4
#define RECEIVE_DATA_STK_SIZE 256
    
#define UPLOAD_DATA_TASK_PRIO 5
#define UPLOAD_DATA_STK_SIZE 256 //任务堆栈大小

#define DISPLAY_TASK_PRIO  6
#define DISPLAY_STK_SIZE       512 //任务堆栈大小	

/******************************************************************************
Task variable declaration
******************************************************************************/
extern OS_TCB StartTaskTCB;
extern CPU_STK START_TASK_STK[];//任务堆栈	

extern OS_TCB ReceiveDataTaskTCB;
extern CPU_STK RECEIVE_DATA_TASK_STK[];

extern OS_TCB	UploadDataTaskTCB;//任务控制块
extern CPU_STK	UPLOAD_DATA_TASK_STK[];//任务堆栈

extern OS_TCB DisplayTaskTCB;//任务控制块
extern CPU_STK DISPLAY_TASK_STK[];//任务堆栈	

typedef struct nodeData
{
    u16 localShortAddress;
    float temperature;
    float humidity;
    u8 controlWord;
}NodeDataStruct;
#define NODE_DATA_BUFFER_LENGTH 10
extern NodeDataStruct nodeDataBuffer[];
extern u8 nodeDataBufferPointer;
/******************************************************************************
Task function declaration
******************************************************************************/
extern void StartTask(void *p_arg);
extern void ReceiveDataTask(void *p_arg);
extern void UploadDataTask(void *p_arg);
extern void DisplayTask(void *p_arg);
/******************************************************************************
Interrupt service function
******************************************************************************/
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif
#endif
