#ifndef __APPLICATIONLAYER__
#define __APPLICATIONLAYER__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "transportLayer.h"
typedef struct
{
    float Temperature;
    float Humidity;
    u16 nodeId;
    u8 ontrolWord;
}NodeDataStruct;
/******************************************************************************
Function declaration
******************************************************************************/
void SendNodeData(NodeDataStruct* nodeData);
#endif