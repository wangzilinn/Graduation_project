#ifndef __MODBUS_METHOD__
#define __MODBUS_METHOD__

#include "mb.h"
#include "monitoring_node_data_storage_module.h"

void ModbusConvertStringToHoldingBuffer(char* str, u16* buffer, int bufferLength);
void NodeDataToJSON(NodeDataStruct nodeData, char* str);
#endif