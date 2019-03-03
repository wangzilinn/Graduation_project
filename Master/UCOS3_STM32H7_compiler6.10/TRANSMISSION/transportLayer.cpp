#include "transportLayer.h"
bool receiveFinished = false;
bool isValidPackage = false;
u8 totalSliceNumber = 0;
u8 currentSliceNumber = 0;
void ZigBeeReceive(u8* receiveArray, u8* dataArray, int dataArrayLength)
{
    totalSliceNumber = receiveArray[16];
    currentSliceNumber = receiveArray[17];
    do
    {
        
    } while ({/* condition */});
    
}