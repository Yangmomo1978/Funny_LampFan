#include "gpio.h"

//PB1 ä¸­æ–­å¼•è„�?æ£€æµ‹è¾“å�?
static void NFC_INT()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  	
}

static void EXIT_Line1_Init()
{//EXIT13é…ç½�? 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); 
  
 EXTI_InitTypeDef EXTI_InitStructure;
 EXTI_InitStructure.EXTI_Line = EXTI_Line1;//å¤–éƒ¨ä¸­æ–­ç�? 
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ä¸­æ–­æ¨¡å¼�? 
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设置为中断请求，非事件请�? 
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ä¸‹é™æ²¿è§¦å? 
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;//æ‰“å¼€ä¸­æ–�? 
 EXTI_Init(&EXTI_InitStructure);
}

static void NVIC_Configuration()
{  
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   // æŠ¢å å¼ä¼˜å…ˆçº§å�?
 NVIC_InitStructure.NVIC_IRQChannel =  EXTI1_IRQn;//æŒ‡å®šä¸­æ–­æ�? 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// æŒ‡å®šå“åº”ä¼˜å…ˆçº§åˆ�?
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
}

void NFC_EXT_Init()
{
	NFC_INT();
	NVIC_Configuration();
	EXIT_Line1_Init();
}

void EXTI1_Config(void)
{
  EXTI_InitTypeDef  EXTI_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef  NVIC_InitStructure; 

  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  /* Configure PB.01 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PB.01 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI1 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_Enable()
{
  EXTI_InitTypeDef  EXTI_InitStructure; 
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void EXTI1_Disable()
{
  EXTI_InitTypeDef  EXTI_InitStructure; 
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
}



