#ifndef __APPLICATIONLAYER__
#define __APPLICATIONLAYER__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "method.h"
#include "transportLayer.h"
/******************************************************************************
Struct definition
******************************************************************************/
typedef struct
{
    float Temperature;
    float Humidity;
    u16 nodeId;
    u8 controlWord;
}NodeDataStruct;
/******************************************************************************
Function definitions
******************************************************************************/
NodeDataStruct ReceiveNodeData();
#endif