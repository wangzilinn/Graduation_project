#include "deviceParameters.h"
/******************************************************************************
汇集点参数定义
******************************************************************************/
LocalDataSetStruct localDataSet;
NodeDataStruct nodeDataArray[MAX_NUMBER_OF_NODES];
void DeviceParametersInit()
{
    localDataSet.offLineTime = 10;
    localDataSet.nodeData = nodeDataArray;
    for (int i = 0; i < MAX_NUMBER_OF_NODES; i++)
    {
        localDataSet.nodeData[i].coordinate.x = i;
        localDataSet.nodeData[i].coordinate.y = i;
        localDataSet.nodeData[i].coordinate.z = i;
        localDataSet.nodeData[i].deviceStatus = CLOSED;
        localDataSet.activeNodeArray[i] = 0;
    }
}