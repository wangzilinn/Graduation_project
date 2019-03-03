#ifndef __LED__
#define __LED__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"


/******************************************************************************
@Class: LED

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:08 by Wang Zilin
******************************************************************************/
class LED
{
    public:
        enum status{
            ON = 0,
            OFF = 1
        };
    private:
        int __lebal;
        GPIO_TypeDef* __GPIOx;
        uint16_t __GPIOPin;
        enum status __LEDStatus;
        void __ChangeToStatus();   
    public:
        static void BSPInit(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
        LED(GPIO_TypeDef* GPIOx, uint16_t GPIOPin, int lebal, enum status theStatus):
            __lebal(lebal), __GPIOx(GPIOx), __GPIOPin(GPIOPin), __LEDStatus(theStatus) {__ChangeToStatus();}
        LED(GPIO_TypeDef* GPIOx, uint16_t GPIOPin, int lebal):
            __lebal(lebal), __GPIOx(GPIOx), __GPIOPin(GPIOPin), __LEDStatus(ON) {__ChangeToStatus();}
        void Toggle();
        void TurnOn();
        void TurnOff();
};
#endif
