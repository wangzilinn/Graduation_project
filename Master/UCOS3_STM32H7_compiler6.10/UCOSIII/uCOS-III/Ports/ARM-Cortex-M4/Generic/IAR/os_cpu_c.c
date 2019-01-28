/*
*********************************************************************************************************
*                                                uC/OS-III
*                                          The Real-Time Kernel
*
*
*                         (c) Copyright 2009-2015; Micrium, Inc.; Weston, FL
*                    All rights reserved.  Protected by international copyright laws.
*
*                                           ARM Cortex-M4 Port
*
* File      : OS_CPU_C.C
* Version   : V3.04.05
* By        : JJL
*             BAN
*             JBL
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or 
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your 
*           application/product.   We provide ALL the source code for your convenience and to help you 
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use 
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can find our product's user manual, API reference, release notes and
*           more information at https://doc.micrium.com.
*           You can contact us at www.micrium.com.
*
* For       : ARMv7M Cortex-M4
* Mode      : Thumb-2 ISA
* Toolchain : IAR EWARM
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_cpu_c__c = "$Id: $";
#endif


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../../../Source/os.h"


#ifdef __cplusplus
extern  "C" {
#endif


/*
*********************************************************************************************************
*                                           IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSIdleTaskHook (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppIdleTaskHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppIdleTaskHookPtr)();
    }
#endif
}


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSInitHook (void)
{
                                                                /* 8-byte align the ISR stack.                            */    
    OS_CPU_ExceptStkBase = (CPU_STK *)(OSCfg_ISRStkBasePtr + OSCfg_ISRStkSize);
    OS_CPU_ExceptStkBase = (CPU_STK *)((CPU_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);
}


/*
*********************************************************************************************************
*                                         STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-III's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSStatTaskHook (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppStatTaskHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppStatTaskHookPtr)();
    }
#endif
}


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : p_tcb        Pointer to the task control block of the task being created.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskCreateHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskCreateHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskCreateHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                                /* Prevent compiler warning                               */
#endif
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : p_tcb        Pointer to the task control block of the task being deleted.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskDelHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskDelHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskDelHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                                /* Prevent compiler warning                               */
#endif
}


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : p_tcb        Pointer to the task control block of the task that is returning.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskReturnHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskReturnHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskReturnHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                                /* Prevent compiler warning                               */
#endif
}


/*
**********************************************************************************************************
*                                       INITIALIZE A TASK'S STACK
*
* Description: This function is called by OS_Task_Create() or OSTaskCreateExt() to initialize the stack
*              frame of the task being created. This function is highly processor specific.
*
* Arguments  : p_task       Pointer to the task entry point address.
*
*              p_arg        Pointer to a user supplied data area that will be passed to the task
*                               when the task first executes.
*
*              p_stk_base   Pointer to the base address of the stack.
*
*              stk_size     Size of the stack, in number of CPU_STK elements.
*
*              opt          Options used to alter the behavior of OS_Task_StkInit().
*                            (see OS.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : (1) Interrupts are enabled when task starts executing.
*
*              (2) All tasks run in Thread mode, using process stack.
*
*              (3) There are two different stack frames depending on whether the Floating-Point(FP)
*                  co-processor is enabled or not.
*
*                  (a) The stack frame shown in the diagram is used when the FP co-processor is not present and
*                      OS_OPT_TASK_SAVE_FP is disabled. In this case, the FP registers and FP Status Control 
*                      register are not saved in the stack frame.
*
*                  (b) If the FP co-processor is present but the OS_OPT_TASK_SAVE_FP is not set, then the stack
*                      frame is saved as shown in diagram (a). Moreover, if OS_OPT_TASK_SAVE_FP is set, then the
*                      FP registers and FP Status Control register are saved in the stack frame.
*
*                      (1) When enabling the FP co-processor, make sure to clear bits ASPEN and LSPEN in the
*                          Floating-Point Context Control Register (FPCCR).
*
*                    +------------+       +------------+
*                    |            |       |            |
*                    +------------+       +------------+
*                    |    xPSR    |       |    xPSR    | 
*                    +------------+       +------------+       
*                    |Return Addr |       |Return Addr |
*                    +------------+       +------------+
*                    |  LR(R14)   |       |   LR(R14)  |
*                    +------------+       +------------+ 
*                    |    R12     |       |     R12    | 
*                    +------------+       +------------+
*                    |    R3      |       |     R3     |
*                    +------------+       +------------+
*                    |    R2      |       |     R0     |
*                    +------------+       +------------+
*                    |    R1      |       |     R1     |
*                    +------------+       +------------+
*                    |    R0      |       |     R0     |
*                    +------------+       +------------+
*                    |    R11     |       |     R11    |
*                    +------------+       +------------+
*                    |    R10     |       |     R10    |
*                    +------------+       +------------+
*                    |    R9      |       |     R9     |
*                    +------------+       +------------+
*                    |    R8      |       |     R8     |
*                    +------------+       +------------+
*                    |    R7      |       |     R7     |
*                    +------------+       +------------+
*                    |    R6      |       |     R6     |
*                    +------------+       +------------+
*                    |    R5      |       |     R5     |
*                    +------------+       +------------+
*                    |    R4      |       |     R4     |
*                    +------------+       +------------+
*                         (a)             |   FPSCR    |
*                                         +------------+
*                                         |     S31    |
*                                         +------------+
*                                                .
*                                                .
*                                                .
*                                         +------------+
*                                         |     S1     |
                                          +------------+
*                                         |     S0     |
*                                         +------------+
*                                              (b)
*
*             (4) The SP must be 8-byte aligned in conforming to the Procedure Call Standard for the ARM architecture 
*
*                    (a) Section 2.1 of the  ABI for the ARM Architecture Advisory Note. SP must be 8-byte aligned 
*                        on entry to AAPCS-Conforming functions states : 
*                    
*                        The Procedure Call Standard for the ARM Architecture [AAPCS] requires primitive 
*                        data types to be naturally aligned according to their sizes (for size = 1, 2, 4, 8 bytes). 
*                        Doing otherwise creates more problems than it solves. 
*
*                        In return for preserving the natural alignment of data, conforming code is permitted 
*                        to rely on that alignment. To support aligning data allocated on the stack, the stack 
*                        pointer (SP) is required to be 8-byte aligned on entry to a conforming function. In 
*                        practice this requirement is met if:
*
*                           (1) At each call site, the current size of the calling function抯 stack frame is a multiple of 8 bytes.
*                               This places an obligation on compilers and assembly language programmers.
*
*                           (2) SP is a multiple of 8 when control first enters a program.
*                               This places an obligation on authors of low level OS, RTOS, and runtime library 
*                               code to align SP at all points at which control first enters 
*                               a body of (AAPCS-conforming) code. 
*              
*                       In turn, this requires the value of SP to be aligned to 0 modulo 8:
*
*                           (3) By exception handlers, before calling AAPCS-conforming code.
*
*                           (4) By OS/RTOS/run-time system code, before giving control to an application.
*
*                 (b) Section 2.3.1 corrective steps from the the SP must be 8-byte aligned on entry 
*                     to AAPCS-conforming functions advisory note also states.
* 
*                     " This requirement extends to operating systems and run-time code for all architecture versions 
*                       prior to ARMV7 and to the A, R and M architecture profiles thereafter. Special considerations 
*                       associated with ARMV7M are discussed in 