#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
//#include "key.h" 
//#include "led.h"
#include "pic.h"
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
//========================variable==========================//
u16 ColorTab[5]={BRED,YELLOW,RED,GREEN,BLUE};//������ɫ����
//=====================end of variable======================//

//******************************************************************
//��������  DrawTestPage
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ���Ʋ��Խ���
//���������str :�ַ���ָ��
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD_Fill(0,0,lcddev.width,20,BLUE);
//���ƹ̶���down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"QDtech��Ȩ����",16,1);//������ʾ
//���Ʋ�������
LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
}

//******************************************************************
//��������  main_test
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ����ȫ�������ۺϲ��Գ���������
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void main_test(void)
{
	DrawTestPage("ȫ�������ۺϲ��Գ���");
	
	Gui_StrCenter(0,30,RED,BLUE,"ȫ������",16,1);//������ʾ
	Gui_StrCenter(0,60,RED,BLUE,"�ۺϲ��Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,90,YELLOW,BLUE,"2.4' ILI9341 240X320",16,1);//������ʾ
	Gui_StrCenter(0,120,BLUE,BLUE,"xiaoFeng@QDtech 2014-02-25",16,1);//������ʾ
	delay_ms(1500);		
	delay_ms(1500);
}

//******************************************************************
//��������  Test_Color
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ɫ�����ԣ���������ɫ����ɫ����ɫ����ɫ����ɫ
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Test_Color(void)  
{
	//DrawTestPage("����1:��ɫ������");
	LCD_Fill(0,0,lcddev.width,lcddev.height-0,WHITE);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"White",16,1);//delay_ms(500);
	LCD_Fill(0,0,lcddev.width,lcddev.height-0,BLACK);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Black",16,1);//delay_ms(500);
	LCD_Fill(0,0,lcddev.width,lcddev.height-0,RED);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Red",16,1); //delay_ms(500);
	LCD_Fill(0,0,lcddev.width,lcddev.height-0,GREEN);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Green",16,1);//delay_ms(500);
	LCD_Fill(0,0,lcddev.width,lcddev.height-0,BLUE);
	Show_Str(lcddev.width-50,30,WHITE,YELLOW,"Blue",16,1);//delay_ms(500);

}

//******************************************************************
//��������  Test_FillRec
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ���ο���ʾ�������ԣ�������ʾ�ۺ�ɫ����ɫ����ɫ����ɫ����ɫ���ο�
//       	��ʱ1500��������ΰ��շۺ�ɫ����ɫ����ɫ����ɫ����ɫ�����ο� 
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("����2:GUI����������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		LCD_DrawRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);
}

//******************************************************************
//��������  Test_FillRec
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Բ�ο���ʾ�������ԣ�������ʾ�ۺ�ɫ����ɫ����ɫ����ɫ����ɫԲ�ο�
//       	��ʱ1500��������ΰ��շۺ�ɫ����ɫ����ɫ����ɫ����ɫ���Բ�ο� 
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("����3:GUI��Բ������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,1);
	delay_ms(1500);
}

//******************************************************************
//��������  English_Font_test
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Ӣ����ʾ���� 
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void English_Font_test(void)
{
	DrawTestPage("����4:Ӣ����ʾ����");
	POINT_COLOR=RED;
	BACK_COLOR=BLUE;
	LCD_ShowString(10,30,12,"6X12:abcdefghijklmnopqrstuvwxyz0123456789",0);
	LCD_ShowString(10,45,12,"6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",1);
	LCD_ShowString(10,60,12,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",0);
	LCD_ShowString(10,80,16,"8X16:abcdefghijklmnopqrstuvwxyz0123456789",0);
	LCD_ShowString(10,100,16,"8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",1);
	LCD_ShowString(10,120,16,"8X16:~!@#$%^&*()_+{}:<>?/|-+.",0); 
	delay_ms(1200);
}

//******************************************************************
//��������  Chinese_Font_test
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ������ʾ����
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Chinese_Font_test(void)
{	
//	DrawTestPage("����5:������ʾ����");
//	Show_Str(10,30,BLUE,YELLOW,"16X16:ȫ�����Ӽ������޹�˾��ӭ��",16,0);
//	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcomeȫ������",16,1);
//	Show_Str(10,70,BLUE,YELLOW,"24X24:���������Ĳ���",24,1);
//	Show_Str(10,100,BLUE,YELLOW,"32X32:�������",32,1);
	delay_ms(120);
}

//******************************************************************
//��������  Pic_test
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ͼƬ��ʾ���ԣ�������ʾ����40X40 QQͼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Pic_test(void)
{
	DrawTestPage("����6:ͼƬ��ʾ����");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Gui_Drawbmp16(30,30,gImage_qq);
	Show_Str(30+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(90,30,gImage_qq);
	Show_Str(90+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(150,30,gImage_qq);
	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	delay_ms(1200);
}

//******************************************************************
//��������  Touch_Test
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ������д����
//�����������
//����ֵ��  ��
//�޸ļ�¼��
//******************************************************************
void Touch_Test(void)
{
//u8 key;
	u8 i=0;
	u16 j=0;
	u16 colorTemp=0;
	TP_Init();
//	KEY_Init();
	TP_Adjust();  		//ǿ��ִ��һ����ĻУ׼ (������û��IIC�洢�����������û�)
	DrawTestPage("����7:Touch����");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
	POINT_COLOR=RED;
		while(1)
	{
//	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
			{	
				if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)
				{
					DrawTestPage("����7:Touch����");//���
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
				}
				else if((tp_dev.x>(lcddev.width-60)&&tp_dev.x<(lcddev.width-50+20))&&tp_dev.y<20)
				{
				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				else TP_Draw_Big_Point(tp_dev.x,tp_dev.y,POINT_COLOR);		//��ͼ	  			   
			}
		}else delay_ms(10);	//û�а������µ�ʱ�� 	    
//		if(key==1)	//KEY_RIGHT����,��ִ��У׼����
//		{

//			LCD_Clear(WHITE);//����
//		    TP_Adjust();  //��ĻУ׼ 
//			TP_Save_Adjdata();	 
//			DrawTestPage("����7:Touch����");
//		}
		i++;
		if(i==20)
		{
			i=0;
			j++;
			//LED0=!LED0;
		}/*
		if(j>100)
		{
				j=0;
			break;
		}*/
	}   
}




