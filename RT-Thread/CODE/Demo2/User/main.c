/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO���--ʹ�ù̼������LED��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "rtthread.h"
#define Dheap 1 //��̬�ڴ�ʹ��

static void led1_thread_entry(void *parameter);
static void led2_thread_entry(void *parameter);

ALIGN(RT_ALIGN_SIZE)

#if !Dheap //ʹ�þ�̬�ڴ�
static rt_uint8_t led1_thread_stack[1024];
static struct rt_thread led1_thread;
#else //ʹ�ö�̬�ڴ�
	static rt_thread_t led1_thread = RT_NULL;
	static rt_thread_t led2_thread = RT_NULL;
#endif

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	#if !Dheap
		rt_thread_init(&led1_thread,
									"led1",
									(led1_thread_entry),
									RT_NULL,
									&led1_thread_stack[0],
									sizeof(led1_thread_stack),
									3,
									20);
		rt_thread_startup(&led1_thread);
	#else
		led1_thread = rt_thread_create("lea1",led1_thread_entry,RT_NULL,512,3,20);
		if(led1_thread != RT_NULL)
			rt_thread_startup(led1_thread);
		else 
			return -1;
		led2_thread = rt_thread_create("lea2",led2_thread_entry,RT_NULL,512,4,20);
		if(led2_thread != RT_NULL)
			rt_thread_startup(led2_thread);
		else 
			return -1;
	#endif
	
}
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1_ON;
		rt_thread_delay(500);
		LED1_OFF;
		rt_thread_delay(500);
	}
}

static void led2_thread_entry(void *parameter)
{
	while(1)
	{
		LED2_ON;
		rt_thread_delay(500);
		LED2_OFF;
		rt_thread_delay(500);
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
