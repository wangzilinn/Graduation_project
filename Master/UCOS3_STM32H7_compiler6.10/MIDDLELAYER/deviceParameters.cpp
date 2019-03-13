#include "deviceParameters.h"
/******************************************************************************
汇集点参数定义
******************************************************************************/
LocalDataSetStruct localDataSet;
void LocalDataSetInit()
{
    localDataSet.offlineRefreshCntThreshold = 5;
    localDataSet.closedRefreshCntThreshold = 10;
    for (int i = 0; i < MAX_NUMBER_OF_NODES; i++)
    {
        localDataSet.nodeData[i].coordinate = {
            .x = i,
            .y = i,
            .z = i
        };
        localDataSet.nodeData[i].refreshCnt = localDataSet.closedRefreshCntThreshold + 1;
        localDataSet.nodeData[i].deviceStatus = CLOSED;
        localDataSet.activeNodeArray[i] = 0;
    }
}
void UpdateLocatDataSet(ReceivedNodeDataStruct* receivedNodeData)
{
    u8 nodeID = receivedNodeData->localShortAddress;
    localDataSet.nodeData[nodeID].receivedNodeData = *receivedNodeData;
    localDataSet.nodeData[nodeID].deviceStatus = RUNNING;
    localDataSet.nodeData[nodeID].refreshCnt = 0;
    //更新活动节点列表以及下线时间
    for (int i = 0, activeCnt = 1; i < MAX_NUMBER_OF_NODES + 1; i++)
    {
        localDataSet.activeNodeArray[i] = 0;//先抹除活动点的数组
        localDataSet.nodeData[i].refreshCnt++;
        int refreshCnt = localDataSet.nodeData[i].refreshCnt;
        if (refreshCnt > localDataSet.offlineRefreshCntThreshold)
        {
            localDataSet.nodeData[i].deviceStatus = OFFLINE;
        }
        else if (refreshCnt > localDataSet.closedRefreshCntThreshold)
        {
            localDataSet.nodeData[i].deviceStatus = CLOSED;
            localDataSet.nodeData[i].refreshCnt--;//要是已经停机了, 就把刚刚加上的减回来
        }
        else
        {
            localDataSet.activeNodeArray[activeCnt++] = i;
        }
    }
            
}