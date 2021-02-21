#ifndef __BOARD_H__
#define __BOARD_H__


#include "main.h"
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include ".\key\bsp_key.h" 

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif

