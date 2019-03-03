/******************************************************************************
Include header files
******************************************************************************/
#include "usart_keyboard.h"

/******************************************************************************
@Function: __GetKeyMapIndex

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:23 by Wang Zilin
******************************************************************************/
int USARTKeyboard::__GetKeyMapIndex(const char* keyName)
{
    for (int i = 0; i < __keyMapLength; i++)
    {
        if (strcmp(keyName, __keyMap[i].GetKeyName()) == 0)
        {
            return i;
        }
    } 
    return 0;
}

/******************************************************************************
@Function: SendKey

@Description:
    send single key

@Created: by Wang Zilin

@Modified: 2018-11-11 17:24 by Wang Zilin
******************************************************************************/
void USARTKeyboard::SendKey(const char* keyName)
{
    SendKey(&keyName, 1);
}

/******************************************************************************
@Function: SendKey

@Description:
    send key list, 

@Created: by Wang Zilin

@Modified: 2018-11-11 17:24 by Wang Zilin
******************************************************************************/
void USARTKeyboard::SendKey(const char** KeyNameList, int listLength)
{
    u8 sendDataArray[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int normalKeyPointer = 2;//After the third byte is normal buttons
    for (int i = 0; i < listLength; i++)
    {
        int keyMapIndex = __GetKeyMapIndex(*KeyNameList++);
        if (__keyMap[keyMapIndex].IsFunctionKey())
        {
            sendDataArray[0] |= __keyMap[keyMapIndex].GetValue();
        }
        else
        {
            sendDataArray[normalKeyPointer] = __keyMap[keyMapIndex].GetValue();
            normalKeyPointer++;
            if (normalKeyPointer == 8)
                break;//array's length is 8
        }
    }
    __outputUSART.SendArray(sendDataArray, 8);
}