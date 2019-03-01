/******************************************************************************
*  Include headers
******************************************************************************/
#include "app.h"

/******************************************************************************
*  Task variable definition
******************************************************************************/
OS_TCB  StartTaskTCB;                              //任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];            //任务堆栈

OS_TCB  LED0TaskTCB;                               //任务控制块
__attribute__((aligned(8))) CPU_STK LED0_TASK_STK[LED0_STK_SIZE];              //任务堆栈

OS_TCB  LED1TaskTCB;                               //任务控制块
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];              //任务堆栈

OS_TCB FloatTaskTCB;                               //任务控制块
__attribute__((aligned(8)))  CPU_STK FLOAT_TASK_STK[FLOAT_STK_SIZE]; //任务堆栈

/******************************************************************************
*  @Function: StartTask
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:
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

    OS_CRITICAL_ENTER();    //进入临界区
    //创建LED0任务
    OSTaskCreate((OS_TCB * )&LED0TaskTCB,
                 (CPU_CHAR * )"led0 task",
                 (OS_TASK_PTR )LED0Task,
                 (void * )0,
                 (OS_PRIO     )LED0_TASK_PRIO,
                 (CPU_STK * )&LED0_TASK_STK[0],
                 (CPU_STK_SIZE)LED0_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED0_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);

    //创建LED1任务
    OSTaskCreate((OS_TCB * )&LED1TaskTCB,
                 (CPU_CHAR * )"led1 task",
                 (OS_TASK_PTR )LED1Task,
                 (void * )0,
                 (OS_PRIO     )LED1_TASK_PRIO,
                 (CPU_STK * )&LED1_TASK_STK[0],
                 (CPU_STK_SIZE)LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED1_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);

    //创建浮点测试任务
    OSTaskCreate((OS_TCB * )&FloatTaskTCB,
                 (CPU_CHAR * )"float test task",
                 (OS_TASK_PTR )FloatTask,
                 (void * )0,
                 (OS_PRIO     )FLOAT_TASK_PRIO,
                 (CPU_STK * )&FLOAT_TASK_STK[0],
                 (CPU_STK_SIZE)FLOAT_STK_SIZE / 10,
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,
                 (OS_ERR * )&err);
    OS_CRITICAL_EXIT();                            //进入临界区
    OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err); //挂起开始任务
}


/******************************************************************************
*  @Function: LED0Task
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
void LED0Task(void *p_arg)
{
    OS_ERR err;
    u8 Dht11Exist = 0;
    p_arg = p_arg;
    OSTimeDlyHMSM(0, 0, 0, 1000, OS_OPT_TIME_HMSM_STRICT, &err); //延时500ms
    if (DHT11_Init() == 0)
        Dht11Exist = 1;
    while (1)
    {
        OSTimeDlyHMSM(0, 0, 0, 600, OS_OPT_TIME_HMSM_STRICT, &err); //延时500ms
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_10);
        if (Dht11Exist)
        {
        float data[2];
        DHT11_Read_Data(&data[0],&data[1]);		//读取温湿度值	
        char str[40];
        printf("temp %f,humi %f\r\n", data[0], data[1]);
//        UartSendString(&UART2_Handler, str, 1000);
        }            
    }
}


/******************************************************************************
*  @Function: LED1Task
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
void LED1Task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while (1)
    {
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err); //延时500ms
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
//        printf("test");
    }
}


/******************************************************************************
*  @Function: FloatTask
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
void FloatTask(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    static double double_num = 0.00;
    while (1)
    {
        double_num += 0.01f;
        OSTimeDlyHMSM(0, 0, 0, 400, OS_OPT_TIME_HMSM_STRICT, &err); //延时500ms
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_7);
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_8);
    }
}


/******************************************************************************
*  @Function: HAL_UART_RxCpltCallback
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    printf("%x%x%x\r\n", Usart2RxBuffer[0], Usart2RxBuffer[1], Usart2RxBuffer[2]);
//    u8 data[] = {0x01,0x02,0x03};
//    HAL_UART_Transmit(&UART2_Handler,(uint8_t*)data, 3,1000);
    HAL_UART_Receive_IT(&UART2_Handler, (u8 *)Usart2RxBuffer, USART2_RX_BUFFER_LENGTH);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}
