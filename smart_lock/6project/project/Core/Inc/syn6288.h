#ifndef _SYN6288_H
#define _SYN6288_H

#include "main.h"

void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);

void SendStr5(unsigned char* buf, unsigned int len);

#endif
