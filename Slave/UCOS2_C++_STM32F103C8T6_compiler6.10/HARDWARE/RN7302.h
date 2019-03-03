#ifndef __RN7302_H
#define __RN7302_H
#include "public.h"
#include "stm32f10x.h"
/******************************************************************************
外设宏定义：
******************************************************************************/
#define SPI_RN7302 SPI1

#define CS_GPIOX			GPIOA
#define CS_GPIO_PIN			GPIO_Pin_4

#define SCLK_GPIOX			GPIOA
#define SCLK_GPIO_PIN		GPIO_Pin_5

#define MISO_GPIOX			GPIOA
#define MISO_GPIO_PIN		GPIO_Pin_6

#define MOSI_GPIOX			GPIOA
#define MOSI_GPIO_PIN		GPIO_Pin_7

#define CS_SET				GPIO_SetBits(CS_GPIOX, CS_GPIO_PIN)
#define CS_RESET			GPIO_ResetBits(CS_GPIOX, CS_GPIO_PIN)

#define SCLK_SET			GPIO_SetBits(SCLK_GPIOX, SCLK_GPIO_PIN)
#define SCLK_RESET			GPIO_ResetBits(SCLK_GPIOX, SCLK_GPIO_PIN)

#define MISO_READ			GPIO_ReadInputDataBit(MISO_GPIOX, MISO_GPIO_PIN)

#define MOSI_SET			GPIO_SetBits(MOSI_GPIOX, MOSI_GPIO_PIN)
#define MOSI_RESET			GPIO_ResetBits(MOSI_GPIOX, MOSI_GPIO_PIN)

#define RN7302_RECEIVE_BUFF_LINE_MAX 	10
#define RN7302_RECEIVE_BUFF_ROW_MAX 	10
#define RN7302_SEND_BUFF_ROW_MAX		10

//bank0 采样寄存器
#define UA		0x0007
#define UB		0x0008
#define UC		0x0009
#define IA		0x000B
#define IB		0x000C
#define IC		0x000D
#define IN		0x000E

//bank1 配置和状态寄存器空间
#define WREN	0x0180		//写使能寄存器
#define WMSW	0x0181		//工作模式切换寄存器
#define SOFTRST	0x0182		//软件复位寄存器
#define SYSSR	0x018A		//系统状态寄存器
/*****************************************************************************/
// CLASS NAME: RN7302
//
// DESCRIPTION:
//  RN7302电能芯片类
//
// CREATED: 2018-3-28 19:26:29 by zilin Wang
//
/*****************************************************************************/
class RN7302
{
private:
	u8 sendBuff;//发送缓冲区
    float sampValueToCurrent;
    void sendOneByte(u8 byte);
    u8 receiveOneByte(void);
    u8 sumCheck(u8 * buff, u8 length);
public:
    RN7302();
    static void BSPInit();
    void writeData(u16 address, u8 * pbuff, u8 datalength);
    void readData(u16 address, u8 * pbuff, u8 datalength);
    float getCrrent(u16 address);
};
#endif
