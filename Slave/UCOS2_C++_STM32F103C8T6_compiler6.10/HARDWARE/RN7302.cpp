/******************************************************************************
包含头文件
******************************************************************************/
#include "RN7302.h"
/*****************************************************************************/
// FUNCTION NAME: BSPInit()
//
// DESCRIPTION:
//  
//
// CREATED: 2018-3-28 19:25:11 by zilin Wang
//
/*****************************************************************************/
void RN7302::BSPInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*****************************************************************************/
// FUNCTION NAME: RN7302
//
// DESCRIPTION:
//  构造函数
//
// CREATED: 2018-3-28 19:25:28 by zilin Wang
//
/*****************************************************************************/
RN7302::RN7302()
{
    /******************************************************************************
    采样值到电流值的转化
    ******************************************************************************/    
    sampValueToCurrent = 0.000001488;
    /******************************************************************************
    硬件初始化
    ******************************************************************************/
    u8 temp[3] = {0, 0, 0};
	readData(SYSSR, temp, 2);		//等待上电复位
	OSTimeDlyHMSM(0, 0, 0, 100);
	
	sendBuff = 0xE5;
	writeData(WREN, &sendBuff, 1);			//写使能
	OSTimeDlyHMSM(0, 0, 0, 100);
	
	sendBuff = 0xFA;
	writeData(SOFTRST, &sendBuff, 1);		//软件复位
	OSTimeDlyHMSM(0, 0, 1, 0);
	
	sendBuff = 0xE5;
	writeData(WREN, &sendBuff, 1);			//写使能
	OSTimeDlyHMSM(0, 0, 0, 100);

	sendBuff = 0xA2;
	writeData(0x0181, &sendBuff, 1);		//计量模式选择
	OSTimeDlyHMSM(0, 0, 0, 100);
}
/*****************************************************************************/
// FUNCTION NAME: int getCrrent(u16 address);
//
// DESCRIPTION:
//  获得电流值
//
// CREATED: 2018-3-21 20:42:28 by zilin Wang
//
/*****************************************************************************/
float RN7302::getCrrent(u16 address)
{
    u8 temp[4];
    Method::ClearStringBuff(temp, 4);
    readData(address, temp, 4);
    return Method::ByteToInt(temp) * sampValueToCurrent;  
}
/*****************************************************************************/
// FUNCTION NAME: writeData
//
// DESCRIPTION:
//  
//
// CREATED: 2018-3-28 19:25:39 by zilin Wang
//
/*****************************************************************************/
void RN7302::writeData(u16 address, u8 * pbuff, u8 datalength)
{
    u8 i;
	u8 checksum = 0;
	u8 addr;
	u8 cmd;
	
	CS_SET;
	
	addr = (u8)(address & 0x00FF);
	checksum = checksum + addr;
	cmd = (u8)((address >> 4) | 0x0080);
	checksum = checksum + cmd;
	
	CS_RESET;
	
	asm("nop");
	asm("nop");
	
	sendOneByte(addr);
	sendOneByte(cmd);
	
	for(i = 0; i < datalength; i++)
	{
		sendOneByte(*pbuff);
		checksum = checksum + *pbuff;
		pbuff++;
	}
	
	checksum = 0xFF - checksum;
	
	sendOneByte(checksum);
	
	asm("nop");
	asm("nop");
	
	CS_SET;
}
/*****************************************************************************/
// FUNCTION NAME: void RN7302::readData(u16 address, u8 * pbuff, u8 datalength)
//
// DESCRIPTION:
//  读数据
//
// CREATED: 2018-3-21 20:40:38 by zilin Wang
//
/*****************************************************************************/
void RN7302::readData(u16 address, u8 * pbuff, u8 datalength)
{
    u8 i;
	u8 checksum = 0;
	u8 addr;
	u8 cmd;
	u8 tempbuff[10];
	
	CS_SET;
	
	addr = (u8)(address & 0x00FF);
	checksum = checksum + addr;
	cmd = (u8)((address >> 4) & 0x0070);
	checksum = checksum + cmd;
	
	CS_RESET;
	
	asm("nop");
	asm("nop");
	
	sendOneByte(addr);
	sendOneByte(cmd);
	
	for(i = 0; i < datalength; i++)
	{
		tempbuff[i] = receiveOneByte();
		checksum = checksum + tempbuff[i];
	}
	
	tempbuff[datalength] = receiveOneByte();
	
	checksum = 0xFF - checksum;
	
	CS_SET;
	
	if(checksum == tempbuff[datalength])
	{
		for(i = 0; i < datalength; i++)
		{
			pbuff[i] = tempbuff[i];
		}
	}
}
/*****************************************************************************/
// FUNCTION NAME: u8 RN7302::sumCheck(u8 * buff, u8 length)
//
// DESCRIPTION:
//  和校验
//
// CREATED: 2018-3-21 20:40:57 by zilin Wang
//
/*****************************************************************************/
u8 RN7302::sumCheck(u8 * buff, u8 length)
{
    u8 i;
	
	u8 temp = 0;
	
	for(i = 0; i < length; i++)
	{
		temp = *buff;
		buff++;
	}
	
	temp = ~temp;
	
	return temp;
}
/*****************************************************************************/
// FUNCTION NAME: void RN7302::sendOneByte(u8 byte)
//
// DESCRIPTION:
//  发送一个字节
//
// CREATED: 2018-3-21 20:41:20 by zilin Wang
//
/*****************************************************************************/
void RN7302::sendOneByte(u8 byte)
{
    u8 i;
	
	SCLK_RESET;
	
	for(i = 0; i < 8; i++)
	{
		SCLK_SET;		//SPI通讯时钟SCLK置高，主机在SCLK高电平写数据 
		
		if(byte & 0x80)
		{
			MOSI_SET;
		}
		else
		{
			MOSI_RESET;
		}
		
		asm("nop");
		asm("nop");
		
		SCLK_RESET;		//从机在SCLK下降沿读取数据
		
		asm("nop");
		asm("nop");
		
		byte <<= 1;		//准备下一个数据位
	}
	
	SCLK_RESET;			//一个字节数据传送完毕,SPI通讯时钟SCLK恢复到初始状态高电平 
}
/*****************************************************************************/
// FUNCTION NAME: u8 RN7302::receiveOneByte(void)
//
// DESCRIPTION:
//  读取一个字节
//
// CREATED: 2018-3-21 20:41:42 by zilin Wang
//
/*****************************************************************************/
u8 RN7302::receiveOneByte(void)
{
    u8 i;
	u8 temp = 0;
	
	SCLK_RESET;
	
	for(i = 0; i < 8; i++)
	{
		SCLK_SET;		//SPI通讯时钟SCLK置高，从机在SCLK高电平写数据
		
		asm("nop");
		asm("nop");
		
		SCLK_RESET;		//主机在SCLK下降沿读取数据
		
		temp <<= 1;
		temp |= MISO_READ;
	}
	
	SCLK_RESET;			//一个字节数据传送完毕,SPI通讯时钟SCLK恢复到初始状态高电平
	
	return temp;
}
