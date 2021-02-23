
#include "GUI.h"
#include "MainTask.h"
#include "ScreenShot.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "rtthread.h"
#include "rthw.h"
#include "GUIFont_Create.h"


UINT    f_num;
GUI_BITMAP bitmap;
GUI_LOGPALETTE palette;
char *_acbuffer;

extern const char Framewin_text[];
extern const char text[];
extern const char MULTIEDIT_text[];
extern const char *BUTTON_text[];
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
// 	WM_HMEM hMem;
//  char *_acBuffer = NULL;

//	/* 进入临界段 */
//	taskENTER_CRITICAL();
//	/* 打开图片 */
//	result = f_open(&file, sFilename, FA_READ);
//	if ((result != FR_OK))
//	{
//	//	printf("文件打开失败！\r\n");
//		_acBuffer[0]='\0';
//	}
//	
//	/* 申请一块动态内存空间 */
//	hMem = GUI_ALLOC_AllocZero(file.fsize);
//	/* 转换动态内存的句柄为指针 */
//	_acBuffer = GUI_ALLOC_h2p(hMem);

//	/* 读取图片数据到动态内存中 */

//	result = f_read(&file, _acBuffer, file.fsize, &f_num);
//	if(result != FR_OK)
//	{
////		printf("文件读取失败！\r\n");
//	}	
//	/* 绘制流位图 */
//	GUI_DrawStreamedBitmapAuto(_acBuffer, x, y);
//	
//	/* 读取完毕关闭文件 */
//  f_close(&file);
//	/* 退出临界段 */
//	taskEXIT_CRITICAL();
//	/* 释放内存 */
//	GUI_ALLOC_Free(hMem);
}
// USER START (Optionally insert additional public code)
// USER END

static void _calback(WM_MESSAGE *pMsg)
{
	GUI_RECT rect;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&rect);
			GUI_SetBkColor(GUI_RED);
			GUI_SetColor(GUI_YELLOW);
			GUI_ClearRectEx(&rect);
			GUI_DrawRectEx(&rect);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
int _GetData(void *p,const U8 **ppData,unsigned NumBytesReq,U32 Off)
{
	static int FileAddress =0;
	UINT NumBytesRead;
	FIL *PicFile;
	
	PicFile = (FIL *)p;
	if(NumBytesReq > sizeof(_acbuffer))
	{
		NumBytesReq = sizeof(_acbuffer);
	}
	if(Off ==1 )
	{
		FileAddress = 0;
	}
	else{
		FileAddress = Off;
	}
	result=f_lseek(PicFile,FileAddress);
	//taskENTER_CRITICAL();
	rt_enter_critical();
	result=f_read(PicFile,_acbuffer,NumBytesReq,&NumBytesRead);
	//taskEXIT_CRITICAL();
	rt_exit_critical();
	*ppData=(const U8*)_acbuffer;
	
	return NumBytesRead;
}
static void show_gif(const char *sFile)
{
	WM_HMEM hMem;
	GUI_GIF_INFO Gif_Info={0};
	GUI_GIF_IMAGE_INFO Gif_Image_Info={0};
	int i =0;
	
	//taskENTER_CRITICAL();
	rt_enter_critical();
	result=f_open(&file,sFile,FA_READ);
	if(result!=FR_OK)
	{
		_acbuffer[0]='\0';
	}
	hMem=GUI_ALLOC_AllocZero(4*1024);
	_acbuffer=GUI_ALLOC_h2p(hMem);
	//taskEXIT_CRITICAL();
	rt_exit_critical();
	
	GUI_GIF_GetInfoEx(_GetData,&file,&Gif_Info);
	for(i =0 ;i<Gif_Info.NumImages;i++)
	{
			GUI_GIF_GetImageInfoEx(_GetData,&file,&Gif_Image_Info,i);
			GUI_GIF_DrawSubEx(_GetData,&file,0,10,i);
			GUI_Delay(Gif_Image_Info.Delay);
	}
	f_close(&file);
	/* 释放内存 */
	GUI_ALLOC_Free(hMem);
}
static void show_jpeg(const char *sFile)
{
	WM_HMEM hMem;
	//taskENTER_CRITICAL();
	rt_enter_critical();
	result = f_open(&file,sFile,FA_READ);
	if(result != FR_OK)
	{
		_acbuffer[0]='\0';
	}
	hMem=GUI_ALLOC_AllocZero(4*1024);
	_acbuffer=GUI_ALLOC_h2p(hMem);
	//taskEXIT_CRITICAL();
	rt_exit_critical();
	GUI_JPEG_DrawEx(_GetData,&file,0,0);
	f_close(&file);
	/* 释放内存 */
	GUI_ALLOC_Free(hMem);
}

void MainTask(void)
{
//	int i=0;
//	WM_HWIN hWin;
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
//	GUI_SetColor(GUI_WHITE);
//	GUI_SetFont(&GUI_Font24_ASCII);
//	hWin = WM_CreateWindow(10,10,150,150,WM_CF_SHOW|WM_CF_MEMDEV,_calback,0);
//	for(;i<20;i++){
//	WM_MoveWindow(hWin,1,1);
//		GUI_Delay(100);
//	}
//	WM_DeleteWindow(hWin);
//	GUI_Exec();
	//show_jpeg("0:/5.jpg");
	//show_gif("0:/mario.gif");
//	Create_XBF_Font();
//	GUI_SetFont(&FONT_GAITI_18);
	GUI_SetFont(&GUI_Font16B_1);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringAt(BUTTON_text[0],10,10);
  while (1)
  {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
