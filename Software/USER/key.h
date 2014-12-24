#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"
#include "lcd12864.h"

#define GPIO_KEY        GPIOD
#define RCC_APB2Periph_GPIO_KEY    RCC_APB2Periph_GPIOD
#define L_Pin           GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define H_Pin           GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define EXTI_Pin_IRQn   EXTI0_IRQn|EXTI1_IRQn|EXTI2_IRQn|EXTI3_IRQn
#define GPIO_KEYSource  GPIO_PortSourceGPIOD
#define KEY_PinSource   GPIO_PinSource0|GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3
#define EXIT_KEY_Line   EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3

void Key_GPIO_Configuration(void);
void Key_Turn(void);
void Key_Normal(void);
void EXTI0_Config(void);
void EXTI1_Config(void);
void EXTI2_Config(void);
void EXTI3_Config(void);
void Init_Key(void);
void Value_Transform(void);

#endif 