#ifndef __TRANSPORT_LAYER__
#define __TRANSPORT_LAYER__
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
    
/******************************************************************************
Function declaraction 
******************************************************************************/
extern void ZigBeeReceiveCallBack();

#ifdef __cplusplus
}
#endif
#endif
