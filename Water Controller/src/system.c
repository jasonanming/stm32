/*********************************************************************
Copyright:Anxin Ming Co.,Ltd. 
Author: Jason.anming
Date:2013-3-10
Description:stm32f4xx system init functions
**********************************************************************/

#include <system.h>

#if 1
void SetSysClock(void);

/*********************************************************************
Function: SystemInit      
Description: ϵͳ�Զ��������õĳ�ʼ����������Ҫ���ó�ʼ����ʱ�ӣ�Ȼ���л�
Calls: SetSysClock()
Table Accessed: 
Table Updated: 
Input:          
                 
Output:
Return:        
Others:      
**********************************************************************/
void SystemInit(void)
{
/***************************ϵͳ�ڲ�ʱ���趨**************************/
	/* Reset the RCC clock configuration to the default reset state----*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
	
	/***************************ϵͳ�ڲ�ʱ���趨**************************/
  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings ---------------------------*/
  SetSysClock();
	
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | 0x00; /* Vector Table Relocation in Internal FLASH */
#endif
	return;
}

/*********************************************************************
Function: SetSysClock      
Description: ��ϵͳʱ������Ϊ�ⲿ����ͨ��PLL��Ƶ��ò���������Ƶ��
Calls: SetSysClock()
Table Accessed: 
Table Updated: 
Input:          
                 
Output:         
Return:        
Others:      
**********************************************************************/
void SetSysClock(void)
{
/************************************************************************/
/*            PLL (clocked by HSE) used as System clock source          */
/************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable high performance mode, System frequency up to 168 MHz */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_PMODE;  

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#if 1	/* �ر�I2S��ʹ�� */
/******************************************************************************/
/*                        I2S clock configuration                             */
/******************************************************************************/
  /* PLLI2S clock used as I2S clock source */
  RCC->CFGR &= ~RCC_CFGR_I2SSRC;

  /* Configure PLLI2S */
  RCC->PLLI2SCFGR = (PLLI2S_N << 6) | (PLLI2S_R << 28);

  /* Enable PLLI2S */
  RCC->CR |= ((uint32_t)RCC_CR_PLLI2SON);

  /* Wait till PLLI2S is ready */
  while((RCC->CR & RCC_CR_PLLI2SRDY) == 0)
  {
  }
#endif
	
}
#endif
