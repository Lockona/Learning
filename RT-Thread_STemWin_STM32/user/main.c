#include "rtthread.h"
#include "rthw.h"
#include "board.h"

static rt_sem_t ScreenShotSem_Handle =RT_NULL;

static rt_thread_t gui_thread_entry=RT_NULL;
static rt_thread_t touch_thread_entry=RT_NULL;

static void GUI_Task(void *parameter); /* GUI_Task??????? */
static void Touch_Task(void *parameter);



int main(void)
{
	rt_enter_critical();
	ScreenShotSem_Handle=rt_sem_create("ScreenShot",0,RT_IPC_FLAG_FIFO);
	gui_thread_entry=rt_thread_create("GUI",GUI_Task,RT_NULL,1024,4,500);
	if(RT_NULL!=gui_thread_entry)
		rt_thread_startup(gui_thread_entry);
	else
		return -1;
	touch_thread_entry=rt_thread_create("touch",Touch_Task,RT_NULL,512,3,20);
	if(RT_NULL!=touch_thread_entry)
		rt_thread_startup(touch_thread_entry);
	else
		return -1;
	rt_exit_critical();
}


static void Touch_Task(void *parameter)
{

//  xSemaphoreTake(ScreenShotSem_Handle, /* ??????????? */
//                 portMAX_DELAY);       /* ??????? */
	rt_sem_take(ScreenShotSem_Handle,RT_WAITING_FOREVER);
  while (1)
  {
    //GTP_TouchProcess();
    GUI_TOUCH_Scan();
    rt_thread_delay(20);
  }
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
//  WM_MULTIBUF_Enable(1);//配置多缓冲

	rt_sem_release(ScreenShotSem_Handle);
  while (1)
  {
    MainTask();
  }
}

/********************************END OF FILE****************************/
