 
 /* Includes ------------------------------------------------------------------*/
#include "pwm.h"
 
 unsigned int  PWM_Frequence=0;
 /**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
   */
 void RCC_Configuration(void)
 {
   /* TIM3 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
   /* GPIOA and GPIOB clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                          RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
 }
 
 /**
   * @brief  Configure the TIM3 Ouput Channels.
   * @param  None
   * @retval None
   */
 void GPIO_Configuration(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
   /* GPIOA Configuration:TIM3 Channel1, 2, 3 and 4 as alternate function push-pull */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
   GPIO_Init(GPIOA, &GPIO_InitStructure);
 }
 

/* -----------------------------------------------------------------------
TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
clock at 24 MHz the Prescaler is computed as following:
 - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
and Connectivity line devices and to 24 MHz for Low-Density Value line and
Medium-Density Value line devices
The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                              = 24 MHz / 666 = 36 KHz
TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
--------------------------------------------------------------------- */
 void PWM_Init(unsigned int fre,unsigned int duty1,unsigned int duty2)
 {

   RCC_Configuration();
   GPIO_Configuration();

   unsigned int  PrescalerValue = 0; 
   TIM_OCInitTypeDef  TIM_OCInitStructure; 
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

   PWM_Frequence=fre;

   /* Compute the prescaler value */
   PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 24000000/fre;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);		

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = 24000000/fre*duty1/100;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

   TIM_OC1Init(TIM3, &TIM_OCInitStructure);

   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = 24000000/fre*duty2/100;
 
   TIM_OC2Init(TIM3, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

   TIM_Cmd(TIM3,ENABLE);                                                           
   TIM_CtrlPWMOutputs(TIM3, ENABLE); 
 }

 /**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
   */

void PWM_Frequence_Set(unsigned int fre)
{
	TIM_SetAutoreload(TIM3, 24000000/fre);
}

void PWM1_Duty_Set(unsigned int duty1)
{
    TIM_SetCompare1(TIM3, 24000000/PWM_Frequence*duty1/100);
}

void PWM2_Duty_Set(unsigned int duty2)
{
	TIM_SetCompare2(TIM3, 24000000/PWM_Frequence*duty2/100);
}


 

