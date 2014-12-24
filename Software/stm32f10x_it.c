/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
u8 remote_f=0;       //接收完成一帧标志位
u8 send_f=0;
u16 data0;           //前16个脉冲存放地址 
u16 data1;           //后16个脉冲存放地址       
u16 oldcal=0,newcal=0; //时间的先后纪录 
u16 bitcnt=0;        //脉冲个数纪录 
u8  Rec_Data[8];

unsigned char NFC_Int_flag=0;

u8 i;
u8 saddress;
u8 eaddress;
u8 times;
u16 Time;
u8 nTime;
u8 rTime;
unsigned int pulse_width1=0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */




void TIM2_IRQHandler(void)
{
  unsigned char temp=0;
  u16 time;
  if(TIM_GetITStatus( TIM2,  TIM_IT_CC4 ))
  {
    TIM_ClearITPendingBit(TIM2,  TIM_IT_CC4);
    newcal = TIM_GetCapture4(TIM2);
    if(newcal<oldcal)
      time = 0xffff-oldcal+newcal;
    else
      time = newcal-oldcal;
    oldcal = newcal;
    
    if((time>10)&&(time<13))
      temp = 0;
    else if((time>21)&&(time<24))
      temp = 1;
    else if((time>130)&&(time<140))
    {
      bitcnt = 0;
      data0 = 0;
      data1 = 0;
      return;
    }
    
    
    bitcnt++;
    if(bitcnt<=16)
    {
      data0 = data0<<1;
      data0 = data0|temp;
    }
    else
    {
      data1 = data1<<1;
      data1 = data1|temp;
    }
    
    if(bitcnt==32)
    {
       bitcnt = 0;
       remote_f = 1;
    }
  }
}


void USART1_IRQHandler(void)
{  
   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
   {
     USART_ClearITPendingBit(USART1,USART_IT_RXNE);
     
     //1、接收数据，存入队列
     Uart_fifo_write(USART_ReceiveData(USART1));

   }
}

void USART3_IRQHandler(void)
{  
   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
   {
     USART_ClearITPendingBit(USART3,USART_IT_RXNE);
     //1、接收数据，存入队列
     Uart_fifo_write(USART_ReceiveData(USART3));
     //USART_SendData(USART1,USART_ReceiveData(USART3));
   }
}

 /**
   * @brief  This function handles External line 0 interrupt request.
   * @param  None
   * @retval None
   */
 void EXTI1_IRQHandler(void)
 {
   if(EXTI_GetITStatus(EXTI_Line1) != RESET)
   {

     NFC_Int_flag=1;
     /* Clear the  EXTI line 0 pending bit */
     EXTI_ClearITPendingBit(EXTI_Line1);
   }
 }

void TIM1_CC_IRQHandler(void)
 {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    pulse_width1=TIM_GetCapture1(TIM1);   
 }

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
