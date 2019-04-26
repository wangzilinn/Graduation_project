#ifndef __GRAPH_MODULE__
#define __GRAPH_MODULE__
/******************************************************************************
Include headers
******************************************************************************/
#include "public.h"
#include "lcd.h"
#include "monitoring_node_data_storage_module.h"//用于传入节点参数到显示
#include "touch.h"
typedef enum pageNameEnum
{
    OVERVIEW_PAGE,
    NODE_NUMBER_CONFIG_PAGE
}PageNameEnum;
typedef enum clickedAreaEnum
{
    ID_AREA,
    X_AREA,
    Y_AREA,
    Z_AREA,
    BLANK_AREA,
    TO_OVERVIEW_AREA,
    TO_CONFIG_AREA
}ClickedAreaEnum;
/******************************************************************************
Function declarations
******************************************************************************/
extern PageNameEnum PageName;
static u16 ConvertColor(u32 color);
static void UIDrawNumericKeypadBackGround(int x, int y);
static u8 UIGetNumericKeypad(int x, int y, int* number);
static void UIDrawNumeric(int * number);
static void UIDrawLine(u16 offset, u16 width, u32 color);
void UIDrawBackground(PageNameEnum PageName);
void UIOverviewPageRespond(int x, int y, LocalDataSetStruct* localDataSet, int page);
void UIConfigPageRespond(int x, int y, LocalDataSetStruct *localDataSet);
void DisplayDebugInformation(char* debugStr);
u8 UITestPoint(int lastX, int lastY, int curX, int curY);
u8 UIScan(int* x, int* y);
#endif 
