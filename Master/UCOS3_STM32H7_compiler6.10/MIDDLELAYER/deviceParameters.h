#ifndef __DEVICEPARAMETER__
#define __DEVICEPARAMETER__
#include "public.h"
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
}NodeDataStruct;
typedef struct collectionNodeParameter
{
    int offLineTime;
    int activeNodeArray[MAX_NUMBER_OF_NODES];
    NodeDataStruct* nodeData;
}LocalDataSetStruct;
extern LocalDataSetStruct localDataSet;
extern NodeDataStruct nodeDataArray[];
void DeviceParametersInit();
#endif