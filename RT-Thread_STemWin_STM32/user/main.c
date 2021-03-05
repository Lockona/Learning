#include "rtthread.h"
#include "rthw.h"
#include "board.h"

static rt_sem_t ScreenShotSem_Handle =RT_NULL;
rt_mq_t time_mq_Handle=RT_NULL;
rt_mailbox_t adc_mb_Handle=RT_NULL;
rt_event_t adc_startEvent_Handle=RT_NULL;

static rt_thread_t gui_thread_entry=RT_NULL;
static rt_thread_t touch_thread_entry=RT_NULL;
static rt_thread_t time_get_thread_entry=RT_NULL;
static rt_thread_t adc_thread_entry=RT_NULL;

static void GUI_Task(void *parameter); /* GUI_Task??????? */
static void Touch_Task(void *parameter);
static void time_get_Task(void *parameter);
static void adc_Task(void *parameter);

int main(void)
{
	rt_enter_critical();
	
	ScreenShotSem_Handle=rt_sem_create("ScreenShot",0,RT_IPC_FLAG_FIFO);
	time_mq_Handle=rt_mq_create("getTime_mq",6,1,RT_IPC_FLAG_FIFO);
	adc_mb_Handle=rt_mb_create("ADC",1,RT_IPC_FLAG_FIFO);
	adc_startEvent_Handle=rt_event_create("ADC_Strart",RT_IPC_FLAG_PRIO);
	gui_thread_entry=rt_thread_create("GUI",GUI_Task,RT_NULL,1024*4,2,200);
	if(RT_NULL!=gui_thread_entry)
		rt_thread_startup(gui_thread_entry);
	else
		return -1;
	touch_thread_entry=rt_thread_create("touch",Touch_Task,RT_NULL,512,3,20);
	if(RT_NULL!=touch_thread_entry)
		rt_thread_startup(touch_thread_entry);
	else
		return -1;
	time_get_thread_entry=rt_thread_create("get Time",time_get_Task,RT_NULL,256,4,20);
	if(RT_NULL!=time_get_thread_entry)
		rt_thread_startup(time_get_thread_entry);
	else
		return -1;
	adc_thread_entry=rt_thread_create("ADC_Task",adc_Task,RT_NULL,256,5,20);
	if((RT_NULL!=adc_thread_entry)&&(adc_startEvent_Handle!=RT_NULL))
		rt_thread_startup(adc_thread_entry);
	else
		return -1;
	
	rt_exit_critical();
	while(1){
		rt_thread_delay(500);
	}
}

static void adc_Task(void *parameter)
{
	rt_uint32_t recved;
//	ADC_Stop();
	while(1)
	{
		rt_event_recv(adc_startEvent_Handle,ADC_START_EVENT,
			RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&recved);
		rt_enter_critical();
		if(recved==ADC_START_EVENT)
			ADC_Start();
		else if(recved==ADC_STOP_EVENT)
			ADC_Stop();
		rt_exit_critical();
		recved=RT_NULL;
	}
}

static void time_get_Task(void *parameter)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	rt_uint16_t rtc_temp=0;
	char str[6];
	while(1)
	{
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
		if(rtc_temp!=RTC_TimeStructure.RTC_Seconds)
		{
			rtc_temp=RTC_TimeStructure.RTC_Seconds;	
			rt_sprintf(str,"%02d:%02d",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Seconds);
			rt_mq_send(time_mq_Handle,str,rt_strlen(str));
		}
		rt_thread_delay(100);
	}
}

static void Touch_Task(void *parameter)
{

//  xSemaphoreTake(ScreenShotSem_Handle, /* ??????????? */
//                 portMAX_DELAY);       /* ??????? */
	rt_sem_take(ScreenShotSem_Handle,RT_WAITING_FOREVER);
//  while (1)
//  {
//    //GTP_TouchProcess();
//    GUI_TOUCH_Scan();
//    rt_thread_delay(20);
//  }
}

/**
  * @brief GUI????????
  * @note ??
  * @param ??
  * @retval ??
  */
static void GUI_Task(void *parameter)
{


  GUI_Init();
  //WM_MULTIBUF_Enable(1);//配置多缓冲

	rt_sem_release(ScreenShotSem_Handle);
  while (1)
  {
    MainTask();
  }
}

/********************************END OF FILE****************************/
