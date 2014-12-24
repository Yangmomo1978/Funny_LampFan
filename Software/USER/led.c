#include "led.h"

void LED_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED_GPIO, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_GPIO, &GPIO_InitStructure);
}

void LED(unsigned char On,unsigned char LED_Name)
{
  unsigned int led_temp=0;
  led_temp = GPIO_ReadOutputData(LED_GPIO);

  if(On)
  {
    switch(LED_Name)
    {
      case D2:
          GPIO_Write(LED_GPIO,led_temp&(~LED2_Red));
          break;
      case D3:
          GPIO_Write(LED_GPIO,led_temp&(~LED3_Green));
          break;
      case D4:
          GPIO_Write(LED_GPIO,led_temp&(~LED4_Bule));
          break;
      case ALL:
          GPIO_Write(LED_GPIO,led_temp&(~(LED2_Red|LED3_Green|LED4_Bule)));
          break;
    }
  }
  else
  {
    switch(LED_Name)
    {
      case D2:
          GPIO_Write(LED_GPIO,led_temp|LED2_Red);
          break;
      case D3:
          GPIO_Write(LED_GPIO,led_temp|LED3_Green);
          break;
      case D4:
          GPIO_Write(LED_GPIO,led_temp|LED4_Bule);
          break;
      case ALL:
          GPIO_Write(LED_GPIO,led_temp|(LED2_Red|LED3_Green|LED4_Bule));
          break;
    }
  }    
}
