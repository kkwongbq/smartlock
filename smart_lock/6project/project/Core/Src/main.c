/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcdtp.h"
#include "xpt2046.h"
#include "help.h"
#include "pn532_stm32f1.h"
#include "mylibrary.h"
#include "syn6288.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "as608.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define use_HEXADECIMAL

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern RTC_HandleTypeDef hrtc;
char now_time[20];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int testpassword = 0;
int oldpassword = 1234; // 全局变量
int managerpassword = 2345;
int position = 80;

strType_XPT2046_Coordinate strDisplayCoordinate; // 全局变量
char* NFC_KEYS[5] = {NULL}; // 初始化所有元素为NULL
char manager_nfc[50];
uint8_t returnToNFCMenu = 0; // 初始化标志位
int size_NFC = 1;

uint8_t RX_dat = 0;
int count = 0;
///////////////////////////////////////////////////////////
uint8_t SYS6288Rx = 0; // 语音模块接收缓冲�???????????????
extern uint8_t suoyidakai[] ;
extern uint8_t suoyiguanbi[];
extern uint8_t mimacuowu[]; // 密码错误
extern uint8_t jinruguanliyemian[]; // 进入管理员界�?
extern uint8_t tuichuguanliyemian[]; // �?出管理员界面
extern uint8_t cuowu[];
extern uint8_t jinggao[] ;// 警告
extern uint8_t jingbaojiechuy[];//警报解除
//////////////////////////////////////////////////////

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //syn6288
{
	if (huart->Instance == USART2)
	{
		HAL_UART_Receive_IT(&huart2, &SYS6288Rx, 1);
		//HAL_UART_Receive_IT(&UART2_Handler, &SYS6288Rx, 1);
	}
}

volatile uint32_t seconds = 0;
char left_scond[255];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) { // 假设使用TIM2
        seconds++;
    }
}


uint16_t currentFingerprintCount = 0;

/////////////"ERROR-time"///////////////////////
int test_time = 0;
int finger_time = 0;
int nfc_time = 0;
/////////////////////////////////////////////////
int returnTomain = 0;
int enter = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 NFC_KEYS[0] = strdup("8A 19 43 02");
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  MX_RTC_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  int currentPosition = 170;//lock
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  TIM1->CCR4 = currentPosition;
  HAL_Delay(200);

  macXPT2046_CS_DISABLE();

  LCD_INIT();

  LCD_Clear(50, 80, 140, 70, RED);
  LCD_DrawString(68, 100, "TOUCHPAD DEMO");
  HAL_Delay(2000);

  while (!XPT2046_Touch_Calibrate());

  //////////////////////"pn532-start"////////////////////
  HAL_Delay(1000);
  memset(buffer, 0, sizeof(buffer));
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim5); // 启动定时器中�???????????


  uint8_t buff[255];
  uint8_t uid[MIFARE_UID_MAX_LENGTH];
 // int32_t uid_len = 0;


  PN532 pn532;
  PN532_SPI_Init(&pn532);
  HAL_Delay(100); // 等待PN532模块稳定

  int status = PN532_GetFirmwareVersion(&pn532, buff);
      if (status != PN532_STATUS_OK)
      {
          char debug_msg[50];
          sprintf(debug_msg, "Firmware version error: %d", status);
          LCD_DrawString(60, 160, debug_msg);
      while(1)
      {
    	  LCD_DrawString(60, 180, "can not go inside !");
      }
      }
      else
      {
    	  LCD_DrawString(60, 160, "hello oooooooo");
    	  HAL_Delay(100);
      }
  PN532_SamConfiguration(&pn532);

  ////////////////////"pn532-end///////////////
  ////////////////////"as608-start//////////////
  as608_init(); // initial the as608
  PS_Empty();

  ////////////////////"as608-end"///////////////

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  strType_XPT2046_Coordinate strDisplayCoordinate;

  ///////////////////////////////


  ////////////////////////////////////

  // 绘制母循环界�????????????????????????????????
  DrawMainMenu();
  SYS6288_uartInit(9600);					//语音串口
  HAL_TIM_Base_Start(&htim5);

  while (1)
  {
      RTC_TimeTypeDef sTime;
      RTC_DateTypeDef sDate;

      // 获取当前时间
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

      char* week_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
      char now_date[20];
      sprintf(now_date, "%04d-%02d-%02d %s", 2000 + sDate.Year, sDate.Month, sDate.Date, week_days[sDate.WeekDay - 1]);
      LCD_DrawString(60, 20, now_date);

      // 将时间转换为字符串并显示在LCD�?????????
      sprintf(now_time, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
      LCD_DrawString(80, 40, now_time); // 在位�?????????(10, 120)显示时间字符�?????????



    if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
    {
    	if(TIM1->CCR4 == 150)
    	{
        	currentPosition = 163;
        	TIM1->CCR4 = currentPosition;
        	SYN_FrameInfo(0, suoyiguanbi);
    	}

    	//进入蓝色区域
    	if ((strDisplayCoordinate.y > 120) && (strDisplayCoordinate.y < 230) && (strDisplayCoordinate.x > 150) && (strDisplayCoordinate.x < 230))
    	{
    		HAL_Delay(50);
    		DrawNFCMenu(); // NFC解锁界面

    		seconds = 0;
    		returnTomain = 0;


    		while(1)
    		{
                if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
    			{
                	seconds = 0;
                      if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60))
                      {
                        if ((strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                        {
                          DrawMainMenu(); // 重新绘制母循环界�????????????????????????????????
                          break; // �????????????????????????????????出子循环，回到母循环
                        }
                      }
                      if ((strDisplayCoordinate.y > 190) && (strDisplayCoordinate.y < 310) && (strDisplayCoordinate.x > 10) && (strDisplayCoordinate.x < 110))
                      {
                        	 Draw_adding_NFC();
                        	 while(1)
                        	 {
                                 sprintf(left_scond, "%lu  ", (30 - seconds));
                                 LCD_DrawString(80, 47, "left time:");
                                 LCD_DrawString(170, 47, left_scond);

                                 if (seconds >= 30) {
                        			 seconds = 0;
                                	 DrawNFCMenu(); // NFC解锁界面
                                     break; // �????????????????????????????????出子循环，回到母循环
                                 }

                        		 if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
                        		 {
                                     if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                                     {
                            			 seconds = 0;
                                    	 DrawNFCMenu(); // NFC解锁界面
                                         break; // �????????????????????????????????出子循环，回到母循环
                                     }
                        		 }
                        		 adding_nfc_card(&pn532, buffer, uid);
                        		 if (returnToNFCMenu == 1)
                        		 {
                        			 seconds = 0;
                                	 DrawNFCMenu(); // NFC解锁界面
                                	 returnToNFCMenu = 0;
                                     break; // �????????????????????????????????出子循环，回到母循环
                        		 }
                        	 }
                        }
                      ///////////////////////////////////////
                      if ((strDisplayCoordinate.y > 260) && (strDisplayCoordinate.y < 310) && (strDisplayCoordinate.x > 120) && (strDisplayCoordinate.x < 220))
                      {
                     	 Draw_delete_NFC();
                     	 seconds = 0;
                     	 while(1)
                     	 {
                              sprintf(left_scond, "%lu  ", (30 - seconds));
                              LCD_DrawString(80, 47, "left time:");
                              LCD_DrawString(170, 47, left_scond);

                              if (seconds >= 30) {
                     			 seconds = 0;
                             	 DrawNFCMenu(); // NFC解锁界面
                                  break; // �????????????????????????????????出子循环，回到母循环
                              }

                     		 if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
                     		 {
                                  if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                                  {
                                 	 DrawNFCMenu(); // NFC解锁界面
                                 	 seconds = 0;
                                      break; // �????????????????????????????????出子循环，回到母循环
                                  }
                     		 }
                     		delete_nfc_card_libr(&pn532, buffer, uid);
                     		 if (returnToNFCMenu == 1)
                     		 {
                             	 DrawNFCMenu(); // NFC解锁界面
                             	 returnToNFCMenu = 0;
                             	 seconds = 0;
                                  break; // �????????????????????????????????出子循环，回到母循环
                     		 }
                     	 }
                      }
                      ///////////////////////////////////////////
                      if ((strDisplayCoordinate.y > 190) && (strDisplayCoordinate.y < 240) && (strDisplayCoordinate.x > 120) && (strDisplayCoordinate.x < 220))
                      {
                        	 Draw_delete_NFC();
                        	 seconds = 0;
                        	 while(1)
                        	 {
                                 sprintf(left_scond, "%lu  ", (30 - seconds));
                                 LCD_DrawString(80, 47, "left time:");
                                 LCD_DrawString(170, 47, left_scond);

                                 if (seconds >= 30) {
                        			 seconds = 0;
                                	 DrawNFCMenu(); // NFC解锁界面
                                     break; // �????????????????????????????????出子循环，回到母循环
                                 }

                        		 if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
                        		 {
                                     if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                                     {
                                    	 DrawNFCMenu(); // NFC解锁界面
                                    	 seconds = 0;
                                         break; // �????????????????????????????????出子循环，回到母循环
                                     }
                        		 }
                        		 delete_nfc_card(&pn532, buffer, uid);
                        		 if (returnToNFCMenu == 1)
                        		 {
                                	 DrawNFCMenu(); // NFC解锁界面
                                	 returnToNFCMenu = 0;
                                	 seconds = 0;
                                     break; // �????????????????????????????????出子循环，回到母循环
                        		 }
                        	 }
                        }

    			}
                CheckUIDAndControlLock(&pn532, buffer, uid, &currentPosition);//scan nfc uid

                /////////////////////////////////

                sprintf(left_scond, "%lu  ", (30-seconds));
                LCD_DrawString(80,30,"left time:");
                LCD_DrawString(170, 30, left_scond);

                if (seconds >= 30)
                {
                DrawMainMenu();
                 break; // �????????????????????????????????出子循环，回到母循环
                }
                ///////////////////////////////////

                if(nfc_time == 5)
                {
      	    	  LCD_Clear(0, 150, 240, 80, RED);
      	    	  LCD_DrawString(10, 160, "Trial and error reached five");
      	    	  LCD_DrawString(10, 180, "Please try again later.");
      	    	  seconds = 0;
      	    	  while (seconds < 30)
      	    	  {
      	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    	    		SYN_FrameInfo(0, cuowu);
        	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
      	    		sprintf(left_scond, "%lu  ", (30-seconds));
      	            LCD_DrawString(10,200,"stop time:");
      	            LCD_DrawString(110, 200, left_scond);
      	    	  }
      	    	  seconds = 0;
      	    	  DrawNFCMenu(); // NFC解锁界面
      	    	  nfc_time++;
      	    	returnToNFCMenu = 0;
                }
                if (nfc_time == 10)
                {
                    LCD_Clear(0, 0, 240, 320, GREY);
                    LCD_Clear(0, 0, 240, 80, RED);
                    LCD_DrawString(10, 20, "error time reached maximum");
                    LCD_DrawString(10, 40, "Please find the manager");

                    LCD_DrawString(0, 90, "Please press your finger:");
                    LCD_DrawLine(0, 180, 200, 180, BLACK);
                    LCD_DrawString(0, 190, "OR enter manager password:");
                    LCD_DrawString(0, 210, "manager password:");
                    HAL_Delay(1000);

                    position = 150;
                    testpassword = 0;

                    while (nfc_time != 0)
                    {
        	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        	    		SYN_FrameInfo(0, jinggao);
        	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
                        char keyCH = Get_Key_Ch();

                        HAL_Delay(100);
                        if (keyCH >= '0' && keyCH <= '9') // 打印数字并计算密�?
                        {
                            LCD_DrawChar(position, 210, '*');
                            HAL_Delay(200);
                            position += 9;
                            testpassword = testpassword * 10 + (keyCH - '0');
                            if (position == 186)
                            {
                                position = 150;
                                if (testpassword == managerpassword)
                                {
                                    LCD_DrawString(10, 230, "THE PASSWORD IS CORRECT");
                                    HAL_Delay(2000);
                                    nfc_time = 0;
                                }
                                else
                                {
                                    LCD_DrawString(10, 230, "THE PASSWORD IS WRONG");
                                    HAL_Delay(2000);
                                    LCD_Clear(0, 230, 240, 16, GREY);
                                }
                                LCD_Clear(150, 210, 36, 16, GREY);
                                testpassword = 0;
                            }
                        }
                        if (keyCH == 'A') // 减少�?个数�?
                        {
                            if (position == 150) continue;
                            position -= 9;
                            LCD_Clear(position, 210, 8, 16, GREY);
                            testpassword /= 10;
                        }
                        if (keyCH == 'C') // 清除数字
                        {
                            position = 150;
                            testpassword = 0;
                            LCD_Clear(150, 210, 36, 16, GREY);
                        }

                        uint8_t ensure;
                        SearchResult search;
                        ensure = PS_GetImage();
                        if (ensure == 0x00) // 获取图像成功，表示有手指按下
                        {
                            ensure = PS_GenChar(CharBuffer1);
                            if (ensure == 0x00) // 生成特征成功
                            {
                                ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &search);
                                if (ensure == 0x00 && search.pageID == 0) // 搜索成功且匹配管理员ID
                                {
                                    LCD_DrawString(0, 110, "Admin verified");
                                    nfc_time = 0;
                                    HAL_Delay(2000);
                                }
                                else
                                {
                                    LCD_DrawString(0, 110, "Verification failed");
                                    HAL_Delay(2000);
                                    LCD_Clear(0, 110, 240, 16, GREY);
                                }
                            }
                        }
                        else
                        {
                            LCD_Clear(0, 110, 240, 16, GREY);
                            LCD_DrawString(0, 110, "No finger detected");
                            LCD_DrawLine(0, 128, 150, 128, BLACK);
                            HAL_Delay(1500);
                        }
                    }
                    LCD_Clear(0, 0, 240, 360, GREY);
                    LCD_Clear(40, 70, 200, 60, GREY);
                    LCD_Clear(40, 70, 180, 60, GREEN);
                    LCD_DrawString(50, 80, "SYSTEM BACK TO NORMAL");
        	    	SYN_FrameInfo(0, jingbaojiechuy);
                    returnToNFCMenu = 1;
                    DrawMainMenu();
                    HAL_Delay(500);
                }

          		 if (returnTomain == 1)
          		 {
                    DrawMainMenu();
                    returnToNFCMenu = 0;
                    break; // �????????????????????????????????出子循环，回到母循环
          		 }
            }
          }



    	//进入指纹区域
    	if ((strDisplayCoordinate.y > 240) && (strDisplayCoordinate.y < 320) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 110))
    	{
    		returnTomain = 0;
    		HAL_Delay(50);
    		Draw_FINGER_Menu();
    		while (PS_HandShake(&AS608Addr))
    		{
    			LCD_DrawString(0, 160, "AS608 NO NO");
    		}

    		seconds = 0;
    		while(1)
    		{
                sprintf(left_scond, "%lu  ", (30-seconds));
                LCD_DrawString(80,30,"left time:");
                LCD_DrawString(170, 30, left_scond);

                if (seconds >= 30)
                {
                DrawMainMenu();
                 break; // �????????????????????????????????出子循环，回到母循环
                }
            	if(fingerprintLibrary[0] == 0)
            	{
            		Add_FR();		//管理者指�?
            	}

            if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
            {
            	seconds = 0;
              if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60))
              {
                if ((strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                {
                  DrawMainMenu(); // 重新绘制母循环界�????????????????????????????????
                  break; // �????????????????????????????????出子循环，回到母循环
                }
              }
              if ((strDisplayCoordinate.y > 190) && (strDisplayCoordinate.y < 310) && (strDisplayCoordinate.x > 10) && (strDisplayCoordinate.x < 110))
              {
            	  add_finger_Menu();
            	  add_finger();
              }
              if ((strDisplayCoordinate.y > 190) && (strDisplayCoordinate.y < 240) && (strDisplayCoordinate.x > 120) && (strDisplayCoordinate.x < 220))
              {
            	  delete_finger_Menu();
            	  Del_FR();
              }
              if ((strDisplayCoordinate.y > 260) && (strDisplayCoordinate.y < 310) && (strDisplayCoordinate.x > 120) && (strDisplayCoordinate.x < 220))
              {
            	  delete_finger_Menu();
            	  Del_FR_Lib();
              }
            }
            press_FR();
      		 if (returnTomain == 1)
      		 {
                DrawMainMenu();
                returnToNFCMenu = 0;
                break; // �????????????????????????????????出子循环，回到母循环
      		 }
            if (finger_time == 5)
            {
            	  LCD_Clear(0, 150, 240, 80, RED);
            	  LCD_DrawString(10, 160, "Trial and error reached five");
            	  LCD_DrawString(10, 180, "Please try again later.");
            	  seconds = 0;
            	while (seconds < 30)
            	{
    	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    	    		SYN_FrameInfo(0, cuowu);
        	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
            		sprintf(left_scond, "%lu  ", (30-seconds));
                    LCD_DrawString(10,200,"stop time:");
                    LCD_DrawString(110, 200, left_scond);
            	}
            	seconds = 0;
            	Draw_FINGER_Menu();
            	finger_time++;
            }
            if (finger_time == 10)
            {
    	    	LCD_Clear(0, 0, 240, 320, GREY);
    	    	LCD_Clear(0, 0, 240, 80, RED);
    	    	LCD_DrawString(10, 20, "error time reached maximum");
    	    	LCD_DrawString(10, 40, "Please find the manager");

    	    	LCD_DrawString(0, 90, "Please scan your NFC card:");
    	    	LCD_DrawLine(0, 180, 200, 180, BLACK);
    	    	LCD_DrawString(0, 190, "OR enter your manager password:");
    	    	LCD_DrawString(0, 210, "manager password:");

                position = 150;
                testpassword = 0;

    	    	while (finger_time != 0)
    	    	{

    	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    	    		SYN_FrameInfo(0, jinggao);
    	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

    	    		uint8_t uid[7]; // 假设 UID 长度�? 7
    	    		int32_t uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
    	    		if (uid_len != PN532_STATUS_ERROR)
    	    		{
    	    		    memset(buffer, 0, 50);
    	    		    char stringForHex[5];
    	    		    for (uint8_t i = 0; i < uid_len; i++)
    	    		    {
    	    		        memset(stringForHex, 0, sizeof(stringForHex));
    	    		        #ifdef use_HEXADECIMAL
    	    		        sprintf(stringForHex, "%02X ", (unsigned char)uid[i]);
    	    		        #else
    	    		        sprintf(stringForHex, "%d ", (unsigned char)uid[i]);
    	    		        #endif
    	    		        strcat(buffer, stringForHex);
    	    		    }
    	    		    buffer[strlen(buffer) - 1] = '\0'; // 去掉�?后一个空�?
    	    		    LCD_DrawString(0, 110,"UID:"); // 显示UID
    	    		    LCD_DrawString(60, 110, buffer); // 显示UID

    	    		    // �?查读取到的UID是否与第�?个允许的UID匹配
    	    		    int isAllowed = 0;
    	    		    if (NFC_KEYS[0] != NULL && string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
    	    		    {
    	    		        isAllowed = 1;
    	    		    }

    	    		    // 根据 isAllowed 的�?�执行相应操�?
    	    		    if (isAllowed)
    	    		    {
    	    		    	LCD_DrawString(0, 130,"manager card"); //验证成功
    	    		    	finger_time = 0;
    	    		    	HAL_Delay(2000);
    	    		    }
    	    		    else
    	    		    {
    	    		    	LCD_DrawString(0, 130,"strange card");
    	    		    	LCD_Clear(0,110,240,40,GREY);
    	    		    }
    	    		} //end nfc

            	    char keyCH = Get_Key_Ch();

            	    HAL_Delay(100); // Debounce delay
            	    if (keyCH >= '0' && keyCH <='9') //print number and calculate the password
            	    {
            	    	LCD_DrawChar(position, 210, '*');
            	    	HAL_Delay(200);
            	        position = position + 9;
            	        testpassword = testpassword*10 + (keyCH-'0');
            	        if(position == 186)
            	        {
            	        	position = 150;
                	        if (testpassword == managerpassword)
                	        {
                            LCD_DrawString(10, 230, "THE PASSWORD IS CORRECT");
                	        HAL_Delay(2000);
                            finger_time = 0;
                	        }
                	        else
                	        {
                	        	LCD_DrawString(10, 230, "THE PASSWORD IS WRONG");
                	        	HAL_Delay(2000);
                	        	LCD_Clear(0, 230, 240, 16, GREY);
                	        }
            	        	LCD_Clear(150, 210, 36, 16, GREY);
            	        	testpassword = 0;
            	        }
            	    }
            	    if (keyCH == 'A') //decrease one number
            	    {
            	    	if (position == 150) continue;
            	    	position = position - 9;
            	    	LCD_Clear(position, 210, 8, 16, GREY);
            	    	testpassword = testpassword/10;
            	    }
            	    if (keyCH == 'C') // CLEAR NUMBER
            	    {
            	    	position = 150;
            	    	testpassword = 0000;
            	    	LCD_Clear(150, 210, 36, 16, GREY);
            	    }

    	    	}
    	    	LCD_Clear(0, 0, 240, 360, GREY);
    	    	LCD_Clear(40, 70, 180, 60, GREEN);
    	    	LCD_DrawString(50, 80, "SYSTEM BACK TO NORMAL");
    	    	SYN_FrameInfo(0, jingbaojiechuy);
                DrawMainMenu();
                 break;
            }
    	}
    	}


      // 判断是否在蓝数字密码
      if ((strDisplayCoordinate.y > 150) && (strDisplayCoordinate.y < 230) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 110))
      {
    	  HAL_Delay(50);
          DrawSubMenu(); // 进入子循环前绘制子菜�????????????????????????????????
          position = 80;
          seconds = 0;
          testpassword = 0;
          while (1)
          {
              sprintf(left_scond, "%lu  ", (30-seconds));
              LCD_DrawString(80,30,"left time:");
              LCD_DrawString(170, 30, left_scond);

              if (seconds >= 30)
              {
              DrawMainMenu();
               break; // �????????????????????????????????出子循环，回到母循环
              }

        	    char keyCH = Get_Key_Ch();
        	    HAL_Delay(200); // Debounce delay
        	    if (keyCH >= '0' && keyCH <='9') //print number and calculate the password
        	    {
        	    	seconds = 0;
        	    	LCD_DrawChar(position, 120, keyCH);
        	    	HAL_Delay(100);
        	        LCD_DrawChar(position, 120, '*');
        	        position = position + 9;
        	        testpassword = testpassword*10 + (keyCH-'0');
        	        if(position == 107+9)
        	        {
        	        	position = 80;
            	        if (testpassword == oldpassword)
            	        {
            	        	test_time = 0;
            	        	LCD_Clear(40, 150, 200, 70, GREEN);
            	        	LCD_DrawString(50, 170, "THE LOCK IS OPEN");
            	        	currentPosition = 150;
            	        	TIM1->CCR4 = currentPosition;
            	        	SYN_FrameInfo(0, suoyidakai);
            	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
            	        	HAL_Delay(3000);		//�??????????????????????????门延�??????????????????????????
            	        	LCD_Clear(40, 150, 200, 70, GREY);
            	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
            	              DrawMainMenu();
            	               break;
            	        }
            	        else
            	        {
            	        	test_time++;
            	        	LCD_Clear(40, 150, 200, 70, RED);
            	        	LCD_DrawString(50, 170, "THE PASSWORD IS WRONG");
            	        	SYN_FrameInfo(0, mimacuowu);
            	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
            	        	currentPosition = 1603;
            	        	TIM1->CCR4 = currentPosition;
            	        	HAL_Delay(3000);		//关门延迟
            	        	LCD_Clear(40, 150, 200, 70, GREY);
            	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

            	        }
        	        	LCD_Clear(80, 120, 36, 16, GREY);
        	        	testpassword = 0;
        	        }
        	    }
        	    if (keyCH == 'A') //decrease one number
        	    {
        	    	seconds = 0;
        	    	if (position == 80) continue;
        	    	position = position - 9;
        	    	LCD_Clear(position, 120, 8, 16, GREY);
        	    	testpassword = testpassword/10;
        	    }
        	    if (keyCH == 'C') // CLEAR NUMBER
        	    {
        	    	seconds = 0;
        	    	position = 80;
        	    	testpassword = 0000;
        	    	LCD_Clear(80, 120, 36, 16, GREY);
        	    }
        	    if (keyCH == 'D')
        	    {
        	    	seconds = 0;
        	        ResetPassword();
        	        testpassword = 0000;
        	        position = 80;
        	    }
        	    if (test_time == 5)
        	    {
        	    	  LCD_Clear(0, 150, 240, 80, RED);
        	    	  LCD_DrawString(10, 160, "Trial and error reached five");
        	    	  LCD_DrawString(10, 180, "Please try again later.");
        	    	  seconds = 0;
        	    	while (seconds < 30)
        	    	{
        	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        	        	SYN_FrameInfo(0, cuowu);
        	        	//HAL_Delay(1000);
        	            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        	    		sprintf(left_scond, "%lu  ", (30-seconds));
        	            LCD_DrawString(10,200,"stop time:");
        	            LCD_DrawString(110, 200, left_scond);

        	    	}
        	    	seconds = 0;
        	    	DrawSubMenu();
        	    	test_time++;
        	    }
        	    if (test_time == 10)
        	    {
        	    	LCD_Clear(0, 0, 240, 320, GREY);
        	    	LCD_Clear(0, 0, 240, 80, RED);
        	    	LCD_DrawString(10, 20, "error time reached maximum");
        	    	LCD_DrawString(10, 40, "Please find the manager");
        	    	LCD_DrawString(10, 40, "Please find the manager");

        	    	LCD_DrawString(0, 90, "Please scan your card:");
        	    	LCD_DrawLine(0, 180, 200, 180, BLACK);
        	    	LCD_DrawString(0, 190, "OR press your finger:");

        	    	while (test_time != 0)
        	    	{
        	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        	    		SYN_FrameInfo(0, jinggao);
        	    		//HAL_Delay(100);
        	    		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        	    		uint8_t uid[7]; // 假设 UID 长度�? 7
        	    		int32_t uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
        	    		if (uid_len != PN532_STATUS_ERROR)
        	    		{
        	    		    memset(buffer, 0, 50);
        	    		    char stringForHex[5];
        	    		    for (uint8_t i = 0; i < uid_len; i++)
        	    		    {
        	    		        memset(stringForHex, 0, sizeof(stringForHex));
        	    		        #ifdef use_HEXADECIMAL
        	    		        sprintf(stringForHex, "%02X ", (unsigned char)uid[i]);
        	    		        #else
        	    		        sprintf(stringForHex, "%d ", (unsigned char)uid[i]);
        	    		        #endif
        	    		        strcat(buffer, stringForHex);
        	    		    }
        	    		    buffer[strlen(buffer) - 1] = '\0'; // 去掉�?后一个空�?
        	    		    LCD_DrawString(0, 110,"UID:"); // 显示UID
        	    		    LCD_DrawString(60, 110, buffer); // 显示UID

        	    		    // �?查读取到的UID是否与第�?个允许的UID匹配
        	    		    int isAllowed = 0;
        	    		    if (NFC_KEYS[0] != NULL && string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
        	    		    {
        	    		        isAllowed = 1;
        	    		    }

        	    		    // 根据 isAllowed 的�?�执行相应操�?
        	    		    if (isAllowed)
        	    		    {
        	    		    	LCD_DrawString(0, 130,"manager card"); //验证成功
        	    		    	test_time = 0;
        	    		    	HAL_Delay(2000);
        	    		    }
        	    		    else
        	    		    {
        	    		    	LCD_DrawString(0, 130,"strange card");
        	    		    	LCD_Clear(0,110,240,40,GREY);
        	    		    }
        	    		} //end nfc

        	    	    uint8_t ensure;
        	    	    SearchResult search;
        	            ensure = PS_GetImage();
        	            if (ensure == 0x00) // 获取图像成功，表示有手指按下
        	            {
        	                ensure = PS_GenChar(CharBuffer1);
        	                if (ensure == 0x00) // 生成特征成功
        	                {
        	                    ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &search);
        	                    if (ensure == 0x00 && search.pageID == 0) // 搜索成功且匹配管理员ID
        	                    {
        	                        LCD_DrawString(0, 230, "Admin verified");
        	                        test_time = 0;
        	                        HAL_Delay(2000);
        	                    }
        	                    else
        	                    {
        	                        LCD_DrawString(0, 230, "Verification failed");
        	                        HAL_Delay(2000);
        	                        LCD_Clear(0,230,240,16,GREY);
        	                    }
        	                }

        	            }
        	            else
        	            {
        	                LCD_Clear(0, 210, 240, 16, GREY);
        	                LCD_DrawString(0, 210, "No finger detected");
        	                LCD_DrawLine(0, 228, 150, 228, BLACK);
        	                HAL_Delay(1500);
        	            }

        	    	}
        	    	LCD_Clear(0, 0, 240, 360, GREY);
        	    	LCD_Clear(40, 70, 200, 60, GREY);
        	    	LCD_Clear(40, 70, 180, 60, GREEN);
        	    	LCD_DrawString(50, 80, "Alarm deactivated.");
        	    	SYN_FrameInfo(0, jingbaojiechuy);
        	    	//HAL_Delay(2000);
                    DrawMainMenu();
                     break;
        	    }



              if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
              {
            	  seconds = 0;
                  if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                      (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
                  {
                      DrawMainMenu(); // Redraw main menu
                      break; // Exit to main loop
                  }

              }

          }

      }

      if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60) && (strDisplayCoordinate.x > 180) && (strDisplayCoordinate.x < 240))
      {
          HAL_Delay(500);
    	  if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
			{
                if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60) && (strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                  {

    	  testpassword = 0;
    	  position = 80;
    	  HAL_Delay(50);
    	  DrawmanagerMenu();
    	  seconds = 0;
    	  enter = 0;
    	  LCD_Clear(180,260,60,60,GREY);
    	  SYN_FrameInfo(0, jinruguanliyemian);
    	  while(1)
    	  {

        	  if (enter == -1)
        	  {
                  DrawMainMenu(); // Redraw main menu
                  SYN_FrameInfo(0, tuichuguanliyemian);
                  //HAL_Delay(200);
                  break; // Exit to main loop
        	  }
              sprintf(left_scond, "%lu  ", (30-seconds));
              LCD_DrawString(80,30,"left time:");
              LCD_DrawString(170, 30, left_scond);

              if (seconds >= 30)
              {
            	enter = -1;
              //DrawMainMenu();
               //break; // �????????????????????????????????出子循环，回到母循环
              }
              if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
              {
              if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                  (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
              {
            	  enter = -1;
                  //DrawMainMenu(); // Redraw main menu
                  //break; // Exit to main loop
              }
              }

      	    char keyCH = Get_Key_Ch();
      	    HAL_Delay(200); // Debounce delay
      	    if (keyCH >= '0' && keyCH <='9') //print number and calculate the password
      	    {
      	    	seconds = 0;
      	    	LCD_DrawChar(position, 120, keyCH);
      	    	HAL_Delay(100);
      	        LCD_DrawChar(position, 120, '*');
      	        position = position + 9;
      	        testpassword = testpassword*10 + (keyCH-'0');
      	        if(position == 107+9)
      	        {
      	        	position = 80;
          	        if (testpassword == managerpassword)
          	        {
          	        	LCD_Clear(40, 150, 200, 70, GREEN);
          	        	LCD_DrawString(50, 170, "ENTER MANAGER MODE");
          	        	HAL_Delay(3000);
          	        	LCD_Clear(40, 150, 200, 70, GREY);
          	        	enter = 1;
          	        }
          	        else
          	        {
          	        	LCD_Clear(40, 150, 200, 70, RED);
          	        	LCD_DrawString(50, 170, "THE PASSWORD IS WRONG");
          	        	HAL_Delay(3000);		//关门延迟
          	        	LCD_Clear(40, 150, 200, 70, GREY);

          	        }
      	        	LCD_Clear(80, 120, 36, 16, GREY);
      	        	testpassword = 0;
      	        }
      	    }
      	    if (keyCH == 'A') //decrease one number
      	    {
      	    	seconds = 0;
      	    	if (position == 80) continue;
      	    	position = position - 9;
      	    	LCD_Clear(position, 120, 8, 16, GREY);
      	    	testpassword = testpassword/10;
      	    }
      	    if (keyCH == 'C') // CLEAR NUMBER
      	    {
      	    	seconds = 0;
      	    	position = 80;
      	    	testpassword = 0000;
      	    	LCD_Clear(80, 120, 36, 16, GREY);
      	    }

      	    //////////////////////////////////////"确认进去管理员模�?"//////////////////////////
      	    while (enter == 1)
      	    {
      	    	ResetPassword();
      	    }
      	  if (enter == 2) {
      	      LCD_Clear(0,0,240,320,GREY);
      	    LCD_Clear(0, 0, 60, 60, BLUE);
      	    LCD_DrawString(10, 20, "BACK");
      	    LCD_DrawString(80,10,"Change NFC CARD");
      	    LCD_DrawLine(80, 28, 200, 28, BLACK);
      	      LCD_DrawString(0, 80, "PLEASE SCAN  NEW MANAGER CARD");
      	      LCD_DrawLine(0, 98, 150, 98, BLACK);
      	      LCD_Clear(180,260,60,60,BLUE);
      	    LCD_DrawString(190, 270, "NEXT");
      	      seconds = 0;
      	  }

      	  while (enter == 2)
      	  {
      	      sprintf(left_scond, "%lu  ", (30 - seconds));
      	      LCD_DrawString(80, 30, "left time:");
      	      LCD_DrawString(170, 30, left_scond);

      	      if (seconds >= 30)
      	      {
      	          DrawMainMenu();
      	          enter = -1;
      	      }

      	      if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
      	      {
      	          if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
      	              (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60)) {
      	              DrawMainMenu(); // 触摸特定区域后返回主菜单
      	              enter = -1;
      	          }
      	          if ((strDisplayCoordinate.y > 260 && strDisplayCoordinate.y < 320) &&
      	              (strDisplayCoordinate.x > 180 && strDisplayCoordinate.x < 240)) {
      	              enter++;
      	          }
      	      }
      	    replace_manager_card(&pn532, buffer, uid);

    	  }


      	    while (enter == 3)
      	    {
      	    	fingerprintLibrary[0] = 0;
      	    	currentFingerprintCount--;
      	    	Add_FR();
      	    	if (fingerprintLibrary[0] ==0)
      	    	{
      	    		LCD_Clear(0,120,200,30,RED);
      	    		LCD_DrawString(10, 130, "Manager Fingerprint Reset Fail");
      	    		HAL_Delay(2000);
      	    	}

      	    }


    	  }//EMERGENCY WHILE END
      }

    }

      }
}

  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


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
