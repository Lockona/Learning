#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "stdlib.h"

/****************************************************************************************************
//=======================================Һ���������߽���==========================================//
STM32 PB���Һ����DB0~DB16,��������ΪDB0��PB0,..DB15��PB15.   
//=======================================Һ���������߽���==========================================//
//LCD_CS	��PC9	//Ƭѡ�ź�
//LCD_RS	��PC8	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PC7	//д�ź�
//LCD_RD	��PC6	//���ź�
//LCD_RST	��PC5	//��λ�ź�
//LCD_LED	��PC10	//��������ź�(�ߵ�ƽ����)
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾��������������ɲ�����
//MO(MISO)	��PC2	//SPI�������
//MI(MOSI)	��PC3	//SPI��������
//PEN		��PC1	//�������ж��ź�
//TCS		��PC13	//����ICƬѡ
//CLK		��PC0	//SPI����ʱ��
**************************************************************************************************/

/////////////////////////////////////�û�������///////////////////////////////////
#define USE_HORIZONTAL 1   //�����Ƿ�ʹ�ú��� 		0,��ʹ��; 1,ʹ��.
#define USE_HARDWARE_SPI 1 //����ʹ��Ӳ��SPI����ģ��SPI 0,��ʹ��; 1,ʹ��
//////////////////////////////////////////////////////////////////////////////////

//LCD��Ҫ������
typedef struct
{
	u16 width;	 //LCD ����
	u16 height;	 //LCD �߶�
	u16 id;		 //LCD ID
	u8 dir;		 //���������������ƣ�0��������1��������
	u16 wramcmd; //��ʼдgramָ��
	u16 setxcmd; //����x����ָ��
	u16 setycmd; //����y����ָ��
} _lcd_dev;

//LCD����
extern _lcd_dev lcddev; //����LCD��Ҫ����
//����LCD�ĳߴ�
#if USE_HORIZONTAL == 1 //ʹ�ú���
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif

//TFTLCD������Ҫ���õĺ���
extern u16 POINT_COLOR; //Ĭ�Ϻ�ɫ
extern u16 BACK_COLOR;	//������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD���߶���----------------

#define LCD_CTRL_B GPIOB	//����TFT���ݶ˿�
#define LCD_CTRL_C GPIOC	//����TFT���ݶ˿�
#define LCD_BL GPIO_Pin_11	//MCU_PB11--->>TFT --BL
#define LCD_RS GPIO_Pin_12	//PB12--->>TFT --RS/DC
#define LCD_CS GPIO_Pin_13	//MCU_PB13--->>TFT --CS/CE
#define LCD_RST GPIO_Pin_14 //PB14--->>TFT --RST
#define LCD_SCL GPIO_Pin_10 //PB10--->>TFT --SCL/SCK
#define LCD_SDA GPIO_Pin_3	//PC3 MOSI--->>TFT --SDA/DIN

//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//Һ�����ƿ���1�������궨��
#define LCD_CS_SET LCD_CTRL_B->BSRRL = LCD_CS
#define LCD_RS_SET LCD_CTRL_B->BSRRL = LCD_RS
#define LCD_SDA_SET LCD_CTRL_C->BSRRL = LCD_SDA
#define LCD_SCL_SET LCD_CTRL_B->BSRRL = LCD_SCL
#define LCD_RST_SET LCD_CTRL_B->BSRRL = LCD_RST
#define LCD_BL_SET LCD_CTRL_B->BSRRL = LCD_BL

//Һ�����ƿ���0�������궨��
#define LCD_CS_CLR LCD_CTRL_B->BSRRH = LCD_CS
#define LCD_RS_CLR LCD_CTRL_B->BSRRH = LCD_RS
#define LCD_SDA_CLR LCD_CTRL_C->BSRRH = LCD_SDA
#define LCD_SCL_CLR LCD_CTRL_B->BSRRH = LCD_SCL
#define LCD_RST_CLR LCD_CTRL_B->BSRRH = LCD_RST
#define LCD_BL_CLR LCD_CTRL_B->BSRRH = LCD_BL
//////////////////////////////////////////////////////////////////////

//ɨ�跽����
#define L2R_U2D 0 //������,���ϵ���
#define L2R_D2U 1 //������,���µ���
#define R2L_U2D 2 //���ҵ���,���ϵ���
#define R2L_D2U 3 //���ҵ���,���µ���

#define U2D_L2R 4 //���ϵ���,������
#define U2D_R2L 5 //���ϵ���,���ҵ���
#define D2U_L2R 6 //���µ���,������
#define D2U_R2L 7 //���µ���,���ҵ���

#define DFT_SCAN_DIR L2R_U2D //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //��ɫ
#define BRRED 0XFC07 //�غ�ɫ
#define GRAY 0X8430	 //��ɫ
//GUI��ɫ

#define DARKBLUE 0X01CF	 //����ɫ
#define LIGHTBLUE 0X7D7C //ǳ��ɫ
#define GRAYBLUE 0X5458	 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN 0X841F //ǳ��ɫ
//#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE 0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE 0X2B12	 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

extern u16 BACK_COLOR, POINT_COLOR;

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x, u16 y); //����
u16 LCD_ReadPoint(u16 x, u16 y);  //����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd);
void LCD_DrawPoint_16Bit(u16 color);
u16 LCD_RD_DATA(void); //��ȡLCD����
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
void SPIv_WriteData(u8 Data);
u8 SPI_WriteByte(SPI_TypeDef *SPIx, u8 Byte);

//�����Ȼ�����ٶȲ����죬����ʹ������ĺ궨��,����ٶ�.
//ע��Ҫȥ��lcd.c��void LCD_WR_DATA(u16 data)��������Ŷ
/*
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//ʹ��16λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/

//9320/9325 LCD�Ĵ���
#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R4 0x04
#define R5 0x05
#define R6 0x06
#define R7 0x07
#define R8 0x08
#define R9 0x09
#define R10 0x0A
#define R12 0x0C
#define R13 0x0D
#define R14 0x0E
#define R15 0x0F
#define R16 0x10
#define R17 0x11
#define R18 0x12
#define R19 0x13
#define R20 0x14
#define R21 0x15
#define R22 0x16
#define R23 0x17
#define R24 0x18
#define R25 0x19
#define R26 0x1A
#define R27 0x1B
#define R28 0x1C
#define R29 0x1D
#define R30 0x1E
#define R31 0x1F
#define R32 0x20
#define R33 0x21
#define R34 0x22
#define R36 0x24
#define R37 0x25
#define R40 0x28
#define R41 0x29
#define R43 0x2B
#define R45 0x2D
#define R48 0x30
#define R49 0x31
#define R50 0x32
#define R51 0x33
#define R52 0x34
#define R53 0x35
#define R54 0x36
#define R55 0x37
#define R56 0x38
#define R57 0x39
#define R59 0x3B
#define R60 0x3C
#define R61 0x3D
#define R62 0x3E
#define R63 0x3F
#define R64 0x40
#define R65 0x41
#define R66 0x42
#define R67 0x43
#define R68 0x44
#define R69 0x45
#define R70 0x46
#define R71 0x47
#define R72 0x48
#define R73 0x49
#define R74 0x4A
#define R75 0x4B
#define R76 0x4C
#define R77 0x4D
#define R78 0x4E
#define R79 0x4F
#define R80 0x50
#define R81 0x51
#define R82 0x52
#define R83 0x53
#define R96 0x60
#define R97 0x61
#define R106 0x6A
#define R118 0x76
#define R128 0x80
#define R129 0x81
#define R130 0x82
#define R131 0x83
#define R132 0x84
#define R133 0x85
#define R134 0x86
#define R135 0x87
#define R136 0x88
#define R137 0x89
#define R139 0x8B
#define R140 0x8C
#define R141 0x8D
#define R143 0x8F
#define R144 0x90
#define R145 0x91
#define R146 0x92
#define R147 0x93
#define R148 0x94
#define R149 0x95
#define R150 0x96
#define R151 0x97
#define R152 0x98
#define R153 0x99
#define R154 0x9A
#define R157 0x9D
#define R192 0xC0
#define R193 0xC1
#define R229 0xE5
#endif
