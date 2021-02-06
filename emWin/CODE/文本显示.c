
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
	GUI_SetBkColor(GUI_BLUE);
//	GUI_Clear();
	GUI_Clear();//清屏不完全，要清两次
	GUI_SetFont(GUI_FONT_32_1);
	GUI_SetColor(GUI_BLACK);
  GUI_DispStringAt("Helllo emWin",10,10);
	GUI_SetPenSize(1);//SPI接口不支持
	GUI_SetColor(GUI_RED);
	GUI_DrawLine(50,50,200,200);
	GUI_DrawLine(50,200,200,50);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	GUI_DispStringHCenterAt("Hello!!",120,160);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(GUI_FONT_24_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	GUI_DispStringInRectWrap(acText,&Rect,GUI_TA_LEFT,aWm[i]);
	GUI_DispNextLine();
	GUI_DispDec(12345,6);
	GUI_DispDecSpace(23,6);
	GUI_GotoX(12*10);
	GUI_DispDec(-1345,6);
	GUI_DispNextLine();
	GUI_DispSDecShift(1234,7,2);
  while (1)
  {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
