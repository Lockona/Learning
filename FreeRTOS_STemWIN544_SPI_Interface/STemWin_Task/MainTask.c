
#include "GUI.h"
#include "MainTask.h"
#include "ScreenShot.h"
#include "FreeRTOS.h"
#include "task.h"
UINT    f_num;
GUI_BITMAP bitmap;
GUI_LOGPALETTE palette;
/**
  * @brief ���ⲿ�洢���ж�ȡ������BMPͼƬ����
  * @note ��
  * @param sFilename��Ҫ��ȡ���ļ���
  *        x��Ҫ��ʾ��x������
  *        y��Ҫ��ʾ��y������
  * @retval ��
  */
static void ShowStreamedBitmap(const char *sFilename, int x, int y)
{
 	WM_HMEM hMem;
  char *_acbuffer = NULL;

	/* �����ٽ�� */
	taskENTER_CRITICAL();
	/* ��ͼƬ */
	result = f_open(&file, sFilename, FA_READ);
	if ((result != FR_OK))
	{
	//	printf("�ļ���ʧ�ܣ�\r\n");
		_acbuffer[0]='\0';
	}
	
	/* ����һ�鶯̬�ڴ�ռ� */
	hMem = GUI_ALLOC_AllocZero(file.fsize);
	/* ת����̬�ڴ�ľ��Ϊָ�� */
	_acbuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡͼƬ���ݵ���̬�ڴ��� */

	result = f_read(&file, _acbuffer, file.fsize, &f_num);
	if(result != FR_OK)
	{
//		printf("�ļ���ȡʧ�ܣ�\r\n");
	}	
	/* ������λͼ */
	GUI_DrawStreamedBitmapAuto(_acbuffer, x, y);
	
	/* ��ȡ��Ϲر��ļ� */
  f_close(&file);
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL();
	/* �ͷ��ڴ� */
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
