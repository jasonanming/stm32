/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx ds1302 header
**********************************************************************/
#ifndef _DS1302_H_
#define _DS1302_H_

#include <all_include.h>

WCAPI void ds1302_init(void);
WCAPI void ds1302_write(u8 cmd, u8 dat);
WCAPI u8 ds1302_read(u8 cmd);

#endif
