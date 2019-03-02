/******************************************************************************
Include headers
******************************************************************************/
#include "delay.h"
/******************************************************************************
if support OS
******************************************************************************/
#ifndef SYSTEM_SUPPORT_OS
#define SYSTEM_SUPPORT_OS 1
#endif

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos support  
#endif
/******************************************************************************
Variable definitions
******************************************************************************/
static u32 fac_us=400;							//us延时倍乘数,这个数是系统时钟频率,也是SystickInit()函数的参数
#if SYSTEM_SUPPORT_OS 	
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
#endif
#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).
/******************************************************************************
//当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
//首先是3个宏定义:
//delay_osrunning:用于表示OS当前是否正在运行,以决定是否可以使用相关函数
//delay_ostickspersec:用于表示OS设定的时钟节拍,delay_init将根据这个参数来初始哈systick
//delay_osintnesting:用于表示OS中断嵌套级别,因为中断里面不可以调度,delay_ms使用该参数来决定如何运行
//然后是3个函数:
//delay_osschedlock:用于锁定OS任务调度,禁止调度
//delay_osschedunlock:用于解锁OS任务调度,重新开启调度
//delay_ostimedly:用于OS延时,可以引起任务调度.

//本例程仅作UCOSII和UCOSIII的支持,其他OS,请自行参考着移植
******************************************************************************/
/******************************************************************************
支持UCOSII
******************************************************************************/
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
#endif
/******************************************************************************
支持UCOSIII
******************************************************************************/
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
#endif
/******************************************************************************
@Function: void delay_osschedlock(void)

@Description:us级延时时,关闭任务调度(防止打断us级延迟)

@Created: by ALIENTEK

@Modified: 2019-03-01 11:52 by Wang Zilin
******************************************************************************/
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//使用UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII的方式,禁止调度，防止打断us延时
#else										//否则UCOSII
	OSSchedLock();							//UCOSII的方式,禁止调度，防止打断us延时
#endif
}
/******************************************************************************
@Function: void delay_osschedunlock(void)

@Description:us级延时时,恢复任务调度

@Created: by ALIENTEK

@Modified: 2019-03-01 11:52 by Wang Zilin
******************************************************************************/
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//使用UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII的方式,恢复调度
#else										//否则UCOSII
	OSSchedUnlock();						//UCOSII的方式,恢复调度
#endif
}
/******************************************************************************
@Function: void delay_ostimedly(u32 ticks)

@Description:调用OS自带的延时函数延时
//ticks:延时的节拍数

@Created: by ALIENTEK

@Modified: 2019-03-01 11:53 by Wang Zilin
******************************************************************************/
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);//UCOSIII延时采用周期模式
#else
	OSTimeDly(ticks);						//UCOSII延时
#endif 
}
#endif	
/******************************************************************************
OS下的毫秒及微秒延时函数
******************************************************************************/
#if SYSTEM_SUPPORT_OS 						//如果需要支持OS.
/******************************************************************************
@Function: void delay_us(u32 nus)

@Description:延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	

@Created: by ALIENTEK

@Modified: 2019-03-01 11:54 by Wang Zilin
******************************************************************************/							   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	delay_osschedlock();					//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
	delay_osschedunlock();					//恢复OS调度											    
}  
/******************************************************************************
@Function: void delay_ms(u16 nms)

@Description:延时nms
//nms:要延时的ms数
//nms:0~65535

@Created: by ALIENTEK

@Modified: 2019-03-01 11:54 by Wang Zilin
******************************************************************************/
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));				//普通方式延时
}
/******************************************************************************
非OS下的毫秒及微秒延时函数
******************************************************************************/
#else 
/******************************************************************************
@Function: void delay_us(u32 nus)

@Description:延时nus
//nus为要延时的us数.	
//注意:nus的值不要大于1000us

@Created: by ALIENTEK

@Modified: 2019-03-01 11:56 by Wang Zilin
******************************************************************************/
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}
/******************************************************************************
@Function: void delay_ms(u16 nms)

@Description:延时nms
//nms:要延时的ms数

@Created: by ALIENTEK

@Modified: 2019-03-01 11:57 by Wang Zilin
******************************************************************************/
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}

#endif
			 


































