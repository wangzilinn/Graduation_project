/******************************************************************************
*  Include headers
******************************************************************************/
#include "hardware_init.h"

/******************************************************************************
*  @Function: 汇编指令封装
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-17 20:56 by Wang Zilin
******************************************************************************/
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
void __attribute__((noinline)) WFI_SET(void)
{
    __asm__ ("wfi");
}


//关闭所有中断(但是不包括fault和NMI中断)
void __attribute__((noinline)) INTX_DISABLE(void)
{
    __asm__ ("cpsid i \t\n"
             "bx lr");
}


//开启所有中断
void __attribute__((noinline)) INTX_ENABLE(void)
{
    __asm__ ("cpsie i \t\n"
             "bx lr");
}


//设置栈顶地址
//addr:栈顶地址
void __attribute__((noinline)) MSR_MSP(u32 addr)
{
    __asm__ ("msr msp, r0 \t\n"
             "bx r14");
}


/******************************************************************************
*  @Function: Cache_Enable
*
*  @Description:打开Cache
*
*  @Created: by Wang Zilin
*
*  @Modified:
******************************************************************************/
#define Write_Through()    (*(__IO uint32_t *)0XE000EF9C = 1UL << 2) //Cache透写模式
void Cache_Enable(void)
{
    SCB_EnableICache();  //使能I-Cache
    SCB_EnableDCache();  //使能D-Cache
    SCB->CACR |= 1 << 2; //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题
}


/******************************************************************************
*  @Function: Get_xCahceSta
*
*  @Description:
*  判断x_Cache是否打开
*  返回值:0 关闭，1 打开
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-17 20:55 by Wang Zilin
******************************************************************************/
u8 Get_ICahceSta(void)
{
    u8 sta;

    sta = ((SCB->CCR) >> 17) & 0X01;
    return sta;
}


u8 Get_DCahceSta(void)
{
    u8 sta;

    sta = ((SCB->CCR) >> 16) & 0X01;
    return sta;
}


/******************************************************************************
*  @Function: SystemClockInit
*
*  @Description:400MHz
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-17 20:53 by Wang Zilin
******************************************************************************/
void SystemClockInit()
{
    __HAL_RCC_CSI_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_EnableCompensationCell();

    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /**Supply configuration update enable
     */
    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

    /**Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
    {
    }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 5;
    RCC_OscInitStruct.PLL.PLLN       = 160;
    RCC_OscInitStruct.PLL.PLLP       = 2;
    RCC_OscInitStruct.PLL.PLLQ       = 2;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1)
        {
        }
    }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                  | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        while (1)
        {
        }
    }
}


/******************************************************************************
*  @Function: SystickInit
*
*  @Description:
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-16 18:04 by Wang Zilin
******************************************************************************/
void SystickInit(u16 SYSCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为HCLK
    u32 reload;
    reload  = SYSCLK;                                    //每秒钟的计数次数 单位为K
    reload *= 1000000 / OSCfg_TickRate_Hz;               //根据delay_ostickspersec设定溢出时间
                                                         //reload为24位寄存器,最大值:16777216,在216M下,约合77.7ms左右
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;           //开启SYSTICK中断
    SysTick->LOAD  = reload;                             //每1/OS_TICKS_PER_SEC秒中断一次
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;            //开启SYSTICK
}


/******************************************************************************
*  @Function: hardwareInit
*
*  @Description:硬件初始化
*
*  @Created: by Wang Zilin
*
*  @Modified: 2018-12-17 20:46 by Wang Zilin
******************************************************************************/
void HardwareInit(void)
{
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();                     //初始化HAL库
    SystemClockInit();              //设置时钟,400Mhz
    SystickInit(400);               //延时初始化
    Uart1Init(115200);              //串口初始化
    Uart2Init(115200);
    //Uart3Init(115200);
    LEDInit();                      //初始化LED
}
