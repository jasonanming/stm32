/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx main function
**********************************************************************/

#include <main.h>

/*********************************************************************
Function: main      
Description: 应用程序的总入口函数
Calls: 
		delay_init()		延时初始化函数
Table Accessed: 
Table Updated: 
Input:          
                 
Output:
Return:        
Others:      
**********************************************************************/
#define _DS1302_DEBUG_
u8 current_time = 0;
int main(void)
{
	#if 1
	GPIO_InitTypeDef gpio1;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	delay_init(168);	/* 168MHZ set delay frequency*/
	
#ifdef _DS1302_DEBUG_
	ds1302_init();		/* 初始化实时时钟 */
#endif
	
#if 1
	gpio1.GPIO_Pin = GPIO_Pin_12;
	gpio1.GPIO_Mode = GPIO_Mode_OUT;
	gpio1.GPIO_Speed = GPIO_Speed_25MHz;
	gpio1.GPIO_OType = GPIO_OType_PP;
	gpio1.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &gpio1);
#endif
	while(1)
	{
		//ds1302_write(0x80, 0x01);
		current_time = ds1302_read(0x81);
		if (0 != (current_time & 0x80))
		{
			ds1302_write(0x80, 0x00);
			current_time = ds1302_read(0x81);
		}
		//GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET);
		delay_ms(500);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
		//GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		delay_ms(500);
	}
	#endif
	
	return 0;
}
