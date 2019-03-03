#ifndef __PEDAL__
#define __PEDAL__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"
/******************************************************************************
@Class: Pedal

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:18 by Wang Zilin
******************************************************************************/

class Pedal
{
    private:
        int __label;
        bool __pedalStatus;
    public:
        static void BSPInit();
        Pedal(int label):
            __label(label){};
        int GetLabel();
        bool IsDepressed();
        void IsDepressed(bool status);
};
#endif
