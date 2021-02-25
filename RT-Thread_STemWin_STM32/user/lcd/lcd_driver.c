#include "lcd_driver.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "rtthread.h"
#include "rthw.h"
#include "stdlib.h"
#include "touch.h"
//#include "usart.h"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,����ԭ��MiniSTM32������,��Ƶ72MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567
//�ֻ�:15989313508���빤��
//����:goodtft@126.com
//Skype:QDtech2008
//����֧��QQ:3002773612
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V2.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2013-2019
//All rights reserved
/****************************************************************************************************

//====================================��Դ����==========================================//
//VCC        ---�� 5V��3.3V��Դ
//GND        ---�� ��

//===============================Һ���������߽���======================================//
//CS	       ---��PB11	//Һ����Ƭѡ�ź�
//RESET	     ---��PB12	//Һ������λ�ź�
//DC/RS	     ---��PB10	//�Ĵ���/����ѡ���ź�
//SDI(MOSI)	 ---��PB15	//SPI����д����
//SCK	       ---��PB13	//SPI����ʱ���ź�
//LED	       ---��PB9	 //��������ź�(�ߵ�ƽ����)�������Ҫ���ƿɽ�VCC����
//SDO(MISO)  ---��PB14 //SPI���ź��������ȡ�ɲ���

//===============================����������========================================//
//��ʹ�ô�������ģ�鱾��������������ɲ�����  
//T_CLK	     ---��PC0	 //����SPI����ʱ��
//T_CS	     ---��PC13 //����ICƬѡ
//T_DIN	     ---��PC3	//����SPI��������
//T_DO	     ---��PC2	//����SPI�������
//T_IRQ	     ---��PC1	//�������ж��ź�

*************************************************************************************************/

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
u16 DeviceCode;

void delay_ms(int count)
{
	count = count * 1000;
	for (; count; count--)
		;
}

/****************************************************************************
* ��    �ƣ�void  SPIv_WriteData(u8 Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void SPIv_WriteData(u8 Data)
{
	unsigned char i = 0;
	for (i = 8; i > 0; i--)
	{
		if (Data & 0x80)
			LCD_SDA_SET; //�������
		else
			LCD_SDA_CLR;

		LCD_SCL_CLR;
		LCD_SCL_SET;
		Data <<= 1;
	}
}
/****************************************************************************
* ��    �ƣ�u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
* ��    �ܣ�STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
* ��ڲ�����SPIx,Byte
* ���ڲ��������������յ�������
* ˵    ����STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef *SPIx, u8 Byte)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET)
		;			 //�ȴ���������
	SPIx->DR = Byte; //����һ��byte
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET)
		;			 //�ȴ�������һ��byte
	return SPIx->DR; //�����յ�������
}

/****************************************************************************
* ��    �ƣ�void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
* ��    �ܣ�����SPI���ٶ�
* ��ڲ�����SPIx,SpeedSet
* ���ڲ�������
* ˵    ����SpeedSet:1,����;0,����;
****************************************************************************/
void SPI_SetSpeed(SPI_TypeDef *SPIx, u8 SpeedSet)
{
	SPIx->CR1 &= 0XFFC7;
	if (SpeedSet == 1) //����
	{
		SPIx->CR1 |= SPI_BaudRatePrescaler_2; //Fsck=Fpclk/2
	}
	else //����
	{
		SPIx->CR1 |= SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPIx->CR1 |= 1 << 6; //SPI�豸ʹ��
}

/****************************************************************************
* ��    �ƣ�SPI2_Init(void)
* ��    �ܣ�STM32_SPI2Ӳ�����ó�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����STM32_SPI2Ӳ�����ó�ʼ��
****************************************************************************/
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//����SPI2�ܽ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //SCK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //SDA
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//SPI2����ѡ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	//ʹ��SPI2
	SPI_Cmd(SPI2, ENABLE);
}

//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(u8 data)
{
	LCD_CS_CLR;
	LCD_RS_CLR;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2, data);
#else
	SPIv_WriteData(data);
#endif
	LCD_CS_SET;
}

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ����дһ��8λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u8 data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2, data);
#else
	SPIv_WriteData(data);
#endif
	LCD_CS_SET;
}
//******************************************************************
//��������  LCD_DrawPoint_16Bit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    8λ���������д��һ��16λ����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
	LCD_CS_CLR;
	LCD_RS_SET;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2, color >> 8);
	SPI_WriteByte(SPI2, color);
#else
	SPIv_WriteData(color >> 8);
	SPIv_WriteData(color);
#endif
	LCD_CS_SET;
}

void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
	LCD_SetCursor(x, y); 
	LCD_CS_CLR;
	LCD_RS_SET;
#if USE_HARDWARE_SPI
	SPI_WriteByte(SPI2, color >> 8);
	SPI_WriteByte(SPI2, color);
#else
	SPIv_WriteData(POINT_COLOR >> 8);
	SPIv_WriteData(POINT_COLOR);
#endif
	LCD_CS_SET;
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}

//******************************************************************
//��������  LCD_GPIOInit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Һ����IO��ʼ����Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_GPIOInit(void)
{
	//	GPIO_InitTypeDef  GPIO_InitStructure;
	//
	//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	//
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);

	LCD_RST_SET;
	delay_ms(50);
}

//******************************************************************
//��������  LCD_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{

#if USE_HARDWARE_SPI //ʹ��Ӳ��SPI
	SPI2_Init();
#else
	LCD_GPIO_Init(); //ʹ��ģ��SPI
#endif
	LCD_RESET(); //Reset before LCD Init.

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC1);
	LCD_WR_DATA(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x7A);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC0);  //Power control
	LCD_WR_DATA(0x1B); //VRH[5:0]
	LCD_WR_REG(0xC1);  //Power control
	LCD_WR_DATA(0x01); //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5);  //VCM control
	LCD_WR_DATA(0x30); //3F
	LCD_WR_DATA(0x30); //3C
	LCD_WR_REG(0xC7);  //VCM control2
	LCD_WR_DATA(0XB7);
	LCD_WR_REG(0x36); // Memory Access Control
	LCD_WR_DATA(0x48);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1A);
	LCD_WR_REG(0xB6); // Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);
	LCD_WR_REG(0xF2); // 3Gamma Function Disable
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x26); //Gamma curve selected
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0XA9);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on

	LCD_SetParam(); //����LCD����
	TP_Init();
	LCD_BL_SET; //�ߵ�ƽ��Һ��������
}
//******************************************************************
//��������  LCD_Clear
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index = 0;
	LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);
	LCD_CS_CLR;
	LCD_RS_SET; //д����
	for (index = 0; index < lcddev.width * lcddev.height; index++)
	{
#if USE_HARDWARE_SPI
		SPI_WriteByte(SPI2, Color >> 8);
		SPI_WriteByte(SPI2, Color);
#else
		SPIv_WriteData(SPI2, Color >> 8);
		SPIv_WriteData(SPI2, Color);
#endif
	}
	LCD_CS_SET;
}
void LCD_Fill(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
	u32 index = 0;
	LCD_SetWindows(x0, y0, x1, y1);
	LCD_CS_CLR;
	LCD_RS_SET; //д����
	for (index = 0; index < (x1 - x0) * (y1 - y0); index++)
	{
#if USE_HARDWARE_SPI
		SPI_WriteByte(SPI2, Color >> 8);
		SPI_WriteByte(SPI2, Color);
#else
		SPIv_WriteData(SPI2, Color >> 8);
		SPIv_WriteData(SPI2, Color);
#endif
	}
	LCD_CS_SET;
}
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(xStar >> 8);
	LCD_WR_DATA(0x00FF & xStar);
	LCD_WR_DATA(xEnd >> 8);
	LCD_WR_DATA(0x00FF & xEnd);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(yStar >> 8);
	LCD_WR_DATA(0x00FF & yStar);
	LCD_WR_DATA(yEnd >> 8);
	LCD_WR_DATA(0x00FF & yEnd);

	LCD_WriteRAM_Prepare(); //��ʼд��GRAM
}

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(Xpos >> 8);
	LCD_WR_DATA(0x00FF & Xpos);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(Ypos >> 8);
	LCD_WR_DATA(0x00FF & Ypos);

	LCD_WriteRAM_Prepare(); //��ʼд��GRAM
}

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{
	lcddev.wramcmd = 0x2C;
#if USE_HORIZONTAL == 0 //ʹ�ú���
	lcddev.dir = 1;		//����
	lcddev.width = 320;
	lcddev.height = 240;
	lcddev.setxcmd = 0x2A;
	lcddev.setycmd = 0x2B;
	LCD_WriteReg(0x36, 0x6C);

#else //����
	lcddev.dir = 0; //����
	lcddev.width = 240;
	lcddev.height = 320;
	lcddev.setxcmd = 0x2A;
	lcddev.setycmd = 0x2B;
	LCD_WriteReg(0x36, 0xC9);
#endif
}
