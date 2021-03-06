/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	// ����RTC����
	hrtc.Instance = RTC;

	/*ʹ�� PWR ʱ��*/
	__HAL_RCC_PWR_CLK_ENABLE();
	/* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
	HAL_PWR_EnableBkUpAccess();

	#if defined (RTC_CLOCK_SOURCE_LSI) 
	/* ʹ��LSI��ΪRTCʱ��Դ������� 
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
	/* ��ʼ��LSI */ 
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* ѡ��LSI��ΪRTC��ʱ��Դ */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* ��ʼ��LSE */ 
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* ѡ��LSE��ΪRTC��ʱ��Դ */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

//	/* Configures the External Low Speed oscillator (LSE) drive capability */
//	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);  

	#endif /* RTC_CLOCK_SOURCE_LSI */

	/* ʹ��RTCʱ�� */
	__HAL_RCC_RTC_ENABLE(); 

	/* �ȴ� RTC APB �Ĵ���ͬ�� */
	HAL_RTC_WaitForSynchro(&hrtc);

	/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */

	/* �����첽Ԥ��Ƶ����ֵ */
	hrtc.Init.AsynchPrediv = ASYNCHPREDIV;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	hrtc.Init.SynchPrediv  = SYNCHPREDIV;	
	hrtc.Init.HourFormat   = RTC_HOURFORMAT_24; 
	
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		while(1);
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void rtc_time_set(void)
{
	RTC_DateTypeDef  RTC_DateStructure;
  RTC_TimeTypeDef  RTC_TimeStructure;
	// ��ʼ��ʱ��
	RTC_TimeStructure.TimeFormat = RTC_H12_AMorPM;
	RTC_TimeStructure.Hours = 22;        
	RTC_TimeStructure.Minutes = 19;      
	RTC_TimeStructure.Seconds = 30;      
  HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure, RTC_FORMAT_BIN);
    // ��ʼ������	
	RTC_DateStructure.WeekDay = 5;       
	RTC_DateStructure.Date = 5;         
	RTC_DateStructure.Month = 3;         
	RTC_DateStructure.Year = 21;        
  HAL_RTC_SetDate(&hrtc,&RTC_DateStructure, RTC_FORMAT_BIN);
    
  HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DRX, RTC_BKP_DATA);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
