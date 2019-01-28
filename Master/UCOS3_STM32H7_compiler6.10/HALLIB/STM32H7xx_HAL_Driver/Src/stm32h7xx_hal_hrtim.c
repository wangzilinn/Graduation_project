/**
  ******************************************************************************
  * @file    stm32h7xx_hal_hrtim.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date   29-December-2017
  * @brief   HRTIM HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the High Resolution Timer (HRTIM) peripheral:
  *           + HRTIM Initialization
  *           + Timer Time Base Unit Configuration
  *           + Simple Time Base Start/Stop
  *           + Simple Time Base Start/Stop Interrupt
  *           + Simple Time Base Start/Stop DMA Request
  *           + Simple Output Compare/PWM Channel Configuration
  *           + Simple Output Compare/PWM Channel Start/Stop Interrupt
  *           + Simple Output Compare/PWM Channel Start/Stop DMA Request
  *           + Simple Input Capture Channel Configuration
  *           + Simple Input Capture Channel Start/Stop Interrupt
  *           + Simple Input Capture Channel Start/Stop DMA Request
  *           + Simple One Pulse Channel Configuration
  *           + Simple One Pulse Channel Start/Stop Interrupt
  *           + HRTIM External Synchronization Configuration
  *           + HRTIM Burst Mode Controller Configuration
  *           + HRTIM Burst Mode Controller Enabling
  *           + HRTIM External Events Conditioning Configuration
  *           + HRTIM Faults Conditioning Configuration
  *           + HRTIM Faults Enabling
  *           + HRTIM ADC trigger Configuration
  *           + Waveform Timer Configuration
  *           + Waveform Event Filtering Configuration
  *           + Waveform Dead Time Insertion Configuration
  *           + Waveform Chopper Mode Configuration
  *           + Waveform Compare Unit Configuration
  *           + Waveform Capture Unit Configuration
  *           + Waveform Output Configuration
  *           + Waveform Counter Start/Stop
  *           + Waveform Counter Start/Stop Interrupt
  *           + Waveform Counter Start/Stop DMA Request
  *           + Waveform Output Enabling
  *           + Waveform Output Level Set/Get
  *           + Waveform Output State Get
  *           + Waveform Burst DMA Operation Configuration
  *           + Waveform Burst DMA Operation Start
  *           + Waveform Timer Counter Software Reset
  *           + Waveform Capture Software Trigger 
  *           + Waveform Burst Mode Controller Software Trigger
  *           + Waveform Timer Pre-loadable Registers Update Enabling
  *           + Waveform Timer Pre-loadable Registers Software Update
  *           + Waveform Timer Delayed Protection Status Get
  *           + Waveform Timer Burst Status Get
  *           + Waveform Timer Push-Pull Status Get
  *           + Peripheral State Get
  @verbatim
  ==============================================================================
                      ##### Simple mode v.s. waveform mode #####
==============================================================================
  [..] The HRTIM HAL API is split into 2 categories:
    (#)Simple functions: these functions allow for using a HRTIM timer as a  
        general purpose timer with high resolution capabilities.
       Following simple modes are proposed:
         (+)Output compare mode
         (+)PWM output mode
         (+)Input capture mode
         (+)One pulse mode
       HRTIM simple modes are managed through the set of functions named
       HAL_HRTIM_Simple<Function>. These functions are similar in name and usage 
       to the one defined for the TIM peripheral. When a HRTIM timer operates in 
       simple mode, only a very limited set of HRTIM features are used.
    (#)Waveform functions: These functions allow taking advantage of the HRTIM 
       flexibility to produce numerous types of control signal. When a HRTIM timer 
       operates in waveform mode, all the HRTIM features are accessible without 
       any restriction.  HRTIM waveform modes are managed through the set of
       functions named HAL_HRTIM_Waveform<Function>

==============================================================================
                      ##### How to use this driver #####
==============================================================================
    [..]
     (#)Initialize the HRTIM low level resources by implementing the
        HAL_HRTIM_MspInit() function:
        (##)Enable the HRTIM clock source using __HRTIMx_CLK_ENABLE() 
        (##)Connect HRTIM pins to MCU I/Os
            (+++) Enable the clock for the HRTIM GPIOs using the following
                  function: __GPIOx_CLK_ENABLE()   
            (+++) Configure these GPIO pins in Alternate Function mode using
                  HAL_GPIO_Init()
        (##)When using DMA to control data transfer (e.g HAL_HRTIM_SimpleBaseStart_DMA())
            (+++)Enable the DMAx interface clock using __DMAx_CLK_ENABLE()
            (+++)Initialize the DMA handle
            (+++)Associate the initialized DMA handle to the appropriate DMA 
                 handle of the HRTIM handle using  __HAL_LINKDMA()
            (+++)Initialize the DMA channel using HAL_DMA_Init()
            (+++)Configure the priority and enable the NVIC for the transfer
                 complete interrupt on the DMA channel using HAL_NVIC_SetPriority()
                 and HAL_NVIC_EnableIRQ()
        (##)In case of using interrupt mode (e.g HAL_HRTIM_SimpleBaseStart_IT())
            (+++)Configure the priority and enable the NVIC for the concerned
                 HRTIM interrupt using HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ()

    (#)Initialize the HRTIM HAL using HAL_HRTIM_Init(). The HRTIM configuration 
       structure (field of the HRTIM handle) specifies which global interrupt of
       whole HRTIM must be enabled (Burst mode period, System fault, Faults).
       It also contains the HRTIM external synchronization configuration. HRTIM
       can act as a master (generating a synchronization signal) or as a slave
       (waiting for a trigger to be synchronized).

     (#) Configure HRTIM resources shared by all HRTIM timers
        (##)Burst Mode Controller:
                (+++)HAL_HRTIM_BurstModeConfig(): configures the HRTIM burst mode
                     controller: operating mode (continuous or -shot mode), clock
                     (source, prescaler) , trigger(s), period, idle duration.
        (##)External Events Conditionning:
                (+++)HAL_HRTIM_EventConfig(): configures the conditioning of an
                     external event channel: source, polarity, edge-sensitivity.
                     External event can be used as triggers (timer reset, input
                     capture, burst mode, ADC triggers, delayed protection, 