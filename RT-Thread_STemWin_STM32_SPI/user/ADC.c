#include "ADC.h"
#include "board.h"

void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_10Cycles;
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion=1;
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_56Cycles);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_SoftwareStartConv(ADC1);
	
	NVIC_InitStructure.NVIC_IRQChannel=ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	
}

void ADC_Start(void)
{
	ADC1->CR2|=1<<0;
	ADC1->CR2|=1<<1;
	ADC1->CR2|=1<<30;
	
}
void ADC_Stop(void)
{
	ADC1->CR2&=~(1<<1);
	ADC1->CR2&=~(1<<30);
	ADC1->CR2&=~(1<<0);
	
}

extern rt_mailbox_t adc_mb_Handle;
static rt_uint16_t Adc_Vaules[10];
static int Adc_Index=0;
void ADC_IRQHandler(void)
{
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET)
	{
		Adc_Vaules[Adc_Index]=ADC_GetConversionValue(ADC1);
		Adc_Index++;
		if(Adc_Index>9)
		{
			Adc_Index=0;
			rt_mb_send(adc_mb_Handle,(rt_uint32_t)&Adc_Vaules);
		}
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}
