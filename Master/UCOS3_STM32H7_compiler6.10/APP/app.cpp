/******************************************************************************
*  Include headers
******************************************************************************/
#include "app.h"

/******************************************************************************
*  Task variable definition
******************************************************************************/
OS_TCB  StartTaskTCB;                              //任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];            //任务堆栈

OS_TCB ReceiveDataTaskTCB;
__attribute__((aligned(8))) CPU_STK RECEIVE_DATA_TASK_STK[RECEIVE_DATA_STK_SIZE];

OS_TCB DisplayTaskTCB;                                                  //任务控制块
__attribute__((aligned(8))) CPU_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE]; //任务堆栈

OS_TCB  UploadDataTaskTCB;                                              //任务控制块
CPU_STK UPLOAD_DATA_TASK_STK[UPLOAD_DATA_STK_SIZE];                     //任务堆栈

/******************************************************************************
*  消息队列缓冲区
******************************************************************************/
ReceivedNodeDataStruct receivedNodeDataBuffer[RECEIVED_NODE_DATA_BUFFER_LENGTH];
u8 receivedNodeDataBufferPointer = 0;

/******************************************************************************
*  Mutex definition
******************************************************************************/
OS_MUTEX loaclDataSetAccessMutex;

/******************************************************************************
*  @Function: StartTask
*
*  @Description:启动任务
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-30 10:50 by Wang Zilin
******************************************************************************/
void StartTask(void *p_arg)
{
    OS_ERR err;

    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);   //统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN      //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
    //使能时间片轮转调度功能,设置默认的时间片长度s
    OSSchedRoundRobinCfg(DEF_ENABLED, 10, &err);
#endif

    //创建互斥量
    OSMutexCreate(&loaclDataSetAccessMutex, (CPU_CHAR * )"loacl data set access mutex", &err);

    //进入临界区
    OS_CRITICAL_ENTER();

    OSTaskCreate((OS_TCB * )&ReceiveDataTaskTCB,
                 (CPU_CHAR * )"receive data task",
                 (OS_TASK_PTR )ReceiveDataTask,
                 (void * )0,
                 (OS_PRIO     )RECEIVE_DATA_TASK_PRIO,
                 (CPU_STK * )&RECEIVE_DATA_TASK_STK[0],
                 (CPU_STK_SIZE)RECEIVE_DATA_STK_SIZE / 10,
                 (CPU_STK_SIZE)RECEIVE_DATA_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);

    //创建LED1任务
    OSTaskCreate((OS_TCB * )&UploadDataTaskTCB,
                 (CPU_CHAR * )"upload data task",
                 (OS_TASK_PTR )UploadDataTask,
                 (void * )0,
                 (OS_PRIO     )UPLOAD_DATA_TASK_PRIO,
                 (CPU_STK * )&UPLOAD_DATA_TASK_STK[0],
                 (CPU_STK_SIZE)UPLOAD_DATA_STK_SIZE / 10,
                 (CPU_STK_SIZE)UPLOAD_DATA_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);

    //显示任务
    OSTaskCreate((OS_TCB * )&DisplayTaskTCB,
                 (CPU_CHAR * )"display task",
                 (OS_TASK_PTR )DisplayTask,
                 (void * )0,
                 (OS_PRIO     )DISPLAY_TASK_PRIO,
                 (CPU_STK * )&DISPLAY_TASK_STK[0],
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE / 10,
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);
    OS_CRITICAL_EXIT();                            //退出临界区
    OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err); //挂起开始任务
}


/******************************************************************************
*  @Function: ReceiveDataTask
*
*  @Description:从ZigBee获取消息的处理任务
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-07 14:52 by Wang Zilin
******************************************************************************/
int restartReceive = 0;
void ReceiveDataTask(void *p_arg)
{
    OS_ERR err;

    p_arg = p_arg;
    HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1); //开始允许接收串口2中断
    OSTimeDlyHMSM(0, 0, 0, 1000, OS_OPT_TIME_HMSM_STRICT, &err);  //延时500ms
    while (1)
    {
        
        OS_MSG_SIZE            msg_size;
        ReceivedNodeDataStruct *receivedNodeData = (ReceivedNodeDataStruct *)OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
        if (err == OS_ERR_NONE)
        {
            restartReceive = 0;
            //printf("t=%f, h=%f, id=%d\r\n",receivedNodeData->temperature, receivedNodeData->humidity, receivedNodeData->localShortAddress);
            char debugStr[10];
            sprintf(debugStr, "ID:%d", receivedNodeData->localShortAddress);
            DisplayDebugInformation(debugStr);
            TogglePilotLED(1);
            OSMutexPend(&loaclDataSetAccessMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
                UpdateLocatDataSet(receivedNodeData);
            OSMutexPost(&loaclDataSetAccessMutex, OS_OPT_POST_NONE, &err);
        }
        OSTimeDlyHMSM(0, 0, 0, 300, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}


/******************************************************************************
*  @Function: void UploadDataTask(void *p_arg)
*
*  @Description:modbus处理任务
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-30 10:51 by Wang Zilin
******************************************************************************/
//OS_MUTEX accessHoldingBufferMutex;
void UploadDataTask(void *p_arg)
{
    
    OS_ERR err;
    //OSMutexCreate(&accessHoldingBufferMutex, (char*)"access Holding Buffer Mutex", &err);
    
    eMBErrorCode eStatus;

    eStatus = eMBInit(MB_RTU, 0x0a, 0, 115200, MB_PAR_NONE);
    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );
    while(1)
    {
        ( void )eMBPoll(  );
        OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
        TogglePilotLED(3);
        restartReceive++;
        if (restartReceive > 200)//10s未接收到，则重启
        {
            restartReceive = 0;
            receiveStatus = RECEIVE_FIXED_PART;
            HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, 1);
        }
    }
    
}


/******************************************************************************
*  @Function: void DisplayTask(void *p_arg)
*
*  @Description:显示任务
*
*  @Created: by Wang Zilin
*
*  @Modified:2019-03-10 20:36 by Wang Zilin
******************************************************************************/

void DisplayTask(void *p_arg)
{
    OS_ERR err;
    UIDrawBackground(PageName);
    while (1)
    { 
        int posX= 0,posY = 0;
        u8 touched = 0;
        if(UIScan(&posX, &posY))
        {
            touched = 1;
        }
        switch (PageName)
        {
            case OVERVIEW_PAGE:
            {
                if(touched)
                {
                    printf("Overview %d,%d\r\n", posX, posY);
                    UIOverviewPageRespond(posX,posY,&localDataSet, 0);
                    break;
                }
                else
                {
                    UIOverviewPageRespond(0, 0, &localDataSet, 0);
                }
            }
            case NODE_NUMBER_CONFIG_PAGE:
            {
                if(touched)
                {
                    printf("Config %d,%d\r\n",posX, posY);
                    UIConfigPageRespond(posX,posY,&localDataSet);
                    break;
                }
            }
        }
        OSTimeDlyHMSM(0, 0, 0, 5, OS_OPT_TIME_HMSM_STRICT, &err);
        TogglePilotLED(2);     
    }
}






