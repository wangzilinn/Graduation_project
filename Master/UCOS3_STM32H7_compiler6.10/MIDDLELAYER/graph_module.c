/******************************************************************************
*  Include headers
******************************************************************************/
#include "graph_module.h"
/******************************************************************************
*  Global Variables definitions
******************************************************************************/
static u16 __colorBuffer[3][360] = { 0 };
PageNameEnum PageName = OVERVIEW_PAGE;
ClickedAreaEnum ClickedArea = BLANK_AREA;
/******************************************************************************
*  @Function: UIDrawBackground
*
*  @Description:画LCD的背景
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:48 by Wang Zilin
******************************************************************************/
void UIDrawBackground(PageNameEnum PageName)
{
    BACK_COLOR  = ConvertColor(0x141B23);
    POINT_COLOR = ConvertColor(0xFFFFFF);
    switch (PageName)
    {
        case OVERVIEW_PAGE:
            UIDrawLine(0, 30, 0xB9042E);
            UIDrawLine(30, 60, 0xB70029);
            UIDrawLine(90, 390, 0x141B23);
            POINT_COLOR = 0xFFFFFFFF;
            LCD_ShowString(40, 10, 120, 32, 32, (u8 *)"Overview", 1);
            //画表头
            POINT_COLOR = ConvertColor(0xB9042E);
            LCD_ShowString(90, 50, 390, 24, 24, (u8 *)" ID|    COORD    |TEMP|HUMI|STAT", 1);
            LCD_ShowString(274, 224, 142, 24, 24, (u8*)"to config", 1);
        break;
        case NODE_NUMBER_CONFIG_PAGE:
            UIDrawLine(0, 30, 0xB9042E);
            UIDrawLine(30, 210, 0xB70029);
            UIDrawLine(210, 270, 0x141B23);
            //标题
            LCD_ShowString(40, 10, 120, 32, 32, (u8 *)"Config", 1);
            //左边label
            LCD_ShowString(60, 70, 40, 24, 24, (u8*)"ID:",1);
            LCD_ShowString(60, 120, 127, 24, 24, (u8*)"Coordinate", 1);
            LCD_ShowString(60, 164, 30, 24, 24, (u8*)"X:", 1);
            LCD_ShowString(60, 194, 30, 24, 24, (u8*)"Y:", 1);
            LCD_ShowString(60, 224, 30, 24, 24, (u8*)"Z:", 1);
            //画数字键：
            UIDrawNumericKeypadBackGround(265,35);
            LCD_ShowString(274, 224, 142, 24, 24, (u8*)"to overview", 1);
         break;
    }
}
/******************************************************************************
*  @Function: UIDrawLine
*
*  @Description:画一条有宽度的竖线
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:48 by Wang Zilin
******************************************************************************/
static void UIDrawLine(u16 offset, u16 width, u32 color)
{
    u16 _color = ConvertColor(color);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 360; j++)
        {
            __colorBuffer[i][j] = _color;
        }
    }
    int blockNumber = width / 3;
    for (int i = 0; i < blockNumber; i++)
    {
        LCD_Color_Fill(offset + 3 * i, 0, offset + 3 * i + 3, 272, *__colorBuffer);
    }
}


/******************************************************************************
*  @Function: UIDrawNodeData
*
*  @Description:显示一个采集点的数据
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:48 by Wang Zilin
******************************************************************************/
void UIOverviewPageRespond(int x, int y, LocalDataSetStruct *localDataSet, int page)
{  
    for (int i = page; i < page + 5; i++)
    {
        int displayNodeNumber = localDataSet->activeNodeArray[i];
        if (displayNodeNumber != 0)//因为并没有0号节点，如果为0，则是activeNodeArray为空
        {
            char displayStr[40] = { 0 };
            char deviceStatusStr[4];
            sprintf(displayStr, "%3d|(%3d,%3d,%3d)|%3.1f|%3.1f|%4s",
                    localDataSet->nodeData[displayNodeNumber].receivedNodeData.localShortAddress,
                    localDataSet->nodeData[displayNodeNumber].coordinate.x,
                    localDataSet->nodeData[displayNodeNumber].coordinate.y,
                    localDataSet->nodeData[displayNodeNumber].coordinate.z,
                    localDataSet->nodeData[displayNodeNumber].receivedNodeData.temperature,
                    localDataSet->nodeData[displayNodeNumber].receivedNodeData.humidity,
                    deviceStatusEnumToStr(localDataSet->nodeData[displayNodeNumber].deviceStatus, deviceStatusStr)
                    );

            LCD_ShowString(90, 74 + i * 24, 390, 24, 24, (u8 *)displayStr, 0);
        }
        else
        {
            LCD_ShowString(90, 74 + i * 24, 390, 24, 24, (u8 *)"                                 ", 0);//如果没有数,则填上空白
        }
    }
    if (x > 274 && y > 224 && x < 408 && y < 247)
    {
        PageName = NODE_NUMBER_CONFIG_PAGE; 
        UIDrawBackground(PageName);
        ClickedArea = BLANK_AREA;
    }
}
/******************************************************************************
@Function: UIConfigPageRespond

@Description:

@Created: by Wangzilin

@Modified: 2019-04-25 19:46 by Wang Zilin
******************************************************************************/
void UIConfigPageRespond(int x, int y, LocalDataSetStruct *localDataSet)
{
    static int ID = 0;
    static int coordX = 0;
    static int coordY = 0;
    static int coordZ = 0;
    static int *NumericKeypadValue = NULL;
    //在哪个区域
    if (x > 60 && y > 70 && x < 150 && y < 90)
    {
        ClickedArea = ID_AREA;
        NumericKeypadValue = &ID;
    }
    else if (x > 60 && y > 164 && x < 124 && y < 183)
    {
        ClickedArea = X_AREA;
        NumericKeypadValue = &coordX;
    }
    else if (x > 60 && y > 194 && x < 123 && y < 216)
    {
        ClickedArea = Y_AREA;
        NumericKeypadValue = &coordY;
    }
    else if (x > 60 && y > 224 && x < 121 && y < 245)
    {
        ClickedArea = Z_AREA; 
        NumericKeypadValue = &coordZ;
    }
    else if (x > 274 && y > 224 && x < 408 && y < 247)
        ClickedArea = TO_OVERVIEW_AREA; 
    //如果点击了要输入数字的区域，则调用数字键盘
    switch (ClickedArea)
    {
        case ID_AREA:
        case X_AREA:
        case Y_AREA:
        case Z_AREA:
        {
            if(UIGetNumericKeypad(x, y, NumericKeypadValue))
            {
                //如果返回真值，说明点击了确认键,取消输入框的选定状态，并保存数据
                ClickedArea = BLANK_AREA; 
                localDataSet->nodeData[ID].coordinate.x = coordX;
                localDataSet->nodeData[ID].coordinate.y = coordY;
                localDataSet->nodeData[ID].coordinate.z = coordZ;    
                DisplayDebugInformation("saved");                
            }
            //将数字显示出来
            UIDrawNumeric(NumericKeypadValue);
            break;
        }
        case TO_OVERVIEW_AREA:
        {
            PageName = OVERVIEW_PAGE;
            UIDrawBackground(PageName);
            ClickedArea = BLANK_AREA;//操作完成后，失能激活区域
            break;
        }
        case BLANK_AREA:
        case TO_CONFIG_AREA:
            break;
    }
    
}
/******************************************************************************
@Function: UIDrawNumeric

@Description:
根据传入参数在特定位置显示数据
@Created: by Wangzilin

@Modified: 2019-04-25 21:25 by Wang Zilin
******************************************************************************/
static void UIDrawNumeric(int * number)
{
    if(number == NULL)
        return;
    char a[10];
    sprintf(a,"%d", *number);
    switch (ClickedArea)
    {
        case ID_AREA:
        {
            LCD_ShowString(100, 70, 60, 24, 24, (u8*)"    ", 0);
            LCD_ShowString(100, 70, 60, 24, 24, (u8*)a, 0);
            break;
        }
        case X_AREA:
        {
            LCD_ShowString(95, 163, 60, 24, 24, (u8*)"    ", 0);
            LCD_ShowString(95, 163, 60, 24, 24, (u8*)a, 0);
            break;
        }
        case Y_AREA:
        {
            LCD_ShowString(95, 192, 60, 24, 24, (u8*)"    ", 0);
            LCD_ShowString(95, 192, 60, 24, 24, (u8*)a, 0);
            break;            
        }
        case Z_AREA:
        {
            LCD_ShowString(95, 222, 60, 24, 24, (u8*)"    ", 0);
            LCD_ShowString(95, 222, 60, 24, 24, (u8*)a, 0);
            break;            
        }
        case BLANK_AREA:
        case TO_OVERVIEW_AREA:
        case TO_CONFIG_AREA:
            break;
    }
}
/******************************************************************************
@Function: UIDrawNumericKeypadBackGround

@Description:画一个数字键盘，参数：
左上角的坐标
@Created: by Wangzilin

@Modified: 2019-04-25 20:46 by Wang Zilin
******************************************************************************/
int xBased, yBased;
static void UIDrawNumericKeypadBackGround(int x, int y)
{
    xBased = x;
    yBased = y;
    LCD_ShowString(x, y, 32, 32, 32, (u8*)"1", 0);
    LCD_ShowString(x + 56, y, 32, 32, 32, (u8*)"2", 0);
    LCD_ShowString(x + 112 , y, 32, 32, 32, (u8*)"3", 0);
    LCD_ShowString(x, y + 49, 32, 32, 32, (u8*)"4", 0);
    LCD_ShowString(x + 56, y + 49, 32, 32, 32, (u8*)"5", 0);
    LCD_ShowString(x + 112, y + 49, 32, 32, 32, (u8*)"6", 0);
    LCD_ShowString(x, y + 97, 32, 32, 32, (u8*)"7", 0);
    LCD_ShowString(x + 56, y + 97, 32, 32, 32, (u8*)"8", 0);
    LCD_ShowString(x + 112, y + 97, 32, 32, 32, (u8*)"9", 0);
    LCD_ShowString(x + 56, y + 139, 32, 32, 32, (u8*)"0", 0);
    LCD_ShowString(x, y + 145, 38, 24, 24, (u8*)"Cfm", 0);
    LCD_ShowString(x + 112, y + 145, 38, 24, 24, (u8*)"Del", 0);  
}
/******************************************************************************
@Function: UIGetNumericKeypad

@Description:
参数：
是否刷新
返回值：
1 点击了cfm
0 没有
@Created: by Wangzilin

@Modified: 2019-04-25 20:00 by Wang Zilin
******************************************************************************/
static u8 UIGetNumericKeypad(int x, int y, int* number)
{
    int xOffset = x-xBased;
    int yOffset = y-yBased;
    if(xOffset < 0 || yOffset < 0)
    {
        return 0;
    }
    if( number == NULL)
    {
        return 0;
    }
    //判断选择了哪个键
    //数字键
    if (xOffset < 32 && yOffset < 32 && xOffset > 0 && yOffset >0)
        *number = *number * 10 + 1;
    else if (xOffset < 56 + 32 && yOffset < 32 && xOffset > 56 && yOffset > 0)
        *number = *number * 10 + 2;
    else if (xOffset < 112 + 32 && yOffset < 32 && xOffset > 112 && yOffset > 0)
        *number = *number * 10 + 3;
    else if (xOffset < 32 && yOffset < 32 + 49 && xOffset > 0 && yOffset > 49)
        *number = *number * 10 + 4;
    else if (xOffset < 32 + 56 && yOffset < 32 + 49 && xOffset > 56 && yOffset > 49)
        *number = *number * 10 + 5;
    else if (xOffset < 32 + 112 && yOffset < 32 + 49 && xOffset > 112 && yOffset > 49)
        *number = *number * 10 + 6;
    else if (xOffset < 32 && yOffset < 32 + 97 && xOffset > 0 && yOffset > 97)
        *number = *number * 10 + 7;
    else if (xOffset < 32 + 56 && yOffset < 32 + 97 && xOffset > 56 && yOffset > 97)
        *number = *number * 10 + 8;
    else if (xOffset < 32 + 112 && yOffset < 32 + 97 && xOffset > 112 && yOffset > 97)
        *number = *number * 10 + 9;
    else if (xOffset < 32 + 56 && yOffset < 32 + 139 && xOffset > 56 && yOffset > 139)
        *number = *number * 10 + 0; 
    //控制键
    else if (xOffset < 32 && yOffset < 32 + 145 && xOffset > 0 && yOffset > 145)
    {
        DisplayDebugInformation("cfm");
        return 1;
    }
    else if (xOffset < 32 + 112 && yOffset < 32 + 145 && xOffset > 112 && yOffset > 145)
    {
        *number /= 10;
        DisplayDebugInformation("del");
    }
    DisplayDebugInformation("num");
    return 0;
}
/******************************************************************************
*  @Function: ConvertColor
*
*  @Description:将24位色转为16位色
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:49 by Wang Zilin
******************************************************************************/
u16 ConvertColor(u32 color)
{
    u8 red   = ((u8)(color >> 16)) >> 3;
    u8 green = ((u8)(color >> 8)) >> 2;
    u8 blue  = ((u8)(color)) >> 3;

    return((red << 13) | (green << 5) | blue);
}


/******************************************************************************
*  @Function: DisplayDebugInformation
*
*  @Description:在屏幕上显示调试信息
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:49 by Wang Zilin
******************************************************************************/
void DisplayDebugInformation(char *debugStr)
{
    static char debugStrBuffer[10][10];

    for (int i = 0; i < 9; i++)
    {
        strcpy(debugStrBuffer[i], debugStrBuffer[i + 1]);
    }
    strcpy(debugStrBuffer[9], debugStr);
    for (int i = 0; i < 10; i++)
    {
        LCD_ShowString(0, 150 + i * 12, 60, 12, 12, (u8 *)"     ", 0);
        LCD_ShowString(0, 150 + i * 12, 60, 12, 12, (u8 *)debugStrBuffer[i], 0);//如果没有数,则填上空白
    }
}
/******************************************************************************
@Function: UITestPoint

@Description:输入两次的触摸屏坐标，检验这两次是不是按了同一个位置

@Created: by Wangzilin

@Modified: 2019-04-26 09:52 by Wang Zilin
******************************************************************************/
u8 UITestPoint(int lastX, int lastY, int curX, int curY)
{
    int threshold = 50;
    int xOffset = lastX - curX;
    int yOffset = lastY - curY;
    if (xOffset < threshold && xOffset > -threshold && yOffset < threshold && yOffset > -threshold)
        return 1;
    return 0;
}
/******************************************************************************
@Function: UIScan

@Description:扫描触摸点

@Created: by Wangzilin

@Modified: 2019-04-26 10:03 by Wang Zilin
******************************************************************************/
u8 UIScan(int* x, int* y)
{
    OS_ERR err;
    tp_dev.scan(0);             
    if ((tp_dev.sta)&1)//有点按下
    {
        tp_dev.sta = 0;
        int lastX= tp_dev.x[0];
        int lastY= tp_dev.y[0];
        tp_dev.x[0] = 0;
        tp_dev.y[0] = 0;
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);        
        tp_dev.scan(0);    
        if ((tp_dev.sta)&1)//有点按下
        {
            tp_dev.sta = 0;
            int curX= tp_dev.x[0];
            int curY= tp_dev.y[0];
            tp_dev.x[0] = 0;
            tp_dev.y[0] = 0;
            if (UITestPoint(lastX, lastY, curX, curY))
            {
                *x = curX;
                *y = curY;
                printf("point %d,%d\r\n",*x, *y);
                return 1;
            }
        }
    }
    *x = *y = 0;
    return 0;
}