#include "stm32f4xx.h"

#define ASYHCHPREDIV 0X7F
#define SYHCHPREDIV 0XFF

// ������Ĵ����궨��
#define RTC_BKP_DRX RTC_BKP_DR0
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA 0X32F2

void RTC_Config(void);
void RTC_Time_Set(void);
