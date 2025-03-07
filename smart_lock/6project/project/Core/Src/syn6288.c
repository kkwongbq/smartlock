#include "syn6288.h"
#include <string.h>
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;
extern uint8_t SYS6288Rx; // 语音模块接收缓冲�??????????????
uint8_t suoyidakai[] = {0xCB, 0xF8, 0xD2, 0xD1, 0xB4, 0xF2, 0xBF, 0xAA, '\0'}; // 确保以 \0 结尾
uint8_t suoyiguanbi[] = {0xCB, 0xF8, 0xD2, 0xD1, 0xB9, 0xD8, 0xB1, 0xD5, '\0'}; //锁已经关闭

uint8_t mimacuowu[] = {0xC3, 0xDC, 0xC2, 0xEB, 0xB4, 0xED, 0xCE, 0xF3, '\0'}; // 密码错误
uint8_t moshengka[] = {0x4E, 0x46, 0x43, 0xB4, 0xED, 0xCE, 0xF3, '\0'}; // 陌生卡
uint8_t zhiwencuowu[] = {0xD6, 0xB8, 0xCE, 0xC6, 0xB4, 0xED, 0xCE, 0xF3, '\0'}; // 指纹错误
uint8_t jinruguanliyemian[] = {0xBD, 0xF8, 0xC8, 0xEB, 0xB9, 0xDC, 0xC0, 0xED, 0xD2, 0xB3, 0xC3, 0xE6, '\0'}; // 进入管理员界面
uint8_t tuichuguanliyemian[] = {0xCD, 0xCB, 0xB3, 0xF6, 0xB9, 0xDC, 0xC0, 0xED, 0xD2, 0xB3, 0xC3, 0xE6, '\0'}; // 退出管理员界面
uint8_t cuowu[] = {0xB4, 0xED, 0xCE, 0xF3, '\0'}; // 错误
uint8_t jinggao[] = {0xBE, 0xAF, 0xB1, 0xA8, '\0'}; // 警告
uint8_t jingbaojiechuy[] = {0xBE, 0xAF, 0xB1, 0xA8, 0xBD, 0xE2, 0xB3, 0xFD, '\0'}; // 警报解除


void ClearBuffer()
{
    __HAL_UART_FLUSH_DRREGISTER(&huart2);
}

void SendStr5(unsigned char* buf, unsigned int len)
{
    if (HAL_UART_Transmit(&huart2, buf, len, 1000) != HAL_OK)
    {
        // Error handling
        printf("UART Transmit Error\n");
    }
}

void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata)
{
    unsigned char Frame_Info[50];
    unsigned char HZ_Length;
    unsigned char ecc = 0;
    unsigned int i = 0;
    HZ_Length = strlen((char*)HZdata);

    if (HZ_Length > 45) // Ensure HZdata length does not exceed Frame_Info buffer size
    {
        printf("Error: HZdata length exceeds buffer size\n");
        return;
    }

    Frame_Info[0] = 0xFD;
    Frame_Info[1] = 0x00;
    Frame_Info[2] = HZ_Length + 3;
    Frame_Info[3] = 0x01;
    Frame_Info[4] = 0x01 | (Music << 4);

    for (i = 0; i < 5; i++)
    {
        ecc = ecc ^ (Frame_Info[i]);
    }

    for (i = 0; i < HZ_Length; i++)
    {
        ecc = ecc ^ (HZdata[i]);
    }

    memcpy(&Frame_Info[5], HZdata, HZ_Length);
    Frame_Info[5 + HZ_Length] = ecc;

    ClearBuffer();

    SendStr5(Frame_Info, 5 + HZ_Length + 1);

    printf("Sent Frame: ");
    for (i = 0; i < 5 + HZ_Length + 1; i++)
    {
        printf("%02X ", Frame_Info[i]);
    }
    printf("\n");

    HAL_Delay(2000);
}

void YS_SYN_Set(uint8_t *Info_data)
{
    uint8_t Com_Len;
    Com_Len = strlen((char*)Info_data);
    SendStr5(Info_data, Com_Len);
}
