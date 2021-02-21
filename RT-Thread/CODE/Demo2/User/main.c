/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO输出--使用固件库点亮LED灯
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "rtthread.h"
#define Dheap 1 //动态内存使能

static void led1_thread_entry(void *parameter);
static void led2_thread_entry(void *parameter);
static void key_thread_entry(void *parameter);

ALIGN(RT_ALIGN_SIZE)

#if !Dheap //使用静态内存
static rt_uint8_t led1_thread_stack[1024];
static struct rt_thread led1_thread;
#else //使用动态内存
	static rt_thread_t led1_thread = RT_NULL;
	static rt_thread_t led2_thread = RT_NULL;
	static rt_thread_t key_thread = RT_NULL;
#endif

static rt_mq_t led_mq = RT_NULL;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	led_mq = rt_mq_create("led",40,20,RT_IPC_FLAG_FIFO);
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
	key_thread = rt_thread_create("key",key_thread_entry,RT_NULL,512,3,20);
		if(key_thread != RT_NULL)
			rt_thread_startup(key_thread);
		else 
			return -1;
		led1_thread = rt_thread_create("lea1",led1_thread_entry,RT_NULL,512,4,20);
		if(led1_thread != RT_NULL)
			rt_thread_startup(led1_thread);
		else 
			return -1;
		led2_thread = rt_thread_create("lea2",led2_thread_entry,RT_NULL,512,5,20);
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
	rt_uint32_t data;
	rt_err_t uwRet=RT_EOK;
	while(1)
	{
		uwRet=rt_mq_recv(led_mq,&data,sizeof(data),RT_WAITING_FOREVER);
		if(data == 0)
		{
			LED2_ON;
		}
		else
		{
		
			LED2_OFF;
		}
	}
}
static void key_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	rt_uint32_t data1 = 1;
	rt_uint32_t data2 = 0;
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN)==KEY_ON)
		{
			uwRet=rt_mq_send(led_mq,&data1,sizeof(data1));
		}
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN)==KEY_ON)
		{
			uwRet=rt_mq_send(led_mq,&data2,sizeof(data2));
		}
		rt_thread_delay(20);
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
