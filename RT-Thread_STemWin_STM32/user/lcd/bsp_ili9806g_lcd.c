/**
  ******************************************************************************
  * @file    bsp_ili9806g_lcd.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ILI9806GҺ��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./lcd/bsp_ili9806g_lcd.h"

//����Һ��ɨ�跽����仯��XY���ؿ���
//����ILI9806G_GramScan�������÷���ʱ���Զ�����
uint16_t LCD_X_LENGTH = ILI9806G_MORE_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9806G_LESS_PIXEL;

//Һ����ɨ��ģʽ����������Ҫ���ڷ���ѡ�������ļ������
//������ѡֵΪ0-7
//����ILI9806G_GramScan�������÷���ʱ���Զ�����
//LCD�ճ�ʼ�����ʱ��ʹ�ñ�Ĭ��ֵ
uint8_t LCD_SCAN_MODE =6;

static uint16_t CurrentBackColor   = BLACK;//����ɫ


__inline void                 ILI9806G_Write_Cmd           ( uint16_t usCmd );
__inline void                 ILI9806G_Write_Data          ( uint16_t usData );
__inline uint16_t             ILI9806G_Read_Data           ( void );
static void                   ILI9806G_Delay               ( __IO uint32_t nCount );
static void                   ILI9806G_GPIO_Config         ( void );
static void                   ILI9806G_FSMC_Config         ( void );
static void                   ILI9806G_REG_Config          ( void );
static __inline void          ILI9806G_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );

/**
  * @brief  ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

/**
  * @brief  ��ILI9806Gд������
  * @param  usCmd :Ҫд���������Ĵ�����ַ��
  * @retval ��
  */	
__inline void ILI9806G_Write_Cmd ( uint16_t usCmd )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9806G_CMD ) = usCmd;
	
}


/**
  * @brief  ��ILI9806Gд������
  * @param  usData :Ҫд�������
  * @retval ��
  */	
__inline void ILI9806G_Write_Data ( uint16_t usData )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9806G_DATA ) = usData;
	
}


/**
  * @brief  ��ILI9806G��ȡ����
  * @param  ��
  * @retval ��ȡ��������
  */	
__inline uint16_t ILI9806G_Read_Data ( void )
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_ILI9806G_DATA ) );
	
}


/**
  * @brief  ���� ILI9806G ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */	
static void ILI9806G_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}


/**
  * @brief  ��ʼ��ILI9806G��IO����
  * @param  ��
  * @retval ��
  */
static void ILI9806G_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
	RCC_AHB1PeriphClockCmd ( 	
													/*�����ź�*/
													ILI9806G_CS_CLK|ILI9806G_DC_CLK|ILI9806G_WR_CLK|
													ILI9806G_RD_CLK	|ILI9806G_BK_CLK|ILI9806G_RST_CLK|
													/*�����ź�*/
													ILI9806G_D0_CLK|ILI9806G_D1_CLK|	ILI9806G_D2_CLK | 
													ILI9806G_D3_CLK | ILI9806G_D4_CLK|ILI9806G_D5_CLK|
													ILI9806G_D6_CLK | ILI9806G_D7_CLK|ILI9806G_D8_CLK|
													ILI9806G_D9_CLK | ILI9806G_D10_CLK|ILI9806G_D11_CLK|
													ILI9806G_D12_CLK | ILI9806G_D13_CLK|ILI9806G_D14_CLK|
													ILI9806G_D15_CLK	, ENABLE );
		
	
	/* ����FSMC���Ӧ��������,FSMC-D0~D15 */	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D0_PIN; 
    GPIO_Init(ILI9806G_D0_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D0_PORT,ILI9806G_D0_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D1_PIN; 
    GPIO_Init(ILI9806G_D1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D1_PORT,ILI9806G_D1_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D2_PIN; 
    GPIO_Init(ILI9806G_D2_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D2_PORT,ILI9806G_D2_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D3_PIN; 
    GPIO_Init(ILI9806G_D3_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D3_PORT,ILI9806G_D3_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D4_PIN; 
    GPIO_Init(ILI9806G_D4_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D4_PORT,ILI9806G_D4_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D5_PIN; 
    GPIO_Init(ILI9806G_D5_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D5_PORT,ILI9806G_D5_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D6_PIN; 
    GPIO_Init(ILI9806G_D6_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D6_PORT,ILI9806G_D6_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D7_PIN; 
    GPIO_Init(ILI9806G_D7_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D7_PORT,ILI9806G_D7_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D8_PIN; 
    GPIO_Init(ILI9806G_D8_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D8_PORT,ILI9806G_D8_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D9_PIN; 
    GPIO_Init(ILI9806G_D9_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D9_PORT,ILI9806G_D9_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D10_PIN; 
    GPIO_Init(ILI9806G_D10_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D10_PORT,ILI9806G_D10_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D11_PIN; 
    GPIO_Init(ILI9806G_D11_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D11_PORT,ILI9806G_D11_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D12_PIN; 
    GPIO_Init(ILI9806G_D12_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D12_PORT,ILI9806G_D12_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D13_PIN; 
    GPIO_Init(ILI9806G_D13_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D13_PORT,ILI9806G_D13_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D14_PIN; 
    GPIO_Init(ILI9806G_D14_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D14_PORT,ILI9806G_D14_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_D15_PIN; 
    GPIO_Init(ILI9806G_D15_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_D15_PORT,ILI9806G_D15_PinSource,FSMC_AF);

	/* ����FSMC���Ӧ�Ŀ�����
	 * FSMC_NOE   :LCD-RD
	 * FSMC_NWE   :LCD-WR
	 * FSMC_NE1   :LCD-CS
	 * FSMC_A0    :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = ILI9806G_RD_PIN; 
    GPIO_Init(ILI9806G_RD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_RD_PORT,ILI9806G_RD_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_WR_PIN; 
    GPIO_Init(ILI9806G_WR_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_WR_PORT,ILI9806G_WR_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9806G_CS_PIN; 
    GPIO_Init(ILI9806G_CS_PORT, &GPIO_InitStructure);   
    GPIO_PinAFConfig(ILI9806G_CS_PORT,ILI9806G_CS_PinSource,FSMC_AF);  

    GPIO_InitStructure.GPIO_Pin = ILI9806G_DC_PIN; 
    GPIO_Init(ILI9806G_DC_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9806G_DC_PORT,ILI9806G_DC_PinSource,FSMC_AF);
	

  /* ����LCD��λRST���ƹܽ�*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ILI9806G_RST_PIN; 
	GPIO_Init ( ILI9806G_RST_PORT, & GPIO_InitStructure );
		
	/* ����LCD������ƹܽ�BK*/
	GPIO_InitStructure.GPIO_Pin = ILI9806G_BK_PIN; 
	GPIO_Init ( ILI9806G_BK_PORT, & GPIO_InitStructure );

}


 /**
  * @brief  LCD  FSMC ģʽ����
  * @param  ��
  * @retval ��
  */
static void ILI9806G_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 	
	
	/* ʹ��FSMCʱ��*/
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);

	//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 5/168M=30ns
	readWriteTiming.FSMC_AddressSetupTime      = 0x04;	 //��ַ����ʱ��
	//���ݱ���ʱ�䣨DATAST��+ 1��HCLK = 12/168M=72ns	
	readWriteTiming.FSMC_DataSetupTime         = 0x0b;	 //���ݽ���ʱ��
	//ѡ����Ƶ�ģʽ
	//ģʽB,�첽NOR FLASHģʽ����ILI9806G��8080ʱ��ƥ��
	readWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_B;	
	
	/*����������ģʽB�޹�*/
	//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�
	readWriteTiming.FSMC_AddressHoldTime       = 0x00;	 //��ַ����ʱ��
	//��������ת�����ڣ������ڸ���ģʽ��NOR����
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//����ʱ�ӷ�Ƶ��������ͬ�����͵Ĵ洢��
	readWriteTiming.FSMC_CLKDivision           = 0x00;
	//���ݱ���ʱ�䣬������ͬ���͵�NOR	
	readWriteTiming.FSMC_DataLatency           = 0x00;	

	
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &readWriteTiming;  
	
	FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
	
	
	/* ʹ�� FSMC_Bank1_NORSRAM3 */
	FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
		
		
}


/**
 * @brief  ��ʼ��ILI9806G�Ĵ���
 * @param  ��
 * @retval ��
 */
/**
 * @brief  ��ʼ��ILI9806G�Ĵ���
 * @param  ��
 * @retval ��
 */
static void ILI9806G_REG_Config ( void )
{	
	//Һ�������ṩ�����ְ汾����Ļ������һ�������ǵ�����������������
	//������ͨ��#if #else #endif�����ã�����Ļ��ʾ���������#if 0�ĳ�#if 1����1�ĳ�0
	//��2017-12-14�պ���Ĳ�Ʒ��ʹ��#if 0
#if 0
	//�ɰ�
	/* EXTC Command Set enable register */
	DEBUG_DELAY  ();
	ILI9806G_Write_Cmd ( 0xFF  );
	ILI9806G_Write_Data ( 0xFF  );
	ILI9806G_Write_Data ( 0x98  );
	ILI9806G_Write_Data ( 0x06  );

	/* GIP 1(BCh)  */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd(0xBC);
	ILI9806G_Write_Data(0x01); 
	ILI9806G_Write_Data(0x0E); 
	ILI9806G_Write_Data(0x61); 
	ILI9806G_Write_Data(0xFB); 
	ILI9806G_Write_Data(0x10); 
	ILI9806G_Write_Data(0x10); 
	ILI9806G_Write_Data(0x0B); 
	ILI9806G_Write_Data(0x0F); 
	ILI9806G_Write_Data(0x2E); 
	ILI9806G_Write_Data(0x73); 
	ILI9806G_Write_Data(0xFF); 
	ILI9806G_Write_Data(0xFF); 
	ILI9806G_Write_Data(0x0E); 
	ILI9806G_Write_Data(0x0E); 
	ILI9806G_Write_Data(0x00); 
	ILI9806G_Write_Data(0x03); 
	ILI9806G_Write_Data(0x66); 
	ILI9806G_Write_Data(0x63); 
	ILI9806G_Write_Data(0x01); 
	ILI9806G_Write_Data(0x00); 
	ILI9806G_Write_Data(0x00);

	/* GIP 2 (BDh) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd(0xBD);
	ILI9806G_Write_Data(0x01); 
	ILI9806G_Write_Data(0x23); 
	ILI9806G_Write_Data(0x45); 
	ILI9806G_Write_Data(0x67); 
	ILI9806G_Write_Data(0x01); 
	ILI9806G_Write_Data(0x23); 
	ILI9806G_Write_Data(0x45); 
	ILI9806G_Write_Data(0x67); 

	/* GIP 3 (BEh) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd(0xBE);
	ILI9806G_Write_Data(0x00); 
	ILI9806G_Write_Data(0x21); 
	ILI9806G_Write_Data(0xAB); 
	ILI9806G_Write_Data(0x60); 
	ILI9806G_Write_Data(0x22); 
	ILI9806G_Write_Data(0x22); 
	ILI9806G_Write_Data(0x22); 
	ILI9806G_Write_Data(0x22); 
	ILI9806G_Write_Data(0x22); 

	/* Vcom  (C7h) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xC7 );
	ILI9806G_Write_Data ( 0x6F );

	/* EN_volt_reg (EDh)*/
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xED );
	ILI9806G_Write_Data ( 0x7F );
	ILI9806G_Write_Data ( 0x0F );
	ILI9806G_Write_Data ( 0x00 );

	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xC0 );
	ILI9806G_Write_Data ( 0x37 );
	ILI9806G_Write_Data ( 0x0B );
	ILI9806G_Write_Data ( 0x0A );

	/* LVGL (FCh) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xFC );
	ILI9806G_Write_Data ( 0x0A );

	/* Engineering Setting (DFh) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xDF );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x20 );

	/* DVDD Voltage Setting(F3h) */
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0xF3 );
	ILI9806G_Write_Data ( 0x74 );

	/* Display Inversion Control (B4h) */
	ILI9806G_Write_Cmd ( 0xB4 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x00 );

	/* 480x854 (F7h)  */
	ILI9806G_Write_Cmd ( 0xF7 );
	ILI9806G_Write_Data ( 0x89 );

	/* Frame Rate (B1h) */
	ILI9806G_Write_Cmd ( 0xB1 );
	ILI9806G_Write_Data ( 0x00 );
	ILI9806G_Write_Data ( 0x12 );
	ILI9806G_Write_Data ( 0x10 );

	/* Panel Timing Control (F2h) */
	ILI9806G_Write_Cmd ( 0xF2 );
	ILI9806G_Write_Data ( 0x80 );
	ILI9806G_Write_Data ( 0x5B );
	ILI9806G_Write_Data ( 0x40 );
	ILI9806G_Write_Data ( 0x28 );

	DEBUG_DELAY ();

	/* Power Control 2 (C1h) */
	ILI9806G_Write_Cmd ( 0xC1 ); 
	ILI9806G_Write_Data ( 0x17 );
	ILI9806G_Write_Data ( 0x7D );
	ILI9806G_Write_Data ( 0x7A );
	ILI9806G_Write_Data ( 0x20 );

	DEBUG_DELAY ();

	ILI9806G_Write_Cmd(0xE0); 
	ILI9806G_Write_Data(0x00); //P1 
	ILI9806G_Write_Data(0x11); //P2 
	ILI9806G_Write_Data(0x1C); //P3 
	ILI9806G_Write_Data(0x0E); //P4 
	ILI9806G_Write_Data(0x0F); //P5 
	ILI9806G_Write_Data(0x0C); //P6 
	ILI9806G_Write_Data(0xC7); //P7 
	ILI9806G_Write_Data(0x06); //P8 
	ILI9806G_Write_Data(0x06); //P9 
	ILI9806G_Write_Data(0x0A); //P10 
	ILI9806G_Write_Data(0x10); //P11 
	ILI9806G_Write_Data(0x12); //P12 
	ILI9806G_Write_Data(0x0A); //P13 
	ILI9806G_Write_Data(0x10); //P14 
	ILI9806G_Write_Data(0x02); //P15 
	ILI9806G_Write_Data(0x00); //P16 

	DEBUG_DELAY ();

	ILI9806G_Write_Cmd(0xE1); 
	ILI9806G_Write_Data(0x00); //P1 
	ILI9806G_Write_Data(0x12); //P2 
	ILI9806G_Write_Data(0x18); //P3 
	ILI9806G_Write_Data(0x0C); //P4 
	ILI9806G_Write_Data(0x0F); //P5 
	ILI9806G_Write_Data(0x0A); //P6 
	ILI9806G_Write_Data(0x77); //P7 
	ILI9806G_Write_Data(0x06); //P8 
	ILI9806G_Write_Data(0x07); //P9 
	ILI9806G_Write_Data(0x0A); //P10 
	ILI9806G_Write_Data(0x0E); //P11 
	ILI9806G_Write_Data(0x0B); //P12 
	ILI9806G_Write_Data(0x10); //P13 
	ILI9806G_Write_Data(0x1D); //P14 
	ILI9806G_Write_Data(0x17); //P15 
	ILI9806G_Write_Data(0x00); //P16  

	/* Tearing Effect ON (35h)  */
	ILI9806G_Write_Cmd ( 0x35 );
	ILI9806G_Write_Data ( 0x00 );

	ILI9806G_Write_Cmd ( 0x3A );
	ILI9806G_Write_Data ( 0x55 );

	ILI9806G_Write_Cmd ( 0x11 );
	DEBUG_DELAY ();
	ILI9806G_Write_Cmd ( 0x29 );
	
#else
	//�°�
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xFF);
	ILI9806G_Write_Data(0xFF);
	ILI9806G_Write_Data(0x98);
	ILI9806G_Write_Data(0x06);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xBA);
	ILI9806G_Write_Data(0x60);  
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xBC);
	ILI9806G_Write_Data(0x03);
	ILI9806G_Write_Data(0x0E);
	ILI9806G_Write_Data(0x61);
	ILI9806G_Write_Data(0xFF);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x1B);
	ILI9806G_Write_Data(0x10);
	ILI9806G_Write_Data(0x73);
	ILI9806G_Write_Data(0x63);
	ILI9806G_Write_Data(0xFF);
	ILI9806G_Write_Data(0xFF);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0xD5);
	ILI9806G_Write_Data(0xD0);
	ILI9806G_Write_Data(0x01);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x40); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xBD);
	ILI9806G_Write_Data(0x01);
	ILI9806G_Write_Data(0x23);
	ILI9806G_Write_Data(0x45);
	ILI9806G_Write_Data(0x67);
	ILI9806G_Write_Data(0x01);
	ILI9806G_Write_Data(0x23);
	ILI9806G_Write_Data(0x45);
	ILI9806G_Write_Data(0x67);  
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xBE);
	ILI9806G_Write_Data(0x01);
	ILI9806G_Write_Data(0x2D);
	ILI9806G_Write_Data(0xCB);
	ILI9806G_Write_Data(0xA2);
	ILI9806G_Write_Data(0x62);
	ILI9806G_Write_Data(0xF2);
	ILI9806G_Write_Data(0xE2);
	ILI9806G_Write_Data(0x22);
	ILI9806G_Write_Data(0x22);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xC7);
	ILI9806G_Write_Data(0x63); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xED);
	ILI9806G_Write_Data(0x7F);
	ILI9806G_Write_Data(0x0F);
	ILI9806G_Write_Data(0x00);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xC0);
	ILI9806G_Write_Data(0x03);
	ILI9806G_Write_Data(0x0B);
	ILI9806G_Write_Data(0x00);   
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xFC);
	ILI9806G_Write_Data(0x08); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xDF);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x20);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xF3);
	ILI9806G_Write_Data(0x74);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xF9);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0xFD);
	ILI9806G_Write_Data(0x80);
	ILI9806G_Write_Data(0x80);
	ILI9806G_Write_Data(0xC0);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xB4);
	ILI9806G_Write_Data(0x02);
	ILI9806G_Write_Data(0x02);
	ILI9806G_Write_Data(0x02); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xF7);
	ILI9806G_Write_Data(0x81);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xB1);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x13);
	ILI9806G_Write_Data(0x13); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xF2);
	ILI9806G_Write_Data(0xC0);
	ILI9806G_Write_Data(0x02);
	ILI9806G_Write_Data(0x40);
	ILI9806G_Write_Data(0x28);  
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xC1);
	ILI9806G_Write_Data(0x17);
	ILI9806G_Write_Data(0x75);
	ILI9806G_Write_Data(0x75);
	ILI9806G_Write_Data(0x20); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xE0);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x08);
	ILI9806G_Write_Data(0x0C);
	ILI9806G_Write_Data(0x0F);
	ILI9806G_Write_Data(0x15);
	ILI9806G_Write_Data(0x09);
	ILI9806G_Write_Data(0x07);
	ILI9806G_Write_Data(0x01);
	ILI9806G_Write_Data(0x06);
	ILI9806G_Write_Data(0x09);
	ILI9806G_Write_Data(0x16);
	ILI9806G_Write_Data(0x14);
	ILI9806G_Write_Data(0x3E);
	ILI9806G_Write_Data(0x3E);
	ILI9806G_Write_Data(0x00);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0xE1);
	ILI9806G_Write_Data(0x00);
	ILI9806G_Write_Data(0x09);
	ILI9806G_Write_Data(0x12);
	ILI9806G_Write_Data(0x12);
	ILI9806G_Write_Data(0x13);
	ILI9806G_Write_Data(0x1c);
	ILI9806G_Write_Data(0x07);
	ILI9806G_Write_Data(0x08);
	ILI9806G_Write_Data(0x05);
	ILI9806G_Write_Data(0x08);
	ILI9806G_Write_Data(0x03);
	ILI9806G_Write_Data(0x02);
	ILI9806G_Write_Data(0x04);
	ILI9806G_Write_Data(0x1E);
	ILI9806G_Write_Data(0x1B);
	ILI9806G_Write_Data(0x00);
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0x3A);
	ILI9806G_Write_Data(0x55); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0x35);
	ILI9806G_Write_Data(0x00); 
	DEBUG_DELAY();
	ILI9806G_Write_Cmd(0x11);
	DEBUG_DELAY() ;
	ILI9806G_Write_Cmd(0x29);	   
	DEBUG_DELAY()  ; 
#endif	
}


/**
 * @brief  ILI9806G��ʼ�����������Ҫ�õ�lcd��һ��Ҫ�����������
 * @param  ��
 * @retval ��
 */
void ILI9806G_Init ( void )
{
	ILI9806G_GPIO_Config ();
	ILI9806G_FSMC_Config ();
	
	
	ILI9806G_Rst ();
	ILI9806G_REG_Config ();
	
	//����Ĭ��ɨ�跽������ 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
	ILI9806G_GramScan(LCD_SCAN_MODE);
    
	ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
	ILI9806G_BackLed_Control ( DISABLE );//��LCD�����
}

/**
 * @brief  ILI9806G����LED����
 * @param  enumState �������Ƿ�ʹ�ܱ���LED
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg ENABLE :ʹ�ܱ���LED
  *     @arg DISABLE :���ñ���LED
 * @retval ��
 */
void ILI9806G_BackLed_Control ( FunctionalState enumState )
{
	if ( enumState )
		 GPIO_SetBits( ILI9806G_BK_PORT, ILI9806G_BK_PIN );	
	else
		 GPIO_ResetBits( ILI9806G_BK_PORT, ILI9806G_BK_PIN );
		
}

/**
 * @brief  ILI9806G ������λ
 * @param  ��
 * @retval ��
 */
void ILI9806G_Rst ( void )
{			
	GPIO_ResetBits ( ILI9806G_RST_PORT, ILI9806G_RST_PIN );	 //�͵�ƽ��λ

	ILI9806G_Delay ( 0xAFF ); 					   

	GPIO_SetBits ( ILI9806G_RST_PORT, ILI9806G_RST_PIN );		 	 

	ILI9806G_Delay ( 0xAFF ); 	
	
}




/**
 * @brief  ����ILI9806G��GRAM��ɨ�跽�� 
 * @param  ucOption ��ѡ��GRAM��ɨ�跽�� 
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 *		
 *	����0��2��4��6 ģʽ��X��������Ϊ480��Y��������Ϊ854
 *	����1��3��5��7 ģʽ��X��������Ϊ854��Y��������Ϊ480
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD��ʾ��
								|-----------------|
								|			Ұ��Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								��Ļ���棨��480����854��

 *******************************************************/
void ILI9806G_GramScan ( uint8_t ucOption )
{	
	//������飬ֻ������0-7
	if(ucOption >7 )
		return;
	
	//����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
	LCD_SCAN_MODE = ucOption;
	
	//����ģʽ����XY��������ؿ���
	if(ucOption%2 == 0)	
	{
		//0 2 4 6ģʽ��X�������ؿ���Ϊ480��Y����Ϊ854
		LCD_X_LENGTH = ILI9806G_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9806G_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7ģʽ��X�������ؿ���Ϊ854��Y����Ϊ480
		LCD_X_LENGTH = ILI9806G_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9806G_LESS_PIXEL; 
	}

	//0x36��������ĸ�3λ����������GRAMɨ�跽��	
	ILI9806G_Write_Cmd ( 0x36 ); 
	ILI9806G_Write_Data (0x00 | (ucOption<<5));//����ucOption��ֵ����LCD��������0-7��ģʽ
	ILI9806G_Write_Cmd ( CMD_SetCoordinateX ); 
	ILI9806G_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	ILI9806G_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	ILI9806G_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ���������8λ */	
	ILI9806G_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x ���������8λ */

	ILI9806G_Write_Cmd ( CMD_SetCoordinateY ); 
	ILI9806G_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	ILI9806G_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	ILI9806G_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y ���������8λ */	 
	ILI9806G_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y ���������8λ */

	/* write gram start */
	ILI9806G_Write_Cmd ( CMD_SetPixel );	
}



/**
 * @brief  ��ILI9806G��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ���
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void ILI9806G_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{	
	ILI9806G_Write_Cmd ( CMD_SetCoordinateX ); 				 /* ����X���� */
	ILI9806G_Write_Data ( usX >> 8  );	 /* �ȸ�8λ��Ȼ���8λ */
	ILI9806G_Write_Data ( usX & 0xff  );	 /* ������ʼ��ͽ�����*/
	ILI9806G_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9806G_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9806G_Write_Cmd ( CMD_SetCoordinateY ); 			     /* ����Y����*/
	ILI9806G_Write_Data ( usY >> 8  );
	ILI9806G_Write_Data ( usY & 0xff  );
	ILI9806G_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9806G_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
}

/**
 * @brief  ��ILI9806G��ʾ������ĳһ��ɫ������ص�
 * @param  ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
 * @param  usColor ����ɫ
 * @retval ��
 */
static __inline void ILI9806G_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	ILI9806G_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9806G_Write_Data ( usColor );
		
	
}


/**
 * @brief  ��ILI9806G��ʾ����ĳһ������ĳ����ɫ��������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ���
 * @param  usHeight �����ڵĸ߶�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9806G_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
	ILI9806G_OpenWindow ( usX, usY, usWidth, usHeight );

	ILI9806G_FillColor ( usWidth * usHeight, CurrentBackColor );		
	
}

/*********************end of file*************************/


