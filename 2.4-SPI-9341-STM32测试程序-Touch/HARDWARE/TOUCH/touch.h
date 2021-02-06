#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567
//�ֻ�:15989313508���빤��
//����:QDtech2008@gmail.com
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//=========================================������������=========================================//
/***************************************************************************************************
//��ʹ�ô�������ģ�鱾��������������ɲ�����
//MO(MISO)	��PC2	//SPI�������
//MI(MOSI)	��PC3	//SPI��������
//PEN		��PC1	//�������ж��ź�
//TCS		��PC13	//����ICƬѡ
//CLK		��PC0	//SPI����ʱ��
**************************************************************************************************/

#define TP_PRES_DOWN 0x80 //����������
#define TP_CATH_PRES 0x40 //�а���������

//������������
typedef struct
{
	u8 (*init)(void);	  //��ʼ��������������
	u8 (*scan)(u8);		  //ɨ�败����.0,��Ļɨ��;1,��������;
	void (*adjust)(void); //������У׼
	u16 x0;				  //ԭʼ����(��һ�ΰ���ʱ������)
	u16 y0;
	u16 x; //��ǰ����(�˴�ɨ��ʱ,����������)
	u16 y;
	u8 sta; //�ʵ�״̬
			//b7:����1/�ɿ�0;
			//b6:0,û�а�������;1,�а�������.
			////////////////////////������У׼����/////////////////////////
	float xfac;
	float yfac;
	short xoff;
	short yoff;
	//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
	//touchtype=0��ʱ��,�ʺ�����ΪX����,����ΪY�����TP.
	//touchtype=1��ʱ��,�ʺ�����ΪY����,����ΪX�����TP.
	u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev; //������������touch.c���涨��

//�봥����оƬ��������
//�봥����оƬ��������
//#define PEN  PCin(2)   //PC1  INT
//#define DOUT PCin(11)   //PC2  MISO
//#define TDIN PCout(12)  //PC3  MOSI
//#define TCLK PCout(10)  //PC0  SCLK
//#define TCS  PCout(4) //PC13 CS
#define Touch_Port GPIOC																		   //����TFT���ݶ˿�
#define PEN (Touch_Port->IDR & (1 << 2))														   //PC2  INT
#define DOUT (Touch_Port->IDR & (1 << 11))														   //PC11  MISO
#define TDIN(x) Touch_Port->ODR = x ? Touch_Port->ODR | (1 << 12) : Touch_Port->ODR & (~(1 << 12)) //PC12  MOSI
#define TCLK(x) Touch_Port->ODR = x ? Touch_Port->ODR | (1 << 10) : Touch_Port->ODR & (~(1 << 10)) //PC10  SCLK
#define TCS(x) Touch_Port->ODR = x ? Touch_Port->ODR | (1 << 4) : Touch_Port->ODR & (~(1 << 4))	   //PC4 CS    //PC3 MOSI--->>TFT --SDA/DIN

void TP_Write_Byte(u8 num);						   //�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							   //��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							   //���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x, u16 *y);					   //˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x, u16 *y);					   //����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color); //��һ������У׼��
void TP_Draw_Big_Point(u16 x, u16 y, u16 color);   //��һ�����
u8 TP_Scan(u8 tp);								   //ɨ��
void TP_Save_Adjdata(void);						   //����У׼����
u8 TP_Get_Adjdata(void);						   //��ȡУ׼����
void TP_Adjust(void);							   //������У׼
u8 TP_Init(void);								   //��ʼ��

void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac); //��ʾУ׼��Ϣ

#endif
