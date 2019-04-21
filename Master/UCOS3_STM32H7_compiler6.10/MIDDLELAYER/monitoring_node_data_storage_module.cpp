/******************************************************************************
*  Include headers
******************************************************************************/
#include "monitoring_node_data_storage_module.h"

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
OS_Q latestNodeDataQuene;
NodeDataStruct latestNodeDataBuffer[10];
int latestNodeDataBufferPointer = 0;
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
    //这个消息队列内储存最新节点的数据
    OS_ERR err;
    OSQCreate(&latestNodeDataQuene, (char*)"the latest node data quene", 10, &err);
}


/******************************************************************************
*  @Function: UpdateLocatDataSet
*
*  @Description:从串口那里更新获取到的数据到参数集
* 注意：localDataSet.nodeData[]中0不被使用。
*
*  @Created: by Wangzilin
*
*  @Modified:2019-03-30 10:21 by Wang Zilin
******************************************************************************/
void UpdateLocatDataSet(ReceivedNodeDataStruct *receivedNodeData)
{
    //清空所有活跃点记录,并刷新每个节点的更新计数器
    for (int i = 0; i < MAX_NUMBER_OF_NODES; i++)
    {
        localDataSet.activeNodeArray[i] = 0;
        if (localDataSet.nodeData[i + 1].deviceStatus == RUNNING || localDataSet.nodeData[i + 1].deviceStatus == OFFLINE)
        {
            localDataSet.nodeData[i + 1].refreshCnt++;
        }
    }
    
    //储存新接收的数据
    u8 nodeAddress = receivedNodeData->localShortAddress;
    localDataSet.nodeData[nodeAddress].receivedNodeData = *receivedNodeData;
    localDataSet.nodeData[nodeAddress].refreshCnt       = 0;
    
    //更新所有节点运行状态
    for (int i = 1, activeNodeCnt = 0; i <= MAX_NUMBER_OF_NODES; i++)
    {
        int refreshCnt = localDataSet.nodeData[i].refreshCnt;
        if (refreshCnt > localDataSet.closedRefreshCntThreshold)
        {
            localDataSet.nodeData[i].deviceStatus = CLOSED;
        }
        else if (refreshCnt > localDataSet.offlineRefreshCntThreshold)
        {
            localDataSet.nodeData[i].deviceStatus = OFFLINE;      
        }
        else
        {
            localDataSet.nodeData[i].deviceStatus = RUNNING;
            localDataSet.activeNodeArray[activeNodeCnt++] = i;
        }
    }
    OS_ERR err;
    latestNodeDataBuffer[latestNodeDataBufferPointer] = localDataSet.nodeData[nodeAddress];
    OSQPost(&latestNodeDataQuene, &latestNodeDataBuffer[latestNodeDataBufferPointer++], sizeof(NodeDataStruct), OS_OPT_POST_FIFO, &err);
    if (latestNodeDataBufferPointer >= 10)
    {
        latestNodeDataBufferPointer = 0;
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
