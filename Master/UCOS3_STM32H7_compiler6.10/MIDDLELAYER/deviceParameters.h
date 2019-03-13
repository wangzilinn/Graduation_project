#ifndef __DEVICEPARAMETER__
#define __DEVICEPARAMETER__
#include "public.h"
#ifdef __cplusplus
extern "C" { 
#endif
#define MAX_NUMBER_OF_NODES 255
typedef struct coordinate
{
    int x;
    int y;
    int z;
}CoordinateStruct;
typedef enum deviceStatus
{
    RUNNING = 0,
    OFFLINE,
    CLOSED,
}DeviceStatusEnum;

typedef struct receivedNodeData
{
    u16 localShortAddress;
    float temperature;
    float humidity;
    u8 controlWord;
}ReceivedNodeDataStruct;
typedef struct nodeData
{
    ReceivedNodeDataStruct receivedNodeData;
    CoordinateStruct coordinate;
    DeviceStatusEnum deviceStatus;
    int refreshCnt;
}NodeDataStruct;
typedef struct localDataSetStruct
{
    int offlineRefreshCntThreshold;
    int closedRefreshCntThreshold;
    int activeNodeArray[MAX_NUMBER_OF_NODES + 1];
    NodeDataStruct nodeData[MAX_NUMBER_OF_NODES + 1];
}LocalDataSetStruct;
extern LocalDataSetStruct localDataSet;
extern void LocalDataSetInit();
extern void UpdateLocatDataSet(ReceivedNodeDataStruct* receivedNodeData);
#ifdef __cplusplus
}
#endif
#endif