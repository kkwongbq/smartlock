/*
 * help.h
 *
 *  Created on: Nov 8, 2024
 *      Author: 24747
 */

#ifndef HELP_H
#define HELP_H

#include "pn532_stm32f1.h"
#include "lcdtp.h"
#include "xpt2046.h"
#include <stdint.h>

//#define MIFARE_UID_MAX_LENGTH 10
#define use_HEXADECIMAL

void DrawMainMenu(void);
void DrawSubMenu(void);
void DrawmanagerMenu(void);
void RESETPASSWORDMenu(void);
void ResetPassword(void);
void DrawNFCMenu(void); // NFC解锁界面
void Draw_adding_NFC(void);
void Draw_delete_NFC(void);
void add_finger_Menu(void);
void delete_finger_Menu(void);
void Draw_FINGER_Menu(void);

void adding_nfc_card(PN532* pn532, char* buffer, uint8_t* uid);
void CheckUIDAndControlLock(PN532* pn532, char* buffer, uint8_t* uid, int* currentPosition);
void delete_nfc_card(PN532* pn532, char* buffer, uint8_t* uid);
void delete_nfc_card_libr(PN532* pn532, char* buffer, uint8_t* uid);
void replace_manager_card(PN532* pn532, char* buffer, uint8_t* uid);


#endif // HELP_H
