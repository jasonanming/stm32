/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx delay header
**********************************************************************/
#ifndef __DELAY_H
#define __DELAY_H 		

#include <all_include.h>

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif



