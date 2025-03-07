#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "as608.h"
#include "help.h"
#include "xpt2046.h"
#include "gpio.h"
#include "tim.h"
#include <stdlib.h>
#include "syn6288.h"

extern uint8_t suoyidakai[];
extern uint8_t zhiwencuowu[]; // 指纹错误



/*串口接收中断处理在stm32f1xx_it.c里面*/
extern strType_XPT2046_Coordinate strDisplayCoordinate;
extern strType_XPT2046_TouchPara strXPT2046_TouchPara;

uint32_t AS608Addr = 0XFFFFFFFF;//默认
extern volatile uint32_t seconds; // 引用 main.c 中的 seconds 变量
extern int test_time;
extern int finger_time;

char str2[6] = {0};

uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.

uint8_t Get_Device_Code[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//口令验证

uint8_t USART3_RX_STA= 0;//串口是否接收到数据

extern uint16_t currentFingerprintCount;  // 当前存储的指纹数量
extern int returnTomain;
extern int enter;


//串口发送一个字节
static uint8_t MYUSART_SendData(uint8_t data)
{
	if(HAL_UART_Transmit(&AS608_UART,&data,1,0xff) == HAL_OK)
		return 0;
	return 1;
}
//发送包头
static void SendHead(void)
{
	memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));//发送前清空数据，因为所有都要发送包头，所以只需要在发送包头前清空即可
  MYUSART_SendData(0xEF);
  MYUSART_SendData(0x01);
}
//发送地址
static void SendAddr(void)
{
  MYUSART_SendData(AS608Addr >> 24);
  MYUSART_SendData(AS608Addr >> 16);
  MYUSART_SendData(AS608Addr >> 8);
  MYUSART_SendData(AS608Addr);
}
//发送包标识,
static void SendFlag(uint8_t flag)
{
  MYUSART_SendData(flag);
}
//发送包长度
static void SendLength(int length)
{
  MYUSART_SendData(length >> 8);
  MYUSART_SendData(length);
}
//发送指令码
static void Sendcmd(uint8_t cmd)
{
  MYUSART_SendData(cmd);
}
//发送校验和
static void SendCheck(uint16_t check)
{
  MYUSART_SendData(check >> 8);
  MYUSART_SendData(check);
}

/*****************************************
函数名：uint8_t AS608_Check(void)
参数：无
功能描述：模块是否连接检测
返回值：模块连接了返回0 否则返回1
*****************************************/
static uint8_t AS608_Check(void)
{
	USART3_RX_BUF[9] = 1;

  SendHead();
  SendAddr();
	for(int i = 0; i < 10; i++)
	{
		MYUSART_SendData(Get_Device_Code[i]);
	}
	//HAL_UART_Receive(&AS608_UART,USART3_RX_BUF,12,100);//串口三接收12个数据
	HAL_Delay(200);//等待200ms
	if(USART3_RX_BUF[9] == 0)
		return 0;

  return 1;
}
/*指纹模块初始化*/
uint8_t as608_init(void)
{
	currentFingerprintCount = 0;
	//设置uart3接收中断
	HAL_UART_Receive_IT(&AS608_UART,USART3_RX_BUF,sizeof( USART3_RX_BUF));//接收数据，且产生中断
	//使能空闲中断
	__HAL_UART_ENABLE_IT(&AS608_UART,UART_IT_IDLE);//

	return AS608_Check();
}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
static uint8_t *JudgeStr(uint16_t waittime)
{
  char *data;
  uint8_t str[8];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = 0x07;
  str[7] = '\0';
  USART3_RX_STA = 0;
  while(--waittime)
  {
    HAL_Delay(1);
    if(USART3_RX_STA) //接收到一次数据
    {
      USART3_RX_STA = 0;
      data = strstr((const char*)USART3_RX_BUF, (const char*)str);
      if(data)
        return (uint8_t*)data;
    }
  }
  return 0;
}
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。
//模块返回确认字
uint8_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x01);
  temp =  0x01 + 0x03 + 0x01;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02
//模块返回确认字
uint8_t PS_GenChar(uint8_t BufferID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x04);
  Sendcmd(0x02);
  MYUSART_SendData(BufferID);
  temp = 0x01 + 0x04 + 0x02 + BufferID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件
//模块返回确认字
uint8_t PS_Match(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x03);
  temp = 0x01 + 0x03 + 0x03;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
uint8_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x08);
  Sendcmd(0x04);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x04 + BufferID
         + (StartPage >> 8) + (uint8_t)StartPage
         + (PageNum >> 8) + (uint8_t)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID   = (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}
//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2
//说明:  模块返回确认字
uint8_t PS_RegModel(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x05);
  temp = 0x01 + 0x03 + 0x05;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
uint8_t PS_StoreChar(uint8_t BufferID, uint16_t PageID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x06);
  Sendcmd(0x06);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  temp = 0x01 + 0x06 + 0x06 + BufferID
         + (PageID >> 8) + (uint8_t)PageID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
uint8_t PS_DeletChar(uint16_t PageID, uint16_t N)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x07);
  Sendcmd(0x0C);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  MYUSART_SendData(N >> 8);
  MYUSART_SendData(N);
  temp = 0x01 + 0x07 + 0x0C
         + (PageID >> 8) + (uint8_t)PageID
         + (N >> 8) + (uint8_t)N;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
uint8_t PS_Empty(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x0D);
  temp = 0x01 + 0x03 + 0x0D;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
uint8_t PS_WriteReg(uint8_t RegNum, uint8_t DATA)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x05);
  Sendcmd(0x0E);
  MYUSART_SendData(RegNum);
  MYUSART_SendData(DATA);
  temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  if(ensure == 0)
    printf("\r\n设置参数成功！");
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数（波特率，包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数（16bytes）
uint8_t PS_ReadSysPara(SysPara *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x0F);
  temp = 0x01 + 0x03 + 0x0F;
  SendCheck(temp);
  data = JudgeStr(1000);
  if(data)
  {
    ensure = data[9];
    p->PS_max = (data[14] << 8) + data[15];
    p->PS_level = data[17];
    p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
    p->PS_size = data[23];
    p->PS_N = data[25];
  }
  else
    ensure = 0xff;
  if(ensure == 0x00)
  {
    printf("\r\n模块最大指纹容量=%d", p->PS_max);
    printf("\r\n对比等级=%d", p->PS_level);
    printf("\r\n地址=%x", p->PS_addr);
    printf("\r\n波特率=%d", p->PS_N * 9600);
  }
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
uint8_t PS_SetAddr(uint32_t PS_addr)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x07);
  Sendcmd(0x15);
  MYUSART_SendData(PS_addr >> 24);
  MYUSART_SendData(PS_addr >> 16);
  MYUSART_SendData(PS_addr >> 8);
  MYUSART_SendData(PS_addr);
  temp = 0x01 + 0x07 + 0x15
         + (uint8_t)(PS_addr >> 24) + (uint8_t)(PS_addr >> 16)
         + (uint8_t)(PS_addr >> 8) + (uint8_t)PS_addr;
  SendCheck(temp);
  AS608Addr = PS_addr; //发送完指令，更换地址
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  AS608Addr = PS_addr;
  if(ensure == 0x00)
    printf("\r\n设置地址成功！");
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}
//功能： 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
//	该记事本逻辑上被分成 16 个页。
//参数:  NotePageNum(0~15),Byte32(要写入内容，32个字节)
//说明:  模块返回确认字
uint8_t PS_WriteNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
  uint16_t temp;
  uint8_t  ensure, i;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(36);
  Sendcmd(0x18);
  MYUSART_SendData(NotePageNum);
  for(i = 0; i < 32; i++)
  {
    MYUSART_SendData(Byte32[i]);
    temp += Byte32[i];
  }
  temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//读记事PS_ReadNotepad
//功能：  读取FLASH用户区的128bytes数据
//参数:  NotePageNum(0~15)
//说明:  模块返回确认字+用户信息
uint8_t PS_ReadNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
  uint16_t temp;
  uint8_t  ensure, i;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x04);
  Sendcmd(0x19);
  MYUSART_SendData(NotePageNum);
  temp = 0x01 + 0x04 + 0x19 + NotePageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    for(i = 0; i < 32; i++)
    {
      Byte32[i] = data[10 + i];
    }
  }
  else
    ensure = 0xff;
  return ensure;
}
//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//		  若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//		  很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
uint8_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x08);
  Sendcmd(0x1b);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x1b + BufferID
         + (StartPage >> 8) + (uint8_t)StartPage
         + (PageNum >> 8) + (uint8_t)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID 	= (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//命令包标识
  SendLength(0x03);
  Sendcmd(0x1d);
  temp = 0x01 + 0x03 + 0x1d;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    *ValidN = (data[10] << 8) + data[11];
  }
  else
    ensure = 0xff;

  if(ensure == 0x00)
  {
    printf("\r\n有效指纹个数=%d", (data[10] << 8) + data[11]);
  }
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）
uint8_t PS_HandShake(uint32_t *PS_Addr)
{
  SendHead();
  SendAddr();
  MYUSART_SendData(0X01);
  MYUSART_SendData(0X00);
  MYUSART_SendData(0X00);
  HAL_Delay(200);
  if(USART3_RX_STA) //接收到数据
  {
    if(//判断是不是模块返回的应答包
      USART3_RX_BUF[0] == 0XEF
      && USART3_RX_BUF[1] == 0X01
      && USART3_RX_BUF[6] == 0X07
    )
    {
      *PS_Addr = (USART3_RX_BUF[2] << 24) + (USART3_RX_BUF[3] << 16)
                 + (USART3_RX_BUF[4] << 8) + (USART3_RX_BUF[5]);
      USART3_RX_STA = 0;
      return 0;
    }
    USART3_RX_STA = 0;
  }
  return 1;
}
//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
const char *EnsureMessage(uint8_t ensure)
{
  const char *p;
  switch(ensure)
  {
  case  0x00:
    p = "       OK       ";
    break;
  case  0x01:
    p = " 数据包接收错误 ";
    break;
  case  0x02:
    p = "传感器上没有手指";
    break;
  case  0x03:
    p = "录入指纹图像失败";
    break;
  case  0x04:
    p = " 指纹太干或太淡 ";
    break;
  case  0x05:
    p = " 指纹太湿或太糊 ";
    break;
  case  0x06:
    p = "  指纹图像太乱  ";
    break;
  case  0x07:
    p = " 指纹特征点太少 ";
    break;
  case  0x08:
    p = "  指纹不匹配    ";
    break;
  case  0x09:
    p = " 没有搜索到指纹 ";
    break;
  case  0x0a:
    p = "   特征合并失败 ";
    break;
  case  0x0b:
    p = "地址序号超出范围";
  case  0x10:
    p = "  删除模板失败  ";
    break;
  case  0x11:
    p = " 清空指纹库失败 ";
    break;
  case  0x15:
    p = "缓冲区内无有效图";
    break;
  case  0x18:
    p = " 读写FLASH出错  ";
    break;
  case  0x19:
    p = "   未定义错误   ";
    break;
  case  0x1a:
    p = "  无效寄存器号  ";
    break;
  case  0x1b:
    p = " 寄存器内容错误 ";
    break;
  case  0x1c:
    p = " 记事本页码错误 ";
    break;
  case  0x1f:
    p = "    指纹库满    ";
    break;
  case  0x20:
    p = "    地址错误    ";
    break;
  default :
    p = " 返回确认码有误 ";
    break;
  }
  return p;
}

//显示确认码错误信息
void ShowErrMessage(uint8_t ensure)
{
  //OLED_ShowCH(5,0,(uint8_t*)EnsureMessage(ensure));
	printf("%s\r\n",EnsureMessage(ensure));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void add_finger(void)
{
	LCD_Clear(0,60,240,320,GREY);

	if (currentFingerprintCount == 0)
	{
		Add_FR();
		return;
	}
    uint8_t ensure;
    SearchResult search;
    char str[20];

    if (currentFingerprintCount >= MAX_FINGERPRINTS) {
        LCD_Clear(0, 60, 240, 16, GREY);
        LCD_DrawString(0, 60, "Max fingerprints reached");
        HAL_Delay(2000);
        Draw_FINGER_Menu();
        return;
    }

    seconds = 0;

    LCD_Clear(60,0,180,60,GREY);
    LCD_DrawString(70, 10, "Administrator");
    LCD_DrawString(70, 30, "Verification Menu");
    char left_scond[20];
    while (1)
    {
        sprintf(left_scond, "%lu  ", (30-seconds));
        LCD_DrawString(80,47,"left time:");
        LCD_DrawString(170, 47, left_scond);

        if (seconds >= 30)
        {
            Draw_FINGER_Menu(); // 返回上一层界面
            return;
        }

        if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
        {
        	seconds = 0;
            if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
            {
            	Draw_FINGER_Menu(); // 返回上一层界面
                return;
            }
        }
        LCD_Clear(0, 60, 240, 16, GREY);
        LCD_DrawString(0, 60, "Admin, please press finger");
        LCD_DrawLine(0, 78, 150, 78, BLACK);

        if (finger_time == 5 || finger_time == 10)
        {
        	Draw_FINGER_Menu(); // 返回上一层界面
	    	return;
        }

        // 尝试获取图像，检查是否有手指按下
        ensure = PS_GetImage();
        if (ensure == 0x00) // 获取图像成功，表示有手指按下
        {
            ensure = PS_GenChar(CharBuffer1);
            if (ensure == 0x00) // 生成特征成功
            {
            	seconds = 0;
                ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &search);
                if (ensure == 0x00 && search.pageID == 0) // 搜索成功且匹配管理员ID
                {
                    LCD_Clear(0, 100, 240, 16, GREY);
                    LCD_DrawString(0, 100, "Admin verified");
                    LCD_DrawLine(0, 118, 150, 118, BLACK);
                    finger_time = 0;
                    HAL_Delay(2000);
                    Add_FR(); // 调用录入新指纹的函数
                    return;
                }
                else
                {
                    LCD_Clear(0, 100, 240, 16, GREY);
                    LCD_DrawString(0, 100, "Verification failed");
                    finger_time++;
                    LCD_DrawLine(0, 118, 150, 118, BLACK);
                    HAL_Delay(2000);
                    LCD_Clear(0, 100, 240, 20, GREY);
                }
            }
            else
            {
                //LCD_Clear(0, 76, 240, 16, GREY);
                //LCD_DrawString(0, 76, "Feature generation failed");
                //HAL_Delay(1500);
            }
        }
        else
        {
            LCD_Clear(0, 80, 240, 16, GREY);
            LCD_DrawString(0, 80, "please put your finger");
            LCD_DrawLine(0, 98, 150, 98, BLACK);
            HAL_Delay(1500);
        }
    }
}

//录指纹
void Add_FR(void)
{
    uint8_t i, ensure, processnum = 0;
    uint8_t ID_NUM = 0;
    seconds = 0;

    LCD_Clear(0,60,240,260,GREY);
    LCD_Clear(60,0,180,60,GREY);
    LCD_Clear(0, 0, 60, 60, BLUE);
    LCD_DrawString(10, 20, "BACK");
    LCD_DrawString(60, 30, "add finger menu");

    char left_scond[20];
    while(1)
    {
        sprintf(left_scond, "%lu  ", (30-seconds));
        LCD_DrawString(80,47,"left time:");
        LCD_DrawString(170, 47, left_scond);

        if (seconds >= 30)
        {
            Draw_FINGER_Menu(); // 返回上一层界面
            enter = -1;
            return;
        }

        if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
        {
        	seconds = 0;
            if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
            {
            	if (enter == 3)
            	{
                	enter = -1;
                	return;
            	}
            	Draw_FINGER_Menu(); // 返回上一层界面
                return;
            }
        }

        switch (processnum)
        {
        case 0:
            i++;
            LCD_Clear(0, 60, 240, 260, GREY);
            LCD_DrawString(0, 60, "Please press finger");
            LCD_DrawLine(0, 78, 150, 78, BLACK);
            ensure = PS_GetImage();
            if(ensure == 0x00)
            {
                ensure = PS_GenChar(CharBuffer1); //生成特征
                if(ensure == 0x00)
                {
                	seconds = 0;
                	LCD_Clear(0, 80, 240, 16, GREY);
                    LCD_DrawString(0, 80, "Fingerprint OK");
                    LCD_DrawLine(0, 98, 150, 98, BLACK);
                    i = 0;
                    processnum = 1; //跳到第二步
                }
                else ShowErrMessage(ensure);
            }
            else ShowErrMessage(ensure);
            break;

        case 1:
            i++;
            LCD_Clear(0, 100, 240, 16, GREY);
            LCD_DrawString(0, 100, "Please press again");
            LCD_DrawLine(0, 118, 150, 118, BLACK);
            ensure = PS_GetImage();
            if(ensure == 0x00)
            {
                ensure = PS_GenChar(CharBuffer2); //生成特征
                if(ensure == 0x00)
                {
                	seconds = 0;
                    LCD_Clear(0, 120, 240, 16, GREY);
                    LCD_DrawString(0, 120, "Fingerprint OK");
                    LCD_DrawLine(0, 138, 150, 138, BLACK);
                    i = 0;
                    processnum = 2; //跳到第三步
                }
                else ShowErrMessage(ensure);
            }
            else ShowErrMessage(ensure);
            break;

        case 2:
            LCD_Clear(0, 140, 240, 40, GREY);
            LCD_DrawString(0, 140, "Compare fingerprints ......");
            LCD_DrawLine(0, 158, 150, 158, BLACK);
            ensure = PS_Match();
            if(ensure == 0x00)
            {
                LCD_Clear(0, 160, 240, 16, GREY);
                LCD_DrawString(0, 160, "Match success");
                LCD_DrawLine(0, 178, 150, 178, BLACK);
                processnum = 3; //跳到第四步
            }
            else
            {
            	LCD_DrawString(0, 160, "Match failed");
            	LCD_DrawLine(0, 178, 150, 178, BLACK);
            	HAL_Delay(2000);
                LCD_Clear(0, 0, 240, 180, GREY);
                ShowErrMessage(ensure);
                i = 0;
                processnum = 0; //跳回第一步
                add_finger_Menu();
            }
            HAL_Delay(500);
            break;

        case 3:
            LCD_Clear(0, 180, 240, 16, GREY);
            LCD_DrawString(0, 180, "Generate template");
            LCD_DrawLine(0, 198, 150, 198, BLACK);
            HAL_Delay(500);
            ensure = PS_RegModel();
            if(ensure == 0x00)
            {
                LCD_Clear(0, 200, 240, 16, GREY);
                LCD_DrawString(0, 200, "Template success");
                LCD_DrawLine(0, 218, 150, 218, BLACK);
                processnum = 4; //跳到第五步
            }
            else
            {
                processnum = 0;
                ShowErrMessage(ensure);
                add_finger_Menu();
            }
            HAL_Delay(1000);
            break;

        case 4:
        	ID_NUM = FindEmptyID(); // 查找第一个空ID
            ensure = PS_StoreChar(CharBuffer2, ID_NUM); //储存模板
            if(ensure == 0x00)
            {
            	fingerprintLibrary[ID_NUM] = 1; // 更新为已使用
                LCD_Clear(0, 220, 240, 16, GREY);
                LCD_DrawString(0, 220, "Fingerprint saved");
                LCD_DrawLine(0, 238, 150, 238, BLACK);
                LCD_DrawString(0, 240, "ID:");
                char str[10]; // 用于存储转换后的字符串
                sprintf(str, "%d", ID_NUM);
                LCD_DrawString(50, 240, str);
                LCD_DrawLine(0, 258, 150, 258, BLACK);
                currentFingerprintCount++;  // 更新已存储指纹数量
                if (enter == 3)
                {
      	    		LCD_Clear(0,120,200,50,GREEN);
      	    		LCD_DrawString(10, 130, "Manager Fingerprint Reset Successful");
                	HAL_Delay(2000);
                    enter = -1;
                    return;
                }
                else
                {
                    Draw_FINGER_Menu();
                    HAL_Delay(2000);
                    return;
                }

            }
            else
            {
                processnum = 0;
                ShowErrMessage(ensure);
                add_finger_Menu();
            }
            break;
        }
        HAL_Delay(400);
        if(i == 10 && enter!= 3) //超过5次没有按手指则退出
        {
            Draw_FINGER_Menu();
            enter = -1;
            break;
        }
    }
}

SysPara AS608Para;//指纹模块AS608参数

void press_FR(void)
{
    SearchResult seach;
    uint8_t ensure;
    char str[20];
    char left_scond[20];
    // 尝试获取图像，检查是否有手指按下

    ensure = PS_GetImage();
    if (ensure == 0x00) // 获取图像成功，表示有手指按下
    {
        ensure = PS_GenChar(CharBuffer1);
        if (ensure == 0x00) // 生成特征成功
        {
        	seconds = 0;
            ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &seach);
            if (ensure == 0x00 ) // 搜索成功
            {
                LCD_DrawString(0, 100, "Verification success");
                LCD_DrawLine(0, 118, 150, 118, BLACK);
                //sprintf(str, " ID:%d Score:%d ", seach.pageID, seach.mathscore);
                sprintf(str, " ID:%d", seach.pageID);
                LCD_DrawString(0, 120, str);
                LCD_DrawLine(0, 138, 150, 138, BLACK);

                returnTomain = 1;
                finger_time = 0;
                test_time = 0;
                TIM1->CCR4 = 150;

	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	        	SYN_FrameInfo(0, suoyidakai);
                //HAL_Delay(2000);
	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
                LCD_Clear(0,100,240,40,GREY);
            }
                else
                {
                	returnTomain = 0;
                    LCD_DrawString(0, 100, "Verification failed");
                    LCD_DrawLine(0, 118, 150, 118, BLACK);
                    finger_time++;
                    TIM1->CCR4 = 163;
    	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    	        	SYN_FrameInfo(0, zhiwencuowu);
                    //HAL_Delay(2000);
    	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    	        	LCD_Clear(0,100,240,20,GREY);
                }


        }
        else
        {
            //LCD_DrawString(0, 80, "Feature generation failed");
        }
    }
    else
    {
        // 没有手指按下时，可以选择不做任何事情，或者显示提示信息
        LCD_DrawString(0, 80,"PLEASE PUT YOUR FINGER");
        LCD_DrawLine(0, 98, 150, 98, BLACK);

    }
}

void Del_FR(void)
{
	if (!Verify_Admin()) // 管理员验证失败，返回
	{
        //Draw_FINGER_Menu();
        return;
	}

    uint8_t ensure;
    char keyCH;
    uint16_t ID_NUM = -1;
    LCD_DrawString(0, 80, "Delete fingerprint ID: ");
    LCD_DrawLine(0, 98, 150, 98, BLACK);
    seconds = 0;
    char left_scond[20];
    while (1)
    {
        // 检查按键输入
        keyCH = Get_Key_Ch();
        HAL_Delay(200); // 防抖延迟
        LCD_DrawString(0,100,"id:");
        LCD_DrawChar(50, 100, keyCH);
        LCD_DrawLine(0, 118, 150, 118, BLACK);

        sprintf(left_scond, "%lu  ", (30-seconds));
        LCD_DrawString(80,40,"left time:");
        LCD_DrawString(170, 40, left_scond);

        if (seconds >= 30)
        {
            Draw_FINGER_Menu(); // 返回上一层界面
            return;
        }

        if(keyCH == '0')
        {
        	seconds = 0;
        	LCD_DrawString(0, 120, "cannot delete manager fingerprint");
        	LCD_DrawLine(0, 138, 150, 138, BLACK);
        	HAL_Delay(2000);
        	LCD_Clear(0,120,240,16,GREY);
            Draw_FINGER_Menu(); // 返回上一层界面
            return;
        }
        if (keyCH >= '6' && keyCH <= '9')
        {
        	seconds = 0;
        	HAL_Delay(200); // 防抖延迟
            LCD_DrawString(0, 120, "it is out of range");
            LCD_DrawLine(0, 138, 150, 138, BLACK);
            HAL_Delay(2000);
            LCD_Clear(0,120,240,16,GREY);
            Draw_FINGER_Menu(); // 返回上一层界面
            return;
        }
        if (keyCH >= '1' && keyCH <= '4')
        {
        	seconds = 0;
            ID_NUM = keyCH - '0';
            HAL_Delay(200); // 防抖延迟
            ensure = PS_DeletChar(ID_NUM, 1); // 删除单个指纹
            if (ensure == 0)
            {
            	if (fingerprintLibrary[ID_NUM] == 0)
            	{
                    LCD_DrawString(0, 140, "this fingerprint does not exist");
                    LCD_DrawLine(0, 158, 150, 158, BLACK);
                    HAL_Delay(2000);
                    Draw_FINGER_Menu(); // 返回上一层界面
                    return;
            	}
            	fingerprintLibrary[ID_NUM] = 0; // 更新为未使用
            	currentFingerprintCount--;
                LCD_DrawString(0, 140, "Delete success");
                LCD_DrawLine(0, 158, 150, 158, BLACK);
                HAL_Delay(2000);
                Draw_FINGER_Menu(); // 返回上一层界面
                return;
            }
            else
            {
                ShowErrMessage(ensure);
                HAL_Delay(2000);
                Draw_FINGER_Menu(); // 返回上一层界面
                return;
            }
        }

        // 检查触摸屏输入
       if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
        {
    	   seconds = 0;
            if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
            {
                Draw_FINGER_Menu(); // 返回上一层界面
                return;
            }
        }
    }

}
void Del_FR_Lib(void)
{
    if (!Verify_Admin()) // 管理员验证失败，返回
    {
        //Draw_FINGER_Menu();
        return;
    }

    uint8_t ensure;
    LCD_DrawString(0, 80, "Deleting fingerprint library");
    LCD_DrawLine(0, 98, 150, 98, BLACK);

    // 备份管理者指纹到 CharBuffer1


    // 清空指纹库
    ensure = PS_Empty();
    if (ensure == 0)
    {
        for (uint8_t id = 0; id < MAX_FINGERPRINTS; id++)
        {
            fingerprintLibrary[id] = 0;
        }
        LCD_DrawString(0, 100, "Library cleared");
        LCD_DrawLine(0, 118, 150, 118, BLACK);
        currentFingerprintCount = 0;

        // 恢复管理者指纹

        Add_FR();
    }
    else
    {
        ShowErrMessage(ensure);
    }
    HAL_Delay(1500);
}

bool Verify_Admin(void)
{
    uint8_t ensure;
    SearchResult search;
    seconds = 0;
    char left_scond[20];
    while (1)
    {
        sprintf(left_scond, "%lu  ", (30-seconds));
        LCD_DrawString(80,40,"left time:");
        LCD_DrawString(170, 40, left_scond);

        if (finger_time == 5 || finger_time == 10)
        {
        	Draw_FINGER_Menu(); // 返回上一层界面
	    	return false;
        }

        if (seconds >= 30)
        {
        	Draw_FINGER_Menu(); // 返回上一层界面
        	return false; // �???????????????????????????????出子循环，回到母循环
        }
        if (XPT2046_Get_TouchedPoint(&strDisplayCoordinate, &strXPT2046_TouchPara))
        {
        	seconds = 0;
            if ((strDisplayCoordinate.y > 0 && strDisplayCoordinate.y < 60) &&
                (strDisplayCoordinate.x > 0 && strDisplayCoordinate.x < 60))
            {
                Draw_FINGER_Menu(); // 返回上一层界面
                return false;
            }
        }
        LCD_Clear(0, 60, 240, 16, GREY);
        LCD_DrawString(0, 60, "Admin, please press finger");
        LCD_DrawLine(0, 78, 150, 78, BLACK);

        // 尝试获取图像，检查是否有手指按下
        ensure = PS_GetImage();
        if (ensure == 0x00) // 获取图像成功，表示有手指按下
        {
            ensure = PS_GenChar(CharBuffer1);
            if (ensure == 0x00) // 生成特征成功
            {
            	seconds = 0;
                ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &search);
                if (ensure == 0x00 && search.pageID == 0) // 搜索成功且匹配管理员ID
                {
                	finger_time = 0;
                    LCD_Clear(0, 80, 240, 16, GREY);
                    LCD_DrawString(0, 80, "Admin verified");
                    LCD_DrawLine(0, 98, 150, 98, BLACK);
                    HAL_Delay(2000);
                    //LCD_Clear(0,80,240,16,GREY);
                    return true;
                }
                else
                {
                	finger_time++;
                    LCD_Clear(0, 80, 240, 16, GREY);
                    LCD_DrawString(0, 80, "Verification failed");
                    LCD_DrawLine(0, 98, 150, 98, BLACK);
                    HAL_Delay(2000);
                    //LCD_Clear(0,80,240,16,GREY);
                }
            }
            else
            {
                LCD_Clear(0, 80, 240, 16, GREY);
                LCD_DrawString(0, 80, "Please put your finger");
                HAL_Delay(1500);
            }
        }
        else
        {
            LCD_Clear(0, 80, 240, 16, GREY);
            LCD_DrawString(0, 80, "No finger detected");
            LCD_DrawLine(0, 98, 150, 98, BLACK);
            HAL_Delay(1500);
        }
    }
}
// 查找第一个空闲的 ID
uint8_t FindEmptyID(void) {
    for (uint8_t id = 0; id < MAX_FINGERPRINTS; id++) {
        if (IsIDEmpty(id)) {
            return id; // 返回第一个空闲的 ID
        }
    }
    return MAX_FINGERPRINTS; // 如果没有空位，返回最大值表示无可用 ID
}

// 判断 ID 是否为空
bool IsIDEmpty(uint8_t id) {
    if (id >= MAX_FINGERPRINTS) {
        return false; // ID 超出范围，返回 false
    }
    return fingerprintLibrary[id] == 0; // 如果 ID 为空，返回 true
}

uint8_t fingerprintLibrary[MAX_FINGERPRINTS] = {0};

