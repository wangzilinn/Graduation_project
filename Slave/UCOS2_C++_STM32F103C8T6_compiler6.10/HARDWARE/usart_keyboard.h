#ifndef __USART_KEYBOARD__
#define __USART_KEYBOARD__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"
#include "usart.h"
/******************************************************************************
@Class: Key

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:25 by Wang Zilin
******************************************************************************/
class Key
{
    private:
        const char* __key;
        u8 __value;
        bool __isFunctionKey;
    public:
        Key(const char * key, u8 value, bool isFunctionKey):
            __value(value), __isFunctionKey(isFunctionKey)
        {
            __key = key;
        }
        const char* GetKeyName() const 
        {
            return __key;
        }
        u8 GetValue() const
        {
            return __value;
        }
        bool IsFunctionKey() const
        {
            return __isFunctionKey;
        }
};

/******************************************************************************
@Class: USARTKeyboard

@Description:
    Do not use the STL map when mapping key values, 
    ecause the template feature in C++ is not 
    supported in stm32f10x_it.h(extern "C").

@Created: by Wang Zilin

@Modified: 2018-11-11 17:26 by Wang Zilin
******************************************************************************/
class USARTKeyboard
{
    public:
        enum Status
        {
            PRESS = 0,
            RELEASE
        };         
    private:
        USART __outputUSART;
        const static int __keyMapLength = 6;
        const Key __keyMap[__keyMapLength] = {
                    Key("release", 0x00, true),
                    Key("leftShift", 0x02, true),
                    Key("win", 0x08, true),
                    Key("leftControl", 0x01, true),   
                    Key("enter", 0x28, false),
                    Key("l", 0x0f, false)
                   };
        int __GetKeyMapIndex(const char* keyName);
    public:
        USARTKeyboard(USART outputUSART):
            __outputUSART(outputUSART){};
        void SendKey(const char* keyName); 
        void SendKey(const char** KeyNameList, int listLength);
        
};
#endif