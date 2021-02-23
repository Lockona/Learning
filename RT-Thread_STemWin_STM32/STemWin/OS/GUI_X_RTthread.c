#include "GUI.h"

/* rtthread头文件 */
#include <rthw.h>
#include <rtthread.h>

/*********************************************************************
*
* Global data
*/
static rt_mutex_t xQueueMutex = RT_NULL;
static rt_sem_t xSemaTxDone = RT_NULL;
 
/*********************************************************************
*
* Timing:
* GUI_X_GetTime()
* GUI_X_Delay(int)

Some timing dependent routines require a GetTime
and delay function. Default time unit (tick), normally is
1 ms.
*/
int GUI_X_GetTime(void)
{
	return ((int) rt_tick_get());

}

void GUI_X_Delay(int ms)
{
  rt_thread_delay( (rt_tick_t)ms );
}

/*********************************************************************
*
* GUI_X_Init()
*
* Note:
* GUI_X_Init() is called from GUI_Init is a possibility to init
* some hardware which needs to be up and running before the GUI.
* If not required, leave this routine blank.
*/

void GUI_X_Init(void) {
}


/*********************************************************************
*
* GUI_X_ExecIdle
*
* Note:
* Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {
	GUI_X_Delay(1);
}

/*********************************************************************
*
* Multitasking:
*
* GUI_X_InitOS()
* GUI_X_GetTaskId()
* GUI_X_Lock()
* GUI_X_Unlock()
*
* Note:
* The following routines are required only if emWin is used in a
* true multi task environment, which means you have more than one
* thread using the emWin API.
* In this case the
* #define GUI_OS 1
* needs to be in GUIConf.h
*/

/* Init OS */
void GUI_X_InitOS(void)
{ 
  /* 创建互斥信号量 用于资源共享 */
  xQueueMutex = rt_mutex_create("test_mutex",RT_IPC_FLAG_PRIO);//xSemaphoreCreateMutex();
  //configASSERT (xQueueMutex != NULL);
	RT_ASSERT(xQueueMutex != NULL);
	/* 创建二值信号量 用于事件触发 */
	xSemaTxDone=rt_sem_create("test_sem",1,RT_IPC_FLAG_FIFO);
	RT_ASSERT(xSemaTxDone != NULL);
  //vSemaphoreCreateBinary( xSemaTxDone );
  //configASSERT ( xSemaTxDone != NULL );
}

void GUI_X_Unlock(void)
{
	/* 给出互斥量 */
  //xSemaphoreGive(xQueueMutex);
	rt_mutex_release(xQueueMutex);
}

void GUI_X_Lock(void)
{
  if(xQueueMutex == NULL)
  {
    GUI_X_InitOS();
  }
	/* 获取互斥量 */
//  xSemaphoreTake(xQueueMutex,   /* 互斥量句柄 */
//								 portMAX_DELAY);/* 阻塞等待 */
	rt_mutex_take(xQueueMutex,   /* 互斥量句柄 */
								 RT_WAITING_FOREVER);
}

/* Get Task handle */
U32 GUI_X_GetTaskId(void) 
{ 
	rt_thread_t thread=rt_thread_self();
  return ((U32) thread);
}


void GUI_X_WaitEvent (void) 
{
  /* 获取信号量 */
 rt_sem_take(xSemaTxDone,RT_WAITING_FOREVER);/* 阻塞等待 */
}


void GUI_X_SignalEvent (void) 
{
  /* 给出信号量 */
  //xSemaphoreGive(xSemaTxDone);
	rt_sem_release(xSemaTxDone);
}

/*********************************************************************
*
* Logging: OS dependent

Note:
Logging is used in higher debug levels only. The typical target
build does not use logging and does therefor not require any of
the logging routines below. For a release build without logging
the routines below may be eliminated to save some space.
(If the linker is not function aware and eliminates unreferenced
functions automatically)

*/

void GUI_X_Log (const char *s) { }
void GUI_X_Warn (const char *s) { }
void GUI_X_ErrorOut(const char *s) { }

/*************************** End of file ****************************/
