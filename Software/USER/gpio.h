#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"

#define RCC_RST_GPIO      RCC_APB2Periph_GPIOB
#define NFC_RST_GPIO      GPIOB
#define NFC_RST_Pin       GPIO_Pin_0

void NFC_EXT_Init();
void EXTI1_Config();
void EXTI1_Enable();
void EXTI1_Disable();

#endif