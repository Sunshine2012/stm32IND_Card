#ifndef _BSP_WDG_H
#define _BSP_WDG_H
#include <includes.h>

void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);

#endif
