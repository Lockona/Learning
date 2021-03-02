#include "stm32f4xx.h"

#define ADC_START_EVENT 0x01<<0
#define ADC_STOP_EVENT 0x01<<1

void ADC_Config(void);
void ADC_Start(void);
void ADC_Stop(void);
