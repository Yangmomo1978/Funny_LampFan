#ifndef _REMOTE_H
#define _REMOTE_H


#include "stm32f10x.h"
#include "USART.h"
#include "delay.h"
#include "i2c_ee.h"



#define   Remote_GPIO            GPIOA
#define   Remote_Receive_Pin     GPIO_Pin_3

void Remote_Send(u32 code);
void GPIO_Config(void);
void TIM2_NVIC_Configuration(void);
void Tim2_Config(void);
void Remote_Init(void);
void Remote_Process(void);
void Cont_Send(void);

#endif