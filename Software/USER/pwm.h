#ifndef _PWM_H_
#define _PWM_H_


#include "stm32f10x.h"
#include <stdio.h>

 void PWM_Init(unsigned int fre,unsigned int duty1,unsigned int duty2);
 void PWM_Frequence_Set(unsigned int fre);
 void PWM1_Duty_Set(unsigned int duty1);
 void PWM2_Duty_Set(unsigned int duty2);

 #endif