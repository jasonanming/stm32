/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx delay functions
**********************************************************************/
#include <delay.h>

static u8 fac_us=0; /* us��ʱ�ĳ��� */
static u16 fac_ms=0;/* ms��ʱ�ĳ��� */

/*********************************************************************
Function: delay_init      
Description: ��ʼ���ӳٺ���
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      SYSCLK	ϵͳʱ��           
Output:
			SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8,SYSTICK��1000������ms
Return:        
Others:      
**********************************************************************/
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    

/*********************************************************************
Function: delay_ms      
Description: ��ʱms
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      nms	           
Output:
Return:        
Others: 
ע��nms�ķ�Χ��nms<=0xffffff*8*1000/SYSCLK����168M�����£�nms<=761ms
**********************************************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;/* ʱ�����(SysTick->LOADΪ24bit) */
	SysTick->VAL =0x00;           /* ��ռ����� */
	SysTick->CTRL=0x01 ;          /* ��ʼ����  */
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));/* �ȴ�ʱ�䵽�� */
	SysTick->CTRL=0x00;       /* �رռ����� */
	SysTick->VAL =0X00;       /* ��ռ�����	*/
}

/*********************************************************************
Function: delay_us      
Description: ��ʱus
Calls: 
Table Accessed: 
Table Updated: 
Input:          
      nus	ΪҪ��ʱ��us��        
Output:
Return:        
Others:      
**********************************************************************/
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; /* ʱ�����	  */
	SysTick->VAL=0x00;        /* ��ռ����� */
	SysTick->CTRL=0x01 ;      /* ��ʼ���� 	*/
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));/* �ȴ�ʱ�䵽��  */
	SysTick->CTRL=0x00;       /* �رռ����� */
	SysTick->VAL =0X00;       /* ��ռ�����	*/
}

void Delay(u32 count)
{
	while(0 == count--);
}

