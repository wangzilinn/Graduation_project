#ifndef __DEVICE_PARAMETER__
#define __DEVICE_PARAMETER__

/******************************************************************************
*  Include headers
******************************************************************************/
#include "public.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*  采样点相关参数
******************************************************************************/
#define MAX_NUMBER_OF_NODES    255

/******************************************************************************
*  enum and struct definitions
******************************************************************************/
typedef struct coordinate
{
    int x;
    int y;
    int z;
} CoordinateStruct;
typedef enum deviceStatus
{
    RUNNING = 0,
    OFFLINE,
    CLOSED,
} DeviceStatusEnum;
typedef struct receivedNodeData
{
    u16   localShortAddress;
    float temperature;
    float humidity;
    u8    controlWord;
} ReceivedNodeDataStruct;
typedef struct nodeData
{
    ReceivedNodeDataStruct receivedNodeData;
    CoordinateStruct       coordinate;
    DeviceStatusEnum       deviceStatus;
    int                    refreshCnt;
} NodeDataStruct;
typedef struct localDataSetStruct
{
    int            offlineRefreshCntThreshold;
    int            closedRefreshCntThreshold;
    int            activeNodeArray[MAX_NUMBER_OF_NODES + 1]; //0节点不使用
    NodeDataStruct nodeData[MAX_NUMBER_OF_NODES + 1];        //0节点不使用
} LocalDataSetStruct;
extern LocalDataSetStruct localDataSet;

/******************************************************************************
*  Function declarations
******************************************************************************/
extern void LocalDataSetInit();
extern void UpdateLocatDataSet(ReceivedNodeDataStruct *receivedNodeData);
extern char *deviceStatusEnumToStr(DeviceStatusEnum deviceStatus, char *str);

#ifdef __cplusplus
}
#endif
#endif
