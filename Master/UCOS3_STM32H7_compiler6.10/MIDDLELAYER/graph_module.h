#ifndef __GRAPH_MODULE__
#define __GRAPH_MODULE__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "lcd.h"
#include "monitoring_node_data_storage_module.h"//用于传入节点参数到显示
/******************************************************************************
Function declarations
******************************************************************************/
static u16 ConvertColor(u32 color);
void UIDrawLine(u16 offset, u16 width, u32 color);
void UIDrawBackground();
void UIDrawNodeData(LocalDataSetStruct* localDataSet, int page);
void DisplayDebugInformation(char* debugStr);
#endif 
