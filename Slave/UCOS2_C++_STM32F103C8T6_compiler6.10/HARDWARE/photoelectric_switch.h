#ifndef __PHOTOELECTRIC_SWITCH__
#define __PHOTOELECTRIC_SWITCH__
/******************************************************************************
Include header files
******************************************************************************/
#include "public.h"
/******************************************************************************
@Class: PhotoElectricSwitch

@Description:

@Created: by Wang Zilin

@Modified: 2018-11-11 17:22 by Wang Zilin
******************************************************************************/
class PhotoElectricSwitch
{
    private:
        bool __detected;
    public:
        static void BSPInit();
        PhotoElectricSwitch():__detected(true){};
        bool IsDetected();
        void IsDetected(bool status);        
};

#endif