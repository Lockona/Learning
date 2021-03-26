#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f4xx.h" 
#include "lcd_driver.h"
//#include "touch.h"
#include "lv_port_indev.h"
#include <rthw.h>
#include <rtthread.h>
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "gpio.h"
#include "fsmc.h"
#include "lcd_driver.h"
#include "lv_port_disp.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
