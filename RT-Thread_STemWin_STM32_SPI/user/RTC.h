#include "stm32f4xx.h"

#define ASYHCHPREDIV 0X7F
#define SYHCHPREDIV 0XFF

// 备份域寄存器宏定义
#define RTC_BKP_DRX RTC_BKP_DR0
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA 0X32F2

void RTC_Config(void);
void RTC_Time_Set(void);
