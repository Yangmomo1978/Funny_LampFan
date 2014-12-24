#ifndef __GPIO_0_H
#define __GPIO_0_H       

#include "stm32f10x.h"

#define  PB0   0
#define  PB1   1
#define  PB2   2
#define  PB10  3
#define  PB11  4
#define  PB12  5
#define  PB13  6
#define  PB14  7
#define  PB15  8
#define  PA8   9

void gpio_0_init();
void gpio_0(uint16_t state);
void gpio_1(unsigned char PinX,unsigned char status);

#endif