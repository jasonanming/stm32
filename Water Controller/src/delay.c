/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx delay functions
**********************************************************************/
#include <delay.h>

static u8 fac_us=0; /* us延时的乘数 */
static u16 fac_ms=0;/* ms延时的乘数 */

/*********************************************************************
Function: delay_init      
Description: 初始化延迟函数
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      SYSCLK	系统时钟           
Output:
			SYSTICK的时钟固定为HCLK时钟的1/8,SYSTICK的1000倍就是ms
Return:        
Others:      
**********************************************************************/
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    

/*********************************************************************
Function: delay_ms      
Description: 延时ms
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      nms	           
Output:
Return:        
Others: 
注意nms的范围，nms<=0xffffff*8*1000/SYSCLK，在168M条件下，nms<=761ms
**********************************************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;/* 时间加载(SysTick->LOAD为24bit) */
	SysTick->VAL =0x00;           /* 清空计数器 */
	SysTick->CTRL=0x01 ;          /* 开始倒数  */
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));/* 等待时间到达 */
	SysTick->CTRL=0x00;       /* 关闭计数器 */
	SysTick->VAL =0X00;       /* 清空计数器	*/
}

/*********************************************************************
Function: delay_us      
Description: 延时us
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      nus	为要延时的us数        
Output:
Return:        
Others:      
**********************************************************************/
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; /* 时间加载	  */
	SysTick->VAL=0x00;        /* 清空计数器 */
	SysTick->CTRL=0x01 ;      /* 开始倒数 	*/
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));/* 等待时间到达  */
	SysTick->CTRL=0x00;       /* 关闭计数器 */
	SysTick->VAL =0X00;       /* 清空计数器	*/
}

void Delay(u32 count)
{
	while(0 == count--);
}

