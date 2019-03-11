#ifndef __GRAPH__
#define __GRAPH__
#include "public.h"
#include "lcd.h"
#include "deviceParameters.h"//用于传入节点参数到显示
static u16 ConvertColor(u32 color);
void UIDrawLine(u16 offset, u16 width, u32 color);
void UIDrawBackground();
void UIDrawNodeData(NodeDataStruct* nodedataArray,int* refreshCnt);
#endif 
