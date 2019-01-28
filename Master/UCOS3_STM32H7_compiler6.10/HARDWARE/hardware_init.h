#ifndef _HARDWARE_INIT_H
#define _HARDWARE_INIT_H
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "includes.h"
/******************************************************************************
Peripheral header file
******************************************************************************/
#include "usart.h" 
#include "led.h"
#include "ITM.h"
/******************************************************************************
Private function declarations
******************************************************************************/
static void Cache_Enable(void);                                    //使能STM32H7的L1-Cahce
static void SystemClockInit(); //配置系统时钟
/******************************************************************************
Function declaration
******************************************************************************/
void __attribute__((noinline)) WFI_SET(void);
void __attribute__((noinline)) INTX_DISABLE(void);
void __attribute__((noinline)) INTX_ENABLE(void);
void __attribute__((noinline)) MSR_MSP(u32 addr);
u8 Get_ICahceSta(void);//判断I_Cache是否打开
u8 Get_DCahceSta(void);//判断I_Dache是否打开
void HardwareInit(void);
#endif