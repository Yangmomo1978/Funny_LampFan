#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED_GPIO   GPIOB
#define RCC_APB2Periph_LED_GPIO    RCC_APB2Periph_GPIOB

#define LED4_Bule     GPIO_Pin_3
#define LED3_Green    GPIO_Pin_4
#define LED2_Red      GPIO_Pin_5


#define OFF           0
#define ON            1
#define D2            1
#define D3            2
#define D4            3
#define ALL           4

void LED_Init(void);
void LED(unsigned char On,unsigned char LED_Name);


#endif