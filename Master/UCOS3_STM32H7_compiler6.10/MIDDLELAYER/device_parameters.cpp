/******************************************************************************
*  Include headers
******************************************************************************/
#include "device_parameters.h"

/******************************************************************************
*  汇集点参数定义
******************************************************************************/
LocalDataSetStruct localDataSet;

/******************************************************************************
*  @Function: LocalDataSetInit
*
*  @Description:汇聚点参数集初始化
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:20 by Wang Zilin
******************************************************************************/
void LocalDataSetInit()
{
    localDataSet.offlineRefreshCntThreshold = 10;
    localDataSet.closedRefreshCntThreshold  = 20;
    for (int i = 0; i < MAX_NUMBER_OF_NODES + 1; i++)
    {
        localDataSet.nodeData[i].coordinate = {
            .x = i,
            .y = i,
            .z = i
        };
        localDataSet.nodeData[i].refreshCnt   = localDataSet.closedRefreshCntThreshold + 1;
        localDataSet.nodeData[i].deviceStatus = CLOSED;
        localDataSet.activeNodeArray[i]       = 0;
    }
}


/******************************************************************************
*  @Function: UpdateLocatDataSet
*
*  @Description:从串口那里更新获取到的数据到参数集
*
*  @Created: by Wangzilin
*
*  @Modified:2019-03-30 10:21 by Wang Zilin
******************************************************************************/
void UpdateLocatDataSet(ReceivedNodeDataStruct *receivedNodeData)
{
    u8 nodeID = receivedNodeData->localShortAddress;

    localDataSet.nodeData[nodeID].receivedNodeData = *receivedNodeData;
    localDataSet.nodeData[nodeID].deviceStatus     = RUNNING;
    localDataSet.nodeData[nodeID].refreshCnt       = 0;
    //printf("t:%f, h:%f, ID:%d\r\n", receivedNodeData->temperature, receivedNodeData->humidity, receivedNodeData->localShortAddress);
    //更新活动节点列表以及下线时间
    for (int i = 0, activeCnt = 1; i < MAX_NUMBER_OF_NODES + 1; i++)
    {
        localDataSet.activeNodeArray[i] = 0;//先抹除活动点的数组
        localDataSet.nodeData[i].refreshCnt++;
        int refreshCnt = localDataSet.nodeData[i].refreshCnt;
        if (refreshCnt > localDataSet.closedRefreshCntThreshold)
        {
            localDataSet.nodeData[i].deviceStatus = CLOSED;
            localDataSet.nodeData[i].refreshCnt--;//要是已经停机了, 就把刚刚加上的减回来,防止溢出
        }
        else
        {
            if (refreshCnt > localDataSet.offlineRefreshCntThreshold)
            {
                localDataSet.nodeData[i].deviceStatus = OFFLINE;      //暂时下线的和激活的都算在激活列表中
            }
            localDataSet.activeNodeArray[activeCnt++] = i;
        }
    }
}


/******************************************************************************
*  @Function: deviceStatusEnumToStr
*
*  @Description:将设备状态的枚举量转变为字符串
*
*  @Created: by Wangzilin
*
*  @Modified: 2019-03-30 10:21 by Wang Zilin
******************************************************************************/
char *deviceStatusEnumToStr(DeviceStatusEnum deviceStatus, char *str)
{
    switch (deviceStatus)
    {
    case RUNNING:
        sprintf(str, "RUN");
        break;

    case OFFLINE:
        sprintf(str, "OFF");
        break;

    case CLOSED:
        sprintf(str, "CLS");
        break;
    }
    return str;
}
