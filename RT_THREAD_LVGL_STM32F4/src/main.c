/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
lv_obj_t* label1;
lv_obj_t* label2;
lv_obj_t* chart;
lv_chart_series_t* ser;

rt_thread_t lv_gui_task=RT_NULL;
rt_thread_t rtc_task=RT_NULL;

rt_timer_t lvgl_time=RT_NULL;

static rt_mq_t time_mq=RT_NULL;
rt_mailbox_t adc_mb_handle=RT_NULL;

void lv_ex_get_started_1(void);
static void rtc_show_task(void *parameter);
static void lvgl_tick(void *parameter);
static void lvgl_task(void *parameter);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	rt_enter_critical();
	time_mq=rt_mq_create("time_mq",12,1,RT_IPC_FLAG_FIFO);
	adc_mb_handle=rt_mb_create("adc_value",1,RT_IPC_FLAG_FIFO);
	lv_gui_task=rt_thread_create("lv_gui",lvgl_task,RT_NULL,1024*2,4,50);
	if(RT_NULL!=lv_gui_task)
		rt_thread_startup(lv_gui_task);
	else
		return -1;
	
	rtc_task=rt_thread_create("rtc",rtc_show_task,RT_NULL,512,5,50);
	if(RT_NULL!=rtc_task)
		rt_thread_startup(rtc_task);
	else
		return -1;
	lvgl_time=rt_timer_create("lv_tick",lvgl_tick,RT_NULL,LV_DISP_DEF_REFR_PERIOD,RT_TIMER_FLAG_PERIODIC);
	if(RT_NULL!=lvgl_time)
		rt_timer_start(lvgl_time);
	else
		return -1;
	rt_exit_critical();
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */


  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */



  /* USER CODE BEGIN 2 */
  

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */
}


/* USER CODE BEGIN 4 */
static void rtc_show_task(void *parameter)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	char str[12];
	int time=-1;
	while(1)
	{
		// 获取日历	
		HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
		if(time!=RTC_TimeStructure.Seconds)
		{
			time=RTC_TimeStructure.Seconds;
			rt_sprintf(str,"%02d:%02d:%02d",RTC_TimeStructure.Hours,RTC_TimeStructure.Minutes,RTC_TimeStructure.Seconds);
			rt_mq_send(time_mq,str,rt_strlen(str));
		}
		rt_thread_delay(500);
	}
}

static void lvgl_task(void *parameter)
{
	char str[12];
	rt_uint32_t adc_value;
	lv_init();
	lv_port_disp_init();
	lv_ex_get_started_1();
	while(1)
	{
		if(RT_EOK==rt_mq_recv(time_mq,str,sizeof(str),0))
		{
			
			lv_label_set_text(label1,str);
			lv_label_refr_text(label1);

		}
		if(RT_EOK==rt_mb_recv(adc_mb_handle,&adc_value,0))
		{
				lv_chart_set_next(chart,ser,(lv_coord_t)((double)(adc_value*66/4096)));
		}
		lv_task_handler();
		rt_thread_delay(10);
		
	}
}
static void lvgl_tick(void *parameter)
{
		lv_tick_inc(LV_DISP_DEF_REFR_PERIOD);
}
static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        static uint8_t clicked = 0;
        clicked=1-clicked;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        if (clicked) {
            lv_label_set_text(label, "Stop");
        }
        else
        {
            lv_label_set_text(label, "Start");
        }
        
        
    }
}
static void slider_event_cb(lv_obj_t* slider, lv_event_t event)
{
    static uint16_t i = 0;
    if (event==LV_EVENT_VALUE_CHANGED)
    {
        i=lv_slider_get_value(slider);
        lv_label_set_text_fmt(label2,"%d",i);
    }
}
void lv_ex_get_started_1(void)
{
    lv_obj_t* btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 60, 30);                          /*Set its size*/
    lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_pos(chart, 85, 5);
    lv_obj_set_size(chart, 230, 200);
    lv_obj_set_style_local_pad_left(chart,LV_CHART_PART_BG,LV_STATE_DEFAULT,30);
    lv_obj_set_style_local_pad_bottom(chart,LV_CHART_PART_BG,LV_STATE_DEFAULT,35);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(chart, 5, 5);
    lv_chart_set_y_tick_length(chart, 10, 5);
    lv_chart_set_y_tick_texts(chart, "5\n4\n3\n2\n1\n0", 5, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_x_tick_length(chart, 10, 5);
    lv_chart_set_x_tick_texts(chart, "0\n1\n2\n3\n4\n5", 5, LV_CHART_AXIS_DRAW_LAST_TICK);
    ser = lv_chart_add_series(chart, LV_COLOR_RED);

//    for (int i=0;i<10;i++)
//    {
//        if (i%2==0)
//        {
//            lv_chart_set_next(chart,X_ser,30);
//        }
//        else
//        {
//            lv_chart_set_next(chart,X_ser, 40);
//        }
//        if (i % 2 == 0)
//        {
//            lv_chart_set_next(chart, Y_ser, 30);
//        }
//        else
//        {
//            lv_chart_set_next(chart, Y_ser, 10);
//        }
//    }
    lv_chart_refresh(chart);
   
    label1 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_set_pos(label1,15,110);
    lv_obj_set_size(label1,50,30);
    lv_label_set_text_fmt(label1,"%02d:%02d:%02d",19,11,11);
    label2 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_set_pos(label2, 80, 215);
    lv_obj_set_size(label2,60,30);
    lv_label_set_text(label2,"100");
    lv_obj_t* slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_pos(slider,120,220);
    lv_obj_set_size(slider,180,10);
    lv_obj_set_event_cb(slider, slider_event_cb);
    lv_slider_set_range(slider, 0, 100);
    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "Start");                     /*Set the labels text*/
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
