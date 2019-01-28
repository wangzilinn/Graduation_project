/******************************************************************************
Include headers
******************************************************************************/
#include "app.h"
/******************************************************************************
Task variable definition
******************************************************************************/
OS_TCB StartTaskTCB;//任务控制块	
CPU_STK START_TASK_STK[START_STK_SIZE];//任务堆栈

OS_TCB LED0TaskTCB;//任务控制块
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];//任务堆栈	

OS_TCB LED1TaskTCB;//任务控制块
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];//任务堆栈	

OS_TCB	FloatTaskTCB;//任务控制块
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];//任务堆栈
/******************************************************************************
@Function: StartTask

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void StartTask(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,设置默认的时间片长度s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED0任务
	OSTaskCreate((OS_TCB 	* )&LED0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )LED0Task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);				
				 
	//创建LED1任务
	OSTaskCreate((OS_TCB 	* )&LED1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )LED1Task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
				 
	//创建浮点测试任务
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )FloatTask, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//进入临界区				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
}
/******************************************************************************
@Function: LED0Task

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void LED0Task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,600,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_10);
	}
}
/******************************************************************************
@Function: LED1Task

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void LED1Task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	while(1)
	{
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
        printf("test");
	}
}
/******************************************************************************
@Function: FloatTask

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void FloatTask(void *p_arg)
{
	OS_ERR err;
    CPU_SR_ALLOC();
	static double double_num=0.00;
	while(1)
	{
		double_num+=0.01f;
        OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_7);
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_8);
	}
}
/******************************************************************************
@Function: HAL_UART_RxCpltCallback

@Description:

@Created: by Wang Zilin

@Modified: 
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
}
