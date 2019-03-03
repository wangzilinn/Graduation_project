#include "applicationLayer.h"
/******************************************************************************
@Function: void SendNodeData(NodeDataStruct* nodeData);

@Description:发送节点数据

@Created: by Wang Zilin

@Modified: 2019-03-03 20:55 by Wang Zilin
******************************************************************************/
void SendNodeData(NodeDataStruct* nodeData)
{
    u8 packageArray[12] = {0};
    Method::FloatToU8(nodeData->Temperature, packageArray);//load Temperature
    Method::FloatToU8(nodeData->Humidity, &packageArray[4]);
    Method::U16ToU8(nodeData->nodeId, &packageArray[8]);
    packageArray[10] = nodeData->ontrolWord;
    packageArray[11] = 
    ({
        u8 sumCheck = 0;
        for (u8 i = 0; i < 11; i++)
        {
            sumCheck += packageArray[i];
        }
        sumCheck;
    }); 
    ZigBeeP2P(USART_ZIGBEE, 0x0001, packageArray, 12);
}