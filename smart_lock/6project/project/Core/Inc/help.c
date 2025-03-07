/*
 * help.c
 *
 *  Created on: Nov 8, 2024
 *      Author: 24747
 */

#include "help.h"
#include "xpt2046.h" // 假设你有一个 LCD 库
#include "lcdtp.h"
#include "gpio.h"
#include "mylibrary.h"
#include <stdlib.h>
#include "rtc.h"
#include <stdio.h>
#include "syn6288.h"

extern uint8_t moshengka[];
extern uint8_t suoyidakai[] ;

extern char left_scond[255];
extern int position;

// 声明在 main.c 中定义的全局变量
extern int test_time;
extern int oldpassword;
extern int managerpassword;
extern strType_XPT2046_Coordinate strDisplayCoordinate;
extern strType_XPT2046_TouchPara strXPT2046_TouchPara;


extern char* NFC_KEYS[5]; // Ensure this is defined somewhere
extern uint8_t returnToNFCMenu;
extern int size_NFC;
extern volatile uint32_t seconds;

extern int nfc_time;
extern int returnTomain;
extern int enter;

void DrawMainMenu(void)
{
    LCD_Clear(0, 0, 240, 320, GREY);

    LCD_DrawString(40, 80, "GROUP: 20");
    LCD_DrawString(40, 110, "SMART LOCK");
    LCD_DrawLine(0, 140, 240, 140, BLACK);

    LCD_Clear(0, 240, 110, 80, YELLOW);
    LCD_DrawString(10, 275, "Fingerprint");

    LCD_Clear(120, 240, 110, 80, RED);
    LCD_DrawString(140, 275, "Emergency");

    LCD_Clear(0, 150, 110, 80, BLUE);
    LCD_DrawString(30, 185, "Digital");

    LCD_Clear(120, 150, 110, 80, GREEN);
    LCD_DrawString(160, 185, "NFC");
}

void DrawSubMenu(void) // 密码锁界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(0, 80, "PLEASE ENTER YOUR PASSWORD");
    LCD_DrawLine(0, 98, 150, 98, BLACK);
    LCD_DrawString(0, 120, "PASSWORD : ");
    LCD_DrawLine(0, 138, 150, 138, BLACK);
    LCD_DrawString(80,10,"Digital Mode");
    LCD_DrawLine(80, 28, 200, 28, BLACK);

    LCD_DrawLine(0, 235, 200,235, BLACK);
    LCD_DrawString(0,240,"press A to clear one digital");
    LCD_DrawLine(0, 255, 200,255, BLACK);
    LCD_DrawString(0,260,"press C to change ALL digital");
    LCD_DrawLine(0, 285, 200,285, BLACK);
    LCD_DrawString(0,290,"press D to change password");
}
void DrawmanagerMenu(void)
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(0, 80, "PLEASE ENTER MANAGER PASSWORD");
    LCD_DrawLine(0, 98, 150, 98, BLACK);
    LCD_DrawString(0, 120, "PASSWORD : ");
    LCD_DrawLine(0, 138, 150, 138, BLACK);
    LCD_DrawString(80,10,"Manager Mode");
    LCD_DrawLine(80, 28, 200, 28, BLACK);
      LCD_Clear(180,260,60,60,BLUE);
    LCD_DrawString(190, 270, "NEXT");
}

void DrawNFCMenu(void) // NFC解锁界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(80,10,"NFC Mode");
    LCD_DrawLine(80, 28, 200, 28, BLACK);
    LCD_DrawString(0, 80, "PLEASE SCAN YOUR CARD");

    LCD_Clear(10, 190, 100, 120, BLUE);
    LCD_DrawString(30, 220, "ADD");
    LCD_DrawString(20, 240, "CARD");
    LCD_Clear(120, 190, 100, 50, YELLOW);
    LCD_DrawString(130, 200, "DELETE");
    LCD_DrawString(130, 220, "SINGLE");
    LCD_Clear(120, 260, 100, 50, YELLOW);
    LCD_DrawString(130, 270, "DELETE");
    LCD_DrawString(130, 290, "ALL");
    LCD_DrawLine(120, 250, 240,250, BLACK);
}
void Draw_adding_NFC(void) //
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(70, 20, "adding card menu");

}
void Draw_delete_NFC(void) //
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(70, 20, "Delete card menu");

}

void RESETPASSWORDMenu(void) // 重置密码锁界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(80,10,"Reset password Mode");
    LCD_DrawString(0, 80, "PLEASE ENTER MANAGER PASSWORD");
    LCD_DrawString(0, 100, "MANAGER PASSWORD : ");
    LCD_DrawLine(0, 118, 200, 118, BLACK);
}

void Draw_FINGER_Menu(void) // 指纹解锁界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(80,10,"Fingerprint Mode");
    LCD_DrawLine(80, 28, 200, 28, BLACK);
    LCD_DrawString(0, 80, "PLEASE PUT YOUR FINGER");

    LCD_DrawLine(0, 188, 240,188, BLACK);
    LCD_Clear(10, 190, 100, 120, BLUE);
    LCD_DrawString(30, 220, "ADD");
    LCD_DrawString(20, 240, "FINGERPRINT");
    LCD_Clear(120, 190, 100, 50, YELLOW);
    LCD_DrawString(130, 200, "DELETE");
    LCD_DrawString(130, 220, "SINGLE");
    LCD_Clear(120, 260, 100, 50, YELLOW);
    LCD_DrawString(130, 270, "DELETE");
    LCD_DrawString(130, 290, "ALL");
    LCD_DrawLine(120, 250, 240,250, BLACK);

}
void add_finger_Menu(void) // 加入指纹界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(70, 20, "adding finger menu");
}
void delete_finger_Menu(void) // 删除指纹锁界面
{
    LCD_Clear(0, 0, 240, 320, GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(70, 20, "delete finger menu");
}


void ResetPassword(void) {
    int enteredOldPassword = 0;
    int newPassword = 0;
    int confirmPassword = 0;
    position = 140;
    char keyCH;
    seconds = 0;

    while (1) {
        RESETPASSWORDMenu(); // 显示重置密码界面
        if (enter == 1)
        {
        	LCD_Clear(180,260,60,60,BLUE);
        	LCD_DrawString(190,270,"NEXT");
        }
        seconds = 0;

        // 输入旧密码
        while (1) {
            sprintf(left_scond, "%lu  ", (30 - seconds));
            LCD_DrawString(80, 30, "left time:");
            LCD_DrawString(170, 30, left_scond);

            if (seconds >= 30) {
                DrawSubMenu();
                seconds = 0;
                enter = -1;
                return;
            }

            keyCH = Get_Key_Ch();
            HAL_Delay(200); // 防抖延迟
            if (keyCH >= '0' && keyCH <= '9') {
                seconds = 0;
                LCD_DrawChar(position, 100, '*');
                position += 9;
                enteredOldPassword = enteredOldPassword * 10 + (keyCH - '0');
                if (position == 140+36) {
                    position = 140;
                    if (enteredOldPassword == managerpassword) {
                    	LCD_DrawString(140,100," * * * *  ");
                        LCD_Clear(50, 210, 190, 60, GREEN);
                        LCD_DrawString(60, 220, "PLEASE ENTER");
                        LCD_DrawString(60, 240, " THE NEW PASSWORD");
                        test_time = 0;
                        HAL_Delay(2000);
                        LCD_Clear(50, 210, 190, 80, GREY);
                        LCD_DrawString(0, 120, "PLEASE ENTER THE NEW PASSWORD");
                        LCD_DrawString(0, 140, "NEW PASSWORD : ");
                        LCD_DrawLine(0, 158, 200, 158, BLACK);
                        break; // 旧密码正确，退出循环
                    } else {
                        LCD_Clear(50, 210, 190, 60, RED);
                        LCD_DrawString(60, 220, "THE PASSWORD IS WRONG");
                        test_time++;
                        HAL_Delay(2000);
                        LCD_Clear(50, 210, 190, 70, GREY);
                        enteredOldPassword = 0;
                    }
                    LCD_Clear(140, 100, 36, 16, GREY);
                }
            }
            if (keyCH == 'A') { // 删除一个数字
                seconds = 0;
                if (position == 140) continue;
                position -= 9;
                LCD_Clear(position, 100, 8, 16, GREY);
                enteredOldPassword /= 10;
            }
            if (keyCH == 'C') { // 清除输入
                seconds = 0;
                position = 140;
                enteredOldPassword = 0;
                LCD_Clear(140, 100, 36, 16, GREY);
            }
            if ((test_time == 5 || test_time ==10) && enter != 100 ) {
                DrawSubMenu(); // 返回上一层界面
                return;
            }
            if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara)) {
                seconds = 0;
                if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                    (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60)) {
                    DrawSubMenu(); // 返回上一层界面
                    enter = -1;
                    return;
                }
    	          if ((strDisplayCoordinate.y > 260 && strDisplayCoordinate.y < 320) &&
    	              (strDisplayCoordinate.x > 180 && strDisplayCoordinate.x < 240) && enter == 1) {
    	              enter++;
    	              return;
    	          }
            }
        }

        // 输入新密码
        while (1) {
            seconds = 0;
            position = 140;
            newPassword = 0;
            while (1) {
                sprintf(left_scond, "%lu  ", (30 - seconds));
                LCD_DrawString(80, 30, "left time:");
                LCD_DrawString(170, 30, left_scond);

                if (seconds >= 30) {
                    DrawSubMenu();
                    enter = -1;
                    seconds = 0;
                    return;
                }

                keyCH = Get_Key_Ch();
                HAL_Delay(200); // 防抖延迟
                if (keyCH >= '0' && keyCH <= '9') {
                    seconds = 0;
                    LCD_DrawChar(position, 140, keyCH);
                    position += 9;
                    newPassword = newPassword * 10 + (keyCH - '0');
                    if (position == 140+36) {
                        LCD_Clear(140, 140, 36, 16, GREY);
                        LCD_DrawString(140, 140, "* * * *");
                        LCD_DrawString(0, 160, "PLEASE CONFIRM THE NEW PASSWORD");
                        LCD_DrawString(0, 180, "CONFIRM PASSWORD : ");
                        LCD_DrawLine(0, 198, 200, 198, BLACK);
                        break; // 新密码输入完成
                    }
                }
                if (keyCH == 'A') { // 删除一个数字
                    seconds = 0;
                    if (position == 140) continue;
                    position -= 9;
                    LCD_Clear(position, 140, 8, 16, GREY);
                    newPassword /= 10;
                }
                if (keyCH == 'C') { // 清除输入
                    seconds = 0;
                    position = 140;
                    newPassword = 0;
                    LCD_Clear(140, 140, 36, 16, GREY);
                }
                if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara)) {
                    seconds = 0;
                    if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                        (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60)) {
                        DrawSubMenu(); // 返回上一层界面
                        enter = -1;
                        return;
                    }
      	          if ((strDisplayCoordinate.y > 260 && strDisplayCoordinate.y < 320) &&
      	              (strDisplayCoordinate.x > 180 && strDisplayCoordinate.x < 240) && enter == 1) {
      	              enter++;
      	              return;
      	          }
                }
            }

            // 确认新密码
            position = 140;
            confirmPassword = 0;
            while (1) {
                sprintf(left_scond, "%lu  ", (30 - seconds));
                LCD_DrawString(80, 30, "left time:");
                LCD_DrawString(170, 30, left_scond);

                if (seconds >= 30) {
                    DrawSubMenu();
                    seconds = 0;
                    enter = -1;
                    return;
                }

                keyCH = Get_Key_Ch();
                HAL_Delay(200); // 防抖延迟
                if (keyCH >= '0' && keyCH <= '9') {
                    seconds = 0;
                    LCD_DrawChar(position, 180, '*');
                    position += 9;
                    confirmPassword = confirmPassword * 10 + (keyCH - '0');
                    if (position == 140+36) {
                        if (confirmPassword == newPassword) {
                            oldpassword = newPassword; // 更新密码
                            LCD_Clear(30, 210, 210, 60, GREEN);
                            LCD_DrawString(40, 220, "PASSWORD RESET SUCCESSFUL");
                            enter++;
                            HAL_Delay(2000);
                            LCD_Clear(30, 210, 210, 70, GREY);
                            DrawSubMenu(); // 返回上一层界面
                            return;
                        } else {
                            LCD_Clear(50, 210, 190, 60, RED);
                            LCD_DrawString(60, 220, "PASSWORDS DO NOT MATCH");
                            LCD_Clear(140, 180, 36, 16, GREY);
                            HAL_Delay(2000);
                            LCD_Clear(50, 210, 190, 70, GREY);
                            LCD_Clear(140, 140, 60, 16, GREY);
                            LCD_Clear(0,160,240,180,GREY);
                            LCD_Clear(140,140,40,16,GREY);
                            break; // 重新输入新密码
                        }
                    }
                }
                if (keyCH == 'A') { // 删除一个数字
                    seconds = 0;
                    if (position == 140) continue;
                    position -= 9;
                    LCD_Clear(position, 180, 8, 16, GREY);
                    confirmPassword /= 10;
                }
                if (keyCH == 'C') { // 清除输入
                    seconds = 0;
                    position = 140;
                    confirmPassword = 0;
                    LCD_Clear(140, 180, 36, 16, GREY);
                }
                if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara)) {
                    seconds = 0;
                    if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                        (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60)) {
                        DrawSubMenu(); // 返回上一层界面
                        enter = -1;
                        return;
                    }
      	          if ((strDisplayCoordinate.y > 260 && strDisplayCoordinate.y < 320) &&
      	              (strDisplayCoordinate.x > 180 && strDisplayCoordinate.x < 240) && enter == 1) {
      	              enter++;
      	              return;
      	          }
                }
            }
        }
    }
}

void CheckUIDAndControlLock(PN532* pn532, char* buffer, uint8_t* uid, int* currentPosition)
  {
      int32_t uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
      if (uid_len != PN532_STATUS_ERROR)
      {
          //HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
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
          buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格
          LCD_DrawString(0, 120, buffer); // 显示UID

          // 检查读取到的UID是否在允许的UID列表中
          int isAllowed = 0;
          for (int i = 0; i < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]); i++)
          {
              if (NFC_KEYS[i] != NULL && string_compare(buffer, NFC_KEYS[i], strlen(NFC_KEYS[i])) == 1)
              {
            	  returnTomain = 1;
                  isAllowed = 1;
                  break;
              }
          }

          if (isAllowed)
          {
        	  nfc_time = 0;
        	  seconds = 0;
	        	LCD_Clear(50, 140, 140, 50, GREEN);
	        	LCD_DrawString(60, 160, "The lock is opened!!");
	        	test_time = 0;	//digital

	        	*currentPosition = 150;
	        	TIM1->CCR4 = *currentPosition;
	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	        	SYN_FrameInfo(0, suoyidakai);
	        	//HAL_Delay(2500);		//�?????????门延�?????????
	        	LCD_Clear(50, 140, 140, 50, GREY);
	            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

          }
          else
          {
        	  nfc_time++;
        	  seconds = 0;
        	  LCD_Clear(50, 140, 140, 50, RED);
        	  LCD_DrawString(60, 160, "STRANGER CARD");
              *currentPosition = 163;
              TIM1->CCR4 = *currentPosition; // 关锁
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
              SYN_FrameInfo(0, moshengka);
	        //HAL_Delay(3000);		//�?????????门延�?????????
	        LCD_Clear(50, 140, 140, 50, GREY);
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

          }

          HAL_Delay(1000);
          LCD_Clear(0, 120, 100, 16, GREY);
      }

  }
  void adding_nfc_card(PN532* pn532, char* buffer, uint8_t* uid)
  {
  	LCD_Clear(30, 160, 140, 70, BLUE);
  	LCD_DrawString(40, 170, "please scan manager card");

      int32_t uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
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
          buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格

          // 检查是否为管理者UID
          if (string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
          {

        	  	LCD_Clear(30, 160, 140, 70, YELLOW);
        	  	LCD_DrawString(40, 170, "please scan new card");
        	  	HAL_Delay(1000);

        	  	seconds = 0;
              // 等待扫描新卡
              while (1)
              {
                  sprintf(left_scond, "%lu  ", (30 - seconds));
                  LCD_DrawString(80, 47, "left time:");
                  LCD_DrawString(170, 47, left_scond);

                  if (seconds >= 30) {
                   	returnToNFCMenu = 1;
                        break;
                  }

                  uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
         		 if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
         		 {
                      if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60))
                      {
                        if ((strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                        {
                     	returnToNFCMenu = 1;
                          break;
                        }
                      }
         		 }
                  if (uid_len != PN532_STATUS_ERROR)
                  {
                      memset(buffer, 0, 50);
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
                      buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格

                      // 显示新卡UID
                      LCD_DrawString(0, 220, buffer);
                      if (size_NFC >= sizeof(NFC_KEYS))
                      {
                    	  	LCD_Clear(30, 160, 140, 70, RED);
                    	  	LCD_DrawString(40, 170, "the number of card is full");
                    	  	HAL_Delay(1000);
                            returnToNFCMenu = 1;
                            break;
                      }
                      // 检查新UID是否已存在
                      int exists = 0;
                      for (int i = 0; i < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]); i++)
                      {
                          if (NFC_KEYS[i] != NULL && string_compare(buffer, NFC_KEYS[i], strlen(NFC_KEYS[i])) == 1)
                          {
                              exists = 1;
                              break;
                          }
                      }

                      if (exists)
                      {
                  	  	LCD_Clear(30, 160, 140, 70, RED);
                  	  	LCD_DrawString(40, 170, "this card already added");
                  	  	HAL_Delay(1000);
                          returnToNFCMenu = 1;
                          break;
                      }
                      else
                      {
                          // 添加新UID到NFC_KEYS
                          for (int i = 0; i < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]); i++)
                          {
                              if (NFC_KEYS[i] == NULL)
                              {
                                  NFC_KEYS[i] = strdup(buffer); // 动态分配内存并复制UID

                            	  	LCD_Clear(30, 160, 140, 70, GREEN);
                            	  	LCD_DrawString(40, 170, "new card added successful");
                            	  	size_NFC++;
                            	  	HAL_Delay(1000);
                                  returnToNFCMenu = 1;
                                  break;
                              }
                          }
                      }
                      HAL_Delay(1000);
                      break; // 退出循环
                  }
              }
          }
          else
          {
        	  nfc_time++;
	        	LCD_Clear(30, 160, 140, 70, RED);
	        	LCD_DrawString(40, 170, "this is not manager card");
	        	HAL_Delay(1000);		//关门延迟
	        	LCD_Clear(50, 140, 220, 70, GREY);
	            if (nfc_time == 5|| nfc_time == 10)
	            {
	          	  returnToNFCMenu = 1;
	            }
          }


  }
  }
  void delete_nfc_card(PN532* pn532, char* buffer, uint8_t* uid)
  {
      LCD_Clear(30, 160, 140, 70, RED);
      LCD_DrawString(40, 170, "please scan manager card");

      int32_t uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
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
          buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格

          // 检查是否为管理者UID
          if (string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
          {
              LCD_Clear(30, 160, 140, 70, BLUE);
              LCD_DrawString(40, 170, "please scan card to delete");
              HAL_Delay(1000);

              // 等待扫描要删除的卡
              seconds = 0;
              while (1)
              {
                  sprintf(left_scond, "%lu  ", (30 - seconds));
                  LCD_DrawString(80, 47, "left time:");
                  LCD_DrawString(170, 47, left_scond);

                  if (seconds >= 30) {
         			 seconds = 0;
                     returnToNFCMenu = 1;
                     break;
                  }

                  uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
                  if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
                  {
                      if ((strDisplayCoordinate.y > 0) && (strDisplayCoordinate.y < 60))
                      {
                          if ((strDisplayCoordinate.x > 0) && (strDisplayCoordinate.x < 60))
                          {
                              returnToNFCMenu = 1;
                              break;
                          }
                      }
                  }
                  if (uid_len != PN532_STATUS_ERROR)
                  {
                      memset(buffer, 0, 50);
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
                      buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格

                      // 显示要删除的卡UID
                      LCD_DrawString(0, 220, buffer);

                      // 检查是否为管理者UID
                      if (string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
                      {
                          LCD_Clear(30, 160, 140, 70, RED);
                          LCD_DrawString(40, 170, "cannot delete manager card");
                          HAL_Delay(1000); // 延迟
                          LCD_Clear(50, 140, 220, 70, GREY);
                          returnToNFCMenu = 1;
                          break;
                      }

                      // 检查UID是否存在于NFC_KEYS中
                      int exists = 0;
                      for (int i = 0; i < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]); i++)
                      {
                          if (NFC_KEYS[i] != NULL && string_compare(buffer, NFC_KEYS[i], strlen(NFC_KEYS[i])) == 1)
                          {
                              exists = 1;
                              free(NFC_KEYS[i]); // 释放内存
                              NFC_KEYS[i] = NULL; // 将指针置为NULL

                              // 将数组内容向前移动
                              for (int j = i; j < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]) - 1; j++)
                              {
                                  NFC_KEYS[j] = NFC_KEYS[j + 1];
                              }
                              NFC_KEYS[sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]) - 1] = NULL; // 将最后一个元素置为NULL
                              break;
                          }
                      }

                      if (exists)
                      {
                          LCD_Clear(30, 160, 140, 70, GREEN);
                          LCD_DrawString(40, 170, "the card successfully deleted");
                          HAL_Delay(1000); // 延迟
                          LCD_Clear(50, 140, 220, 70, GREY);
                          size_NFC--; // 减少已存储的UID数量
                          returnToNFCMenu = 1;
                          break;
                      }
                      else
                      {
                          LCD_Clear(30, 160, 140, 70, RED);
                          LCD_DrawString(40, 170, "stranger card");
                          HAL_Delay(1000); // 延迟
                          LCD_Clear(50, 140, 220, 70, GREY);
                          returnToNFCMenu = 1;
                          break;
                      }
                  }
              }
          }
          else
          {
        	  nfc_time++;
              LCD_Clear(30, 160, 140, 70, RED);
              LCD_DrawString(40, 170, "not manager card");
              HAL_Delay(1000); // 延迟
              LCD_Clear(50, 140, 220, 70, GREY);
              if (nfc_time == 5|| nfc_time == 10)
              {
            	  returnToNFCMenu = 1;
              }
          }

      }

  }
  void delete_nfc_card_libr(PN532* pn532, char* buffer, uint8_t* uid)
  {
      LCD_Clear(30, 160, 140, 70, RED);
      LCD_DrawString(40, 170, "please scan manager card");

      int32_t uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
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
          buffer[strlen(buffer) - 1] = '\0'; // 去掉最后一个空格

          // 检查是否为管理者UID
          if (string_compare(buffer, NFC_KEYS[0], strlen(NFC_KEYS[0])) == 1)
          {
              LCD_Clear(30, 180, 140, 70, BLUE);
              LCD_DrawString(30, 190, "deleting all cards except manager");
              HAL_Delay(2000);

              // 删除所有非管理者卡
              for (int i = 1; i < sizeof(NFC_KEYS) / sizeof(NFC_KEYS[0]); i++)
              {
                      free(NFC_KEYS[i]); // 释放内存
                      NFC_KEYS[i] = NULL; // 将指针置为NULL

              }

              size_NFC = 1; // 重置已存储的UID数量，只保留管理者卡

              LCD_Clear(30, 190, 140, 70, GREEN);
              LCD_DrawString(40, 210, "all cards deleted");
              HAL_Delay(2000); // 延迟
              returnToNFCMenu = 1;
          }
          else
          {
              nfc_time++;
              LCD_Clear(30, 170, 140, 70, RED);
              LCD_DrawString(40, 190, "not manager card");
              HAL_Delay(1000); // 延迟
              LCD_Clear(30, 170, 140, 70, GREY);
              if (nfc_time == 5 || nfc_time == 10)
              {
                  returnToNFCMenu = 1;
              }
          }
      }
  }
  void replace_manager_card(PN532* pn532, char* buffer, uint8_t* uid) {
      LCD_Clear(30, 160, 200, 70, RED);
      LCD_DrawString(40, 170, "Please scan new manager card");

      int32_t uid_len = PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
      if (uid_len != PN532_STATUS_ERROR) {
          memset(buffer, 0, 50);
          char stringForHex[5];
          for (uint8_t i = 0; i < uid_len; i++) {
              memset(stringForHex, 0, sizeof(stringForHex));
              #ifdef use_HEXADECIMAL
              sprintf(stringForHex, "%02X ", (unsigned char)uid[i]);
              #else
              sprintf(stringForHex, "%d ", (unsigned char)uid[i]);
              #endif
              strcat(buffer, stringForHex);
          }
          buffer[strlen(buffer) - 1] = '\0';

          // Free the original manager card memory
          if (NFC_KEYS[0] != NULL) {
              free(NFC_KEYS[0]);
              NFC_KEYS[0] = NULL;
          }

          // Dynamically allocate memory for the new UID and store it in NFC_KEYS[0]
          NFC_KEYS[0] = strdup(buffer);
          if (NFC_KEYS[0] == NULL) {
              LCD_Clear(30, 160, 200, 70, RED);
              LCD_DrawString(40, 170, "Memory allocation failed");
              HAL_Delay(1000);
              return;
          }

          LCD_Clear(30, 160, 140, 70, GREEN);
          LCD_DrawString(40, 170, "Manager card replaced successfully");
          enter++;
          HAL_Delay(1000);
      } else {
          LCD_Clear(30, 160, 140, 70, RED);
          LCD_DrawString(40, 170, "Please scan new manager card");
          HAL_Delay(1000);
      }
  }
