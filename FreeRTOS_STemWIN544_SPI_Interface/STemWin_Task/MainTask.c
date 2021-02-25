
#include "GUI.h"
#include "MainTask.h"
int i = 2;
char acText[] = "This example demostrates text wrapping";					//定义字符串
GUI_RECT Rect = { 50, 50, 220, 300 };													//定义矩形显示区域
GUI_WRAPMODE  aWm[] = { GUI_WRAPMODE_NONE,GUI_WRAPMODE_CHAR,GUI_WRAPMODE_WORD };
// USER START (Optionally insert additional public code)
// USER END

void MainTask(void)
{
  //GUI_Init();
	GUI_HMEM hQr;
	char str[]="http://www.baidu.com";
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_Clear();//清屏不完全，要清两次
	GUI_SetColor(GUI_BLACK);
//	GUI_SetPenSize(1);
	hQr = GUI_QR_Create(str,5,GUI_QR_ECLEVEL_H,0);
	GUI_QR_Draw(hQr,10,10);
//	GUI_SetLineStyle(GUI_LS_DASH);
//	GUI_DrawLine(50,50,170,270);
//	GUI_FillRect(50,50,170,270);
	GUI_QR_Delete(hQr);
  while (1)
  {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
