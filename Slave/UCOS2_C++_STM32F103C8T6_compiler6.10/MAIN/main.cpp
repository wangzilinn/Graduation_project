/******************************************************************************
包含头文件
******************************************************************************/
#include "main.h"

struct __FILE { int handle; /* Add whatever you need here */ };
//FILE __stdout;
//FILE __stdin;
int std::fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

/******************************************************************************
@Function: main

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:31 by Wang Zilin
******************************************************************************/
int main(void)
{
    BSPInit();
    OSInit();   
    OSTaskCreate(GuardianTask,(void *)0,(OS_STK *)&GUARDIAN_TASK_STK[GUARDIAN_TASK_STK_SIZE-1],GUARDIAN_TASK_PRIO );//创建守护任务
    OSStart();
    while(1)
    {
        //gg
    }
}

  







