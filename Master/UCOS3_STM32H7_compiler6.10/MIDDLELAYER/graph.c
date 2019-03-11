#include "graph.h"
static u16 __colorBuffer[3][360] = {0};
void UIDrawBackground()
{
    UIDrawLine(0, 30, 0xB9042E);
    UIDrawLine(30, 60, 0xB70029);
    UIDrawLine(90, 390, 0x141B23);
    POINT_COLOR = 0xFFFFFFFF;
    LCD_ShowString(40, 10, 120, 32, 32, (u8*)"Overview");
    POINT_COLOR = ConvertColor(0xB9042E);
    LCD_ShowString(100, 50, 390, 24, 24, (u8*)"ID   COORD   TEMP   HUMI   STAT");
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
void UIDrawNodeData(NodeDataStruct* nodedataArray, int * refreshCnt)
{
    int lineCnt = 0;
}  
u16 ConvertColor(u32 color)
{
    u8 red = ((u8)(color >> 16))>>3;
    u8 green = ((u8)(color >> 8))>>2;
    u8 blue = ((u8)(color))>>3;
    return ((red<<13) | (green << 5) | blue);
}
    