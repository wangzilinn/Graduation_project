#ifndef __COMMUNICATION_PROTOCOL_STACK_MODULE__
#define __COMMUNICATION_PROTOCOL_STACK_MODULE__
/******************************************************************************
Include headers
******************************************************************************/
#ifdef __cplusplus
extern "C" { 
#endif
#include "public.h"
#include "usart.h"
#include "method.h"
#include "includes.h"   //OS support 必须有extern "C" { 才行
#include "app.h"//获得消息队列
#define LOCAL_SHORT_ADDRESS 0x0001
enum receiveStatusEnum
{
    RECEIVE_FIXED_PART = 0,
    RECEIVE_VARIABLE_PART,
};
/******************************************************************************
Function declaraction 
******************************************************************************/
extern void ZigBeeReceiveCallBack();
extern enum receiveStatusEnum receiveStatus;
#ifdef __cplusplus
}
#endif
#endif
