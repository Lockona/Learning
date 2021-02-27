#include "RTC.h"

void RTC_Config(void)
{
	RTC_InitTypeDef RTC_Init_Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	
	RTC_Init_Structure.RTC_AsynchPrediv=ASYHCHPREDIV;
	RTC_Init_Structure.RTC_HourFormat=RTC_HourFormat_24;
	RTC_Init_Structure.RTC_SynchPrediv=SYHCHPREDIV;
	
	RTC_Init(&RTC_Init_Structure);
	
	
}

void RTC_Time_Set(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_TimeStruct.RTC_H12=RTC_H12_PM;
	RTC_TimeStruct.RTC_Hours=23;
	RTC_TimeStruct.RTC_Minutes=16;
	RTC_TimeStruct.RTC_Seconds=50;
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
	RTC_DateStruct.RTC_Date= 26;
	RTC_DateStruct.RTC_Month=2;
	RTC_DateStruct.RTC_WeekDay=5;
	RTC_DateStruct.RTC_Year=21;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
}
