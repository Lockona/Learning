
#include "GUI.h"
#include "MainTask.h"
#include "ScreenShot.h"
#include "FreeRTOS.h"
#include "task.h"
UINT    f_num;
GUI_BITMAP bitmap;
GUI_LOGPALETTE palette;
/**
  * @brief 从外部存储器中读取并绘制BMP图片数据
  * @note 无
  * @param sFilename：要读取的文件名
  *        x：要显示的x轴坐标
  *        y：要显示的y轴坐标
  * @retval 无
  */
static void ShowStreamedBitmap(const char *sFilename, int x, int y)
{
 	WM_HMEM hMem;
  char *_acbuffer = NULL;

	/* 进入临界段 */
	taskENTER_CRITICAL();
	/* 打开图片 */
	result = f_open(&file, sFilename, FA_READ);
	if ((result != FR_OK))
	{
	//	printf("文件打开失败！\r\n");
		_acbuffer[0]='\0';
	}
	
	/* 申请一块动态内存空间 */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	/* 转换动态内存的句柄为指针 */
	_acbuffer = GUI_ALLOC_h2p(hMem);

	/* 读取图片数据到动态内存中 */

	result = f_read(&file, _acbuffer, file.fsize, &f_num);
	if(result != FR_OK)
	{
//		printf("文件读取失败！\r\n");
	}	
	/* 绘制流位图 */
	GUI_DrawStreamedBitmapAuto(_acbuffer, x, y);
	
	/* 读取完毕关闭文件 */
  f_close(&file);
	/* 退出临界段 */
	taskEXIT_CRITICAL();
	/* 释放内存 */
	GUI_ALLOC_Free(hMem);
}
// USER START (Optionally insert additional public code)
// USER END

void MainTask(void)
{
  ShowStreamedBitmap("0:/main.py", 0,0);
  while (1)
  {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
