/******************************************************************************
Include headers
******************************************************************************/
#include "applicationLayer.h"
/******************************************************************************
@Function: NodeDataStruct ReceiveNodeData()

@Description:

@Created: by Wang Zilin

@Modified: 2019-03-04 22:08 by Wang Zilin
******************************************************************************/
NodeDataStruct ReceiveNodeData()
{
    u8 receivedData[20] = {0};
    while(1)//loop until get correct data
    {
        u8* quene;
        //get data from message quene
        if (ZigBeeReceive(quene, receivedData, 20) == true)
        {
            //check sum
            u8 sum = 0;
            for (u8 i = 0; i < 11; i++)
            {
                sum += receivedData[i];
            }
            if (sum == receivedData[11])//sum check pass
                break;
        }
    }
    NodeDataStruct nodeData;
    nodeData.Temperature = Method::U8ToFloat(receivedData);
    nodeData.Humidity = Method::U8ToFloat(&receivedData[4]);
    nodeData.nodeId = Method::U8ToU16(&receivedData[8]);
    nodeData.controlWord = receivedData[10];
    return nodeData;
}