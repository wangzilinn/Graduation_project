#ifndef __FAN__
#define __FAN__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"

/******************************************************************************
@Class: Fan

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:31 by Wang Zilin
******************************************************************************/
class Fan
{
    public:
        enum Status
        {
            ON = 0,
            OFF
        };
    private:
        Status __fanStatus;
        GPIO_TypeDef* __GPIOx1;
        uint16_t __GPIOPin1;
        GPIO_TypeDef* __GPIOx2;
        uint16_t __GPIOPin2;
    public:
        static void BSPInit();
        Fan(GPIO_TypeDef* GPIOx1, uint16_t GPIOPin1,GPIO_TypeDef* GPIOx2, uint16_t GPIOPin2);
        void SetStatus(Status funStatus);
        Status GetStatus();
        bool IsRunning();  
};
#endif