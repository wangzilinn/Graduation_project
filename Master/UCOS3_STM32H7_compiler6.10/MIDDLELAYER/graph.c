#include "graph.h"
static u16 __colorBuffer[3][360] = {0};
void UIDrawBackground()
{
    UIDrawLine(0, 30, 0xB9042E);
    UIDrawLine(30, 60, 0xB70029);
    UIDrawLine(90, 390, 0x141B23);
    POINT_COLOR = 0xFFFFFFFF;
    LCD_ShowString(40, 10, 120, 32, 32, (u8*)"Overview", 1);
    POINT_COLOR = ConvertColor(0xB9042E);
    LCD_ShowString(90, 50, 390, 24, 24, (u8*)" ID|    COORD    |TEMP|HUMI|STAT", 1);
}
void UIDrawLine(u16 offset, u16 width, u32 color)
{
    u16 _color = ConvertColor(color);
    for (int i = 0; i < 3; i++)
        for (int j = 0;j < 360; j++)
            __colorBuffer[i][j] = _color;
    int blockNumber = width / 3;
    for (int i = 0; i < blockNumber; i++)
    {
        LCD_Color_Fill(offset + 3 * i, 0, offset + 3 * i + 3, 272, *__colorBuffer);
    }
}
void UIDrawNodeData(LocalDataSetStruct* localDataSet, int page)
{
    BACK_COLOR =  ConvertColor(0x141B23);
    POINT_COLOR = ConvertColor(0xFFFFFF);
    for (int i = page; i < page + 5; i++)
    {
        int displayNodeNumber = localDataSet->activeNodeArray[i];
        if (displayNodeNumber != 0)
        {
            char displayStr[40] = {0};
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

            LCD_ShowString(90, 50 + i * 24, 390, 24, 24, (u8*)displayStr, 0);
        }
        else
        {
            LCD_ShowString(90, 50 + i * 24, 390, 24, 24, (u8*)"                                 ", 0);//如果没有数,则填上空白
        }
    }
}
u16 ConvertColor(u32 color)
{
    u8 red = ((u8)(color >> 16))>>3;
    u8 green = ((u8)(color >> 8))>>2;
    u8 blue = ((u8)(color))>>3;
    return ((red<<13) | (green << 5) | blue);
}
void DisplayDebugInformation(char* debugStr)
{
    static char debugStrBuffer[10][10];
    for (int i = 0; i < 9; i++)
    {
        strcpy(debugStrBuffer[i], debugStrBuffer[i + 1]);
    }
    strcpy(debugStrBuffer[9], debugStr);
    for (int i = 0; i < 10; i++)
    {
        LCD_ShowString(30, 150 + i * 12, 60, 12, 12, (u8*)debugStrBuffer[i], 0);//如果没有数,则填上空白
    }
}