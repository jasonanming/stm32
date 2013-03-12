/*********************************************************************
Copyright:Anxin Ming Co.,Ltd.
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx ds1302 functions
**********************************************************************/
#include <ds1302.h>

#define D_SDAPort   GPIOA
#define D_SDAPin    GPIO_Pin_0
#define D_SCLKPort  GPIOA
#define D_SCLKPin   GPIO_Pin_1
#define D_RSTPort   GPIOA
#define D_RSTPin    GPIO_Pin_3
/*********************************************************************
Function:ds1302_init
Description:ds1302оƬ��ʼ��
Calls:
GPIO_Init
Table Accessed:
Table Updated:
Input:
    dat     ��Ҫ���������
Output:
Return:
Others:
**********************************************************************/
WCAPI void ds1302_init(void)
{
    GPIO_InitTypeDef gpio;
    /* ����SDA������������˿� */
    /* TODO nothing */
    /* ����SCLKʱ��ͬ���˿� */
    gpio.GPIO_Pin = D_SCLKPin;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(D_SCLKPort, &gpio);
    /* ����RSTоƬʹ�ܶ˿� */
    memset(&gpio, 0, sizeof(gpio));
    gpio.GPIO_Pin = D_RSTPin;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(D_RSTPort, &gpio);
}
/*********************************************************************
Function:ds1302_ioctl
Description:ds1302�˿ڿ���
Calls:
delay_us
GPIO_Init
Table Accessed:
Table Updated:
Input:
    cmd     ��Ҫ��������� 1--OUT 0--IN
Output:
Return:
Others:
**********************************************************************/
static void ds1302_ioctl(u8 cmd)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = D_SDAPin;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_PuPd = GPIO_PuPd_UP;
    if (cmd)
    {
        gpio.GPIO_Mode = GPIO_Mode_OUT;
    }
    else
    {
        gpio.GPIO_Mode = GPIO_Mode_IN;
    }
    GPIO_Init(D_SDAPort, &gpio);
}
/*********************************************************************
Function:ds1302_writebyte
Description:ds1302�����ֽڶ�ȡ����
Calls:
GPIO_Write
delay_us
Table Accessed:
Table Updated:
Input:
    dat     ��Ҫ���������
Output:
Return:
Others:
**********************************************************************/
static void ds1302_writebyte(u8 dat)
{
    u8 loop = 0;

	  ds1302_ioctl(1);
		GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);	
    delay_us(2);
    for (; loop < 8; loop++)
    {
				if (dat&0x01)
				{
					GPIO_WriteBit(D_SDAPort, D_SDAPin, Bit_SET);
				}
				else
				{
					GPIO_WriteBit(D_SDAPort, D_SDAPin, Bit_RESET);
				}
        delay_us(3);
        GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_SET);
        delay_us(1);
				GPIO_WriteBit(D_SDAPort, D_SDAPin, Bit_RESET);
				delay_us(1);
        GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);
        delay_us(1);
        dat >>= 1;
    }
    return;
}
/*********************************************************************
Function:ds1302_write
Description:����ӿ�,ִ��д��ʱ�����
Calls:
GPIO_Write
ds1302_writebyte
Table Accessed:
Table Updated:
Input:
    cmd     ��Ӧ����
    dat     д�������
Output:
Return:
Others:
**********************************************************************/
WCAPI void ds1302_write(u8 cmd, u8 dat)
{
    GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);
		delay_us(1);
    GPIO_WriteBit(D_RSTPort, D_RSTPin, Bit_SET);
    ds1302_writebyte(cmd);
    ds1302_writebyte(dat);
    GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_SET);
	  delay_us(1);
    GPIO_WriteBit(D_RSTPort, D_RSTPin, Bit_RESET);
}
/*********************************************************************
Function:ds1302_readbyte
Description:ʵʱʱ�ӵ��ֽڶ�ȡ
Calls:
delay_us
GPIO_Write
Table Accessed:
Table Updated:
Input:

Output:��ȡ��������
Return:��ȡ��������
Others:
**********************************************************************/
static u8 ds1302_readbyte(void)
{
    u8 loop = 0;
    u8 dat = 0;

	  ds1302_ioctl(0);
    delay_us(3);
    for (; loop < 8; loop++)
    {
        dat >>= 1;
        if (Bit_SET == GPIO_ReadInputDataBit(D_SDAPort, D_SDAPin))
        {
            dat |= 0x80;
        }
        GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_SET);
        delay_us(3);
        GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);
        delay_us(3);
    }
    return dat;
}
/*********************************************************************
Function:ds1302_read
Description:��ȡָ����ֵ
Calls:
ds1302_writebyte
ds1302_readbyte
GPIO_Write
Table Accessed:
Table Updated:
Input:
    cmd         ָ����־
Output:��ȡ��������
Return:��ȡ��������
Others:
**********************************************************************/
WCAPI u8 ds1302_read(u8 cmd)
{
    u8 dat = 0;

    GPIO_WriteBit(D_RSTPort, D_RSTPin, Bit_RESET);
    GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);
    GPIO_WriteBit(D_RSTPort, D_RSTPin, Bit_SET);
	  //delay_us(1);
    ds1302_writebyte(cmd);
    dat = ds1302_readbyte();
    GPIO_WriteBit(D_SCLKPort, D_SCLKPin, Bit_RESET);
    GPIO_WriteBit(D_RSTPort, D_RSTPin, Bit_RESET);

    return dat;
}
