#include "mb_method.h"
/******************************************************************************
@Function: ModbusConvertStringToHoldingBuffer

@Description:将字符串转为Holding寄存器中的字符

@Created: by Wangzilin

@Modified: 2019-04-19 15:28 by Wang Zilin
******************************************************************************/
void ModbusConvertStringToHoldingBuffer(char* str, u16* buffer, int bufferLength)
{
    memcpy(buffer, str, strlen(str)+1);
    for (int i = 0; i < bufferLength; i++)
    {
        buffer[i] = (buffer[i] << 8) | (buffer[i] >> 8);//高8位低8位互换
    }
}
/******************************************************************************
@Function: NodeDataToJSON

@Description:将获得的最新数据转换为json字符串

@Created: by Wangzilin

@Modified: 2019-04-19 20:52 by Wang Zilin
******************************************************************************/
void NodeDataToJSON(NodeDataStruct nodeData, char* str)
{
    sprintf(str, "{\"position\":{\"x\":%d,\"y\":%d,\"z\":%d},\"temp\":%d.%d,\"humi\":%d}", 
    nodeData.coordinate.x, nodeData.coordinate.y, nodeData.coordinate.z, 
    (int)nodeData.receivedNodeData.temperature, (int)((nodeData.receivedNodeData.temperature - (int)nodeData.receivedNodeData.temperature) * 10),
        (int)nodeData.receivedNodeData.humidity);
}