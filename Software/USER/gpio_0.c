#include "stm32f10x.h"
#include "gpio_0.h"



static uint16_t gpio_data[10] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_8};

//enum PinX
//{
//   PB0,PB1,PB2,PB10,PB11,PB12,PB13,PB14,PB15,PA8 
//};

//00 0000 0000
void gpio_0_init()
{
    
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/* Configure PC.04, PC.05, PC.06 and PC.07 as Output push-pull */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

}
//从高位到低位 : PB0 PB1 PB2 PB10 PB11 PB12 PB13 PB14 PB15 PA8 
//state 对应有效位为低10位
void gpio_0(uint16_t state)
{
    uint16_t cycle,data = 0;

    for(cycle = 1;cycle < 10;cycle++)
    {
      data = (state>>cycle)&0x0001;
      if(data)
      {
        GPIO_SetBits(GPIOB,gpio_data[9-cycle]);
      }
      else
      {
        GPIO_ResetBits(GPIOB,gpio_data[9-cycle]);
      }
    }
    data = state&0x01;
    if(data)
      GPIO_SetBits(GPIOA,gpio_data[9]);
    else
      GPIO_ResetBits(GPIOA,gpio_data[9]);
}

void gpio_1(unsigned char PinX,unsigned char status)
{
  if(9!=PinX)
  {
    if(status)
    {
      GPIO_SetBits(GPIOB,gpio_data[PinX]);
    }
    else
    {
      GPIO_ResetBits(GPIOB,gpio_data[PinX]);
    }
  }
  else
  {
    if(status)
      {
        GPIO_SetBits(GPIOA,gpio_data[PinX]);
      }
    else
      {
        GPIO_ResetBits(GPIOA,gpio_data[PinX]);
      }  
  }
}
