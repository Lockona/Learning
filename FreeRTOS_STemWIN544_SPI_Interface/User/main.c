/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ????????
  *********************************************************************
  * @attention
  *
  * ?????:??? STM32???п????? 
  * ???    :http://www.firebbs.cn
  * ???    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */

/*
*************************************************************************
*                             ??????????
*************************************************************************
*/
/* FreeRTOS???? */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ?????????bsp???? */
//#include "./led/bsp_led.h"
//#include "./usart/bsp_debug_usart.h"
//#include "./key/bsp_key.h"
//#include "./lcd/bsp_ili9806g_lcd.h"
//#include "./flash/bsp_spi_flash.h"
//#include "./TouchPad/bsp_touchpad.h"
//#include "./beep/bsp_beep.h"
#include "lcd_driver.h"
#include "touch.h"
//#include "./sram/bsp_sram.h"
//#include "./touch/gt5xx.h"
/* STemWIN???? */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "ScreenShot.h"
/**************************** ?????? ********************************/
/* 
 * ???????????????????????????????????????????????????????????
 * ???????????????????????????????????????????????????????????????
 * ???????????NULL??
 */
/* ?????????? */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED?????? */
//static TaskHandle_t LED_Task_Handle = NULL;
/* Touch?????? */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI?????? */
static TaskHandle_t GUI_Task_Handle = NULL;

/********************************** ???????? *********************************/
/*
 * ?????????????У????????飬???????????Щ?????????????????????Щ???
 * ????????????????????????????????????????????????????????????
 * ?????????????????????Щ??????
 *
 * ?????????????????????????????????Щ?????????????????????????
 * ???????????????????????????Щ???????????????????????Щ??????????
 * ??????
 * 
 */
SemaphoreHandle_t ScreenShotSem_Handle = NULL;

/******************************* ?????????? ************************************/
/*
 * ????????д??ó??????????????????Щ????????
 */

/*
*************************************************************************
*                             ????????
*************************************************************************
*/
static void AppTaskCreate(void); /* ??????????? */

//static void LED_Task(void* parameter);/* LED_Task??????? */
static void GUI_Task(void *parameter); /* GUI_Task??????? */
static void Touch_Task(void *parameter);
static void BSP_Init(void); /* ?????????????????? */


/*****************************************************************
  * @brief  ??????
  * @param  ??
  * @retval ??
  * @note   ????????????????????? 
            ???????????APP???????
            ????????????FreeRTOS??????????????
  ****************************************************************/
int main(void)
{
  BaseType_t xReturn = pdPASS; /* ?????????????????????????pdPASS */

  /* ?????????????? */
  BSP_Init();
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  //printf("\r\n ********** emwin DEMO *********** \r\n");

  /* ????AppTaskCreate???? */
  xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,          /* ?????????? */
                        (const char *)"AppTaskCreate",          /* ???????? */
                        (uint16_t)512,                          /* ???????С */
                        (void *)NULL,                           /* ?????????????? */
                        (UBaseType_t)1,                         /* ?????????? */
                        (TaskHandle_t *)&AppTaskCreate_Handle); /* ??????????? */
  /* ??????????? */
  if (pdPASS == xReturn)
    vTaskStartScheduler(); /* ???????????????? */
  else
    return -1;

  while (1)
    ; /* ??????????е????? */
}

/***********************************************************************
  * @ ??????  ?? AppTaskCreate
  * @ ????????? ??????????????е????????????????????????????
  * @ ????    ?? ??  
  * @ ?????  ?? ??
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS; /* ?????????????????????????pdPASS */

  taskENTER_CRITICAL(); //?????????

  /* ????ScreenShotSem????? */
  ScreenShotSem_Handle = xSemaphoreCreateBinary();
  if (NULL != ScreenShotSem_Handle)
    //		printf("ScreenShotSem?????????????????\r\n");

    //	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* ?????????? */
    //											 (const char*    )"LED_Task",/* ???????? */
    //											 (uint16_t       )128,       /* ???????С */
    //											 (void*          )NULL,      /* ?????????????? */
    //											 (UBaseType_t    )4,         /* ?????????? */
    //											 (TaskHandle_t   )&LED_Task_Handle);/* ??????????? */
    //	if(pdPASS == xReturn)
    //		printf("????LED1_Task????????\r\n");

    xReturn = xTaskCreate((TaskFunction_t)Touch_Task,        /* ?????????? */
                          (const char *)"Touch_Task",        /* ???????? */
                          (uint16_t)256,                     /* ???????С */
                          (void *)NULL,                      /* ?????????????? */
                          (UBaseType_t)3,                    /* ?????????? */
                          (TaskHandle_t)&Touch_Task_Handle); /* ??????????? */
  if (pdPASS == xReturn)
    ;
  //	printf("????Touch_Task????????\r\n");

  xReturn = xTaskCreate((TaskFunction_t)GUI_Task,        /* ?????????? */
                        (const char *)"GUI_Task",        /* ???????? */
                        (uint16_t)1024,                  /* ???????С */
                        (void *)NULL,                    /* ?????????????? */
                        (UBaseType_t)2,                  /* ?????????? */
                        (TaskHandle_t)&GUI_Task_Handle); /* ??????????? */
  if (pdPASS == xReturn)
    ;
  //	printf("????GUI_Task????????\r\n");

  vTaskDelete(AppTaskCreate_Handle); //???AppTaskCreate????

  taskEXIT_CRITICAL(); //????????
}

/**
  * @brief LED????????
  * @note ??
  * @param ??
  * @retval ??
  */
//static void LED_Task(void* parameter)
//{
//	while(1)
//	{
//		LED3_TOGGLE;
//		vTaskDelay(1000);
//	}
//}

/**
  * @brief ???????????????
  * @note ??
  * @param ??
  * @retval ??
  */
static void Touch_Task(void *parameter)
{
  /* ???????? */
  xSemaphoreTake(ScreenShotSem_Handle, /* ??????????? */
                 portMAX_DELAY);       /* ??????? */
  while (1)
  {
    //GTP_TouchProcess();//????????????
    GUI_TOUCH_Scan();
    vTaskDelay(20);
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

  /* ?????STemWin */
  GUI_Init();
  //WM_MULTIBUF_Enable(1);//配置多缓冲
  /* ??????????? */
  //GTP_Init_Panel();
  /* ????????? */
  xSemaphoreGive(ScreenShotSem_Handle);
  /* ??LCD????? */
  //ILI9806G_BackLed_Control ( ENABLE );
  while (1)
  {
    MainTask();
  }
}

/**
  * @brief ?弶????????
  * @note ???а????????????????????????????
  * @param ??
  * @retval ??
  */
static void BSP_Init(void)
{
  /* SRAM????? */
  //  FSMC_SRAM_Init();

  /* CRC??emWin??й?????????????????????????
   * ????STemWin??????????ST??о????棬???о???????????
   */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

  /*
	 * STM32?ж???????????4????4bit?????????????????????Χ???0~15
	 * ???????????????????μ?????????????????????????????ж??
	 * ?????????????????飬????????飬?мɡ?
	 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// WM_SetCreateFlags(WM_CF_MEMDEV);//使用内存设备.不复杂的界面要注释,
  /* LED ????? */
  //	LED_GPIO_Config();
	result = f_mount(&fs,"0:",1);
	if(result != FR_OK)
	{
		//printf("SD¿¨³õÊ¼»¯Ê§°Ü£¬ÇëÈ·±£SD¿¨ÒÑÕýÈ·½ÓÈë¿ª·¢°å£¬»ò»»Ò»ÕÅSD¿¨²âÊÔ£¡\n");
		while(1);
	}
  /* ??????????? */
  //BEEP_GPIO_Config();

  /* ????????	*/
  //Debug_USART_Config();
}

/********************************END OF FILE****************************/
