#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f4xx.h" 
#include "lcd_driver.h"
#include "touch.h"
#include <rthw.h>
#include <rtthread.h>
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "ScreenShot.h"
#include "RTC.h"
#include "ADC.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
