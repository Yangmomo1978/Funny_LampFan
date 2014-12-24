#include "key.h"


extern u8 KeyValue;
u8        KeyNumber;
u16       tempdata;
unsigned char study_flag;


void Key_GPIO_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_KEY, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = L_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = H_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  tempdata = GPIO_ReadOutputData(GPIO_KEY);
  tempdata &= 0xFF0F;  
  GPIO_Write(GPIO_KEY, tempdata);
}


void Key_Turn(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = L_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = H_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  tempdata = GPIO_ReadOutputData(GPIO_KEY);
  tempdata &= 0xFFF0;  
  GPIO_Write(GPIO_KEY, tempdata);
  
  
}

void Key_Normal(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;  
  
  GPIO_InitStructure.GPIO_Pin = L_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = H_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_KEY, &GPIO_InitStructure);
  
  tempdata = GPIO_ReadOutputData(GPIO_KEY);
  tempdata &= 0xFF0F;  
  GPIO_Write(GPIO_KEY, tempdata);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}





void EXTI0_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_EXTILineConfig(GPIO_KEYSource, GPIO_PinSource0); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI1_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  GPIO_EXTILineConfig(GPIO_KEYSource, GPIO_PinSource1); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI2_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  GPIO_EXTILineConfig(GPIO_KEYSource, GPIO_PinSource2); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI3_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  GPIO_EXTILineConfig(GPIO_KEYSource, GPIO_PinSource3); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void Init_Key(void)
{
  Key_GPIO_Configuration();
  EXTI0_Config();
  EXTI1_Config();
  EXTI2_Config();
  EXTI3_Config();
}


void Value_Transform(void)
{
  switch (KeyValue)                
    {
     case 0xee: KeyNumber = 0;break;                 
     case 0xde: KeyNumber = 1;break; 
     case 0xbe: KeyNumber = 2;break;
     case 0x7e: KeyNumber = 3;break;
     case 0xed: KeyNumber = 4;break;                 
     case 0xdd: KeyNumber = 5;break; 
     case 0xbd: KeyNumber = 6;break;
     case 0x7d: KeyNumber = 7;break;
     case 0xeb: KeyNumber = 8;break;                 
     case 0xdb: KeyNumber = 9;break; 
     case 0xbb: KeyNumber = 10;break;
     case 0x7b: KeyNumber = 11;break;
     case 0xe7: KeyNumber = 12;break;                 
     case 0xd7: KeyNumber = 13;break; 
     case 0xb7: KeyNumber = 14;break;
     case 0x77: KeyNumber = 15;break;
     default:   break;                                     
    }
  if(KeyNumber==15)
  {
    Write_Cmd(0x01);
    Print(0,1,"学习型遥控器");
    Print(1,1,"进入学习状态");
    Print(2,0,"输入学习键值");
    TIM_ITConfig(TIM2,TIM_IT_CC4,ENABLE);
    study_flag = 1;
    return;
  }
  else if(KeyNumber==14)
  {
    Write_Cmd(0x01);
    Print(0,1,"学习型遥控器");
    Print(1,1,"退出学习状态");
    TIM_ITConfig(TIM2,TIM_IT_CC4,DISABLE);
    study_flag = 0;
    return;
  }
  
  
  if(study_flag)
  {
    Write_Data(0x30+KeyNumber/10);
    Write_Data(0x30+KeyNumber%10);
  }
    
  
}

