#ifndef __MB_REGISTER_CALLBACK__
#define __MB_REGISTER_CALLBACK__
/******************************************************************************
Include headers
******************************************************************************/
#include "mb.h"
#include "mbutils.h"//增加xMBUtilSetBits函数声明
/******************************************************************************
线圈,离散量,输入寄存器,保持寄存器相关宏定义
******************************************************************************/
//离散量
#define S_DISCRETE_INPUT_START        0
#define S_DISCRETE_INPUT_NDISCRETES   16
//线圈
#define S_COIL_START                  0
#define S_COIL_NCOILS                 64
//输入寄存器
#define S_REG_INPUT_START             0
#define S_REG_INPUT_NREGS             7
//保持寄存器
#define S_REG_HOLDING_START           0
#define S_REG_HOLDING_NREGS           100
/* salve mode: holding register's all address */
#define          S_HD_RESERVE                     0
#define          S_HD_CPU_USAGE_MAJOR             1
#define          S_HD_CPU_USAGE_MINOR             2
/* salve mode: input register's all address */
#define          S_IN_RESERVE                     0
/* salve mode: coil's all address */
#define          S_CO_RESERVE                     0
/* salve mode: discrete's all address */
#define          S_DI_RESERVE                     0

extern USHORT usSRegInputBuf[]               ;
#endif