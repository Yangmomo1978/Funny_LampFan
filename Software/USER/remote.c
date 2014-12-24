#include "remote.h"
extern u8 remote_f;
extern u8 send_f;
extern u16 data0,data1;
extern u8  Rec_Data[];
u8 rcode[4];
u8 code[4];
u8 raddress;
u8 waddress;
extern u8 saddress;
extern u8 eaddress;
extern u8 times;
extern u8 nTime;
extern u8 rTime;
u16 count;


void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Remote_Receive_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Remote_GPIO, &GPIO_InitStructure);
	
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void TIM2_NVIC_Configuration(void)
{   
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


void Tim2_Config(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=65535; // 自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler= 7200-1; // 时钟预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; // 采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4 ;
	TIM_ICInitStructure.TIM_ICFilter =0 ;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling ;
	TIM_ICInitStructure.TIM_ICPrescaler= 0 ;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit( TIM2,&TIM_ICInitStructure );

	TIM_ClearITPendingBit(TIM2,  TIM_IT_CC4);
	TIM_ITConfig(TIM2,TIM_IT_CC4,ENABLE);
	TIM_Cmd(TIM2, ENABLE); //开启时钟
}




void TIM4_Init(void)                        
{
    uint16_t                PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef        TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (SystemCoreClock /14400000)-1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 378;
    TIM_TimeBaseStructure.TIM_Prescaler=PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 126;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    
    TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4,ENABLE);
    TIM_Cmd(TIM4,DISABLE);
}


void Remote_Init(void)
{
       GPIO_Config();
       TIM2_NVIC_Configuration();
       Tim2_Config();
       TIM4_Init();
}


void Remote_Process(void)
{

   if(remote_f)
   {
       remote_f=0; 
       code[0]=((u8)((data0&0xff00)>>8));//机器码 
       code[1]=((u8)(data0&0x00ff));//机器反码 
       code[2]=((u8)((data1&0xff00)>>8));//用户码 
       code[3]=((u8)(data1&0x00ff));//用户反码 

       //printf("mcode=%d  mcode_n=%d  ucode=%d  ucode_n=%d\r\n",code[0],code[1],code[2],code[3]);
       printf("%d-%d!",count++,code[2]);
       I2C_EE_BufferWrite(code,waddress,4);
       waddress+=4;
      // printf("读出\r\n");
      // I2C_EE_BufferRead(rcode,EEPADDRESS,4);
       //printf("mcode=%d  mcode_n=%d  ucode=%d  ucode_n=%d\r\n",rcode[0],rcode[1],rcode[2],rcode[3]);
   }
   return;
}



void Remote_Send(u32 code)
{
  unsigned char i;
  TIM_Cmd(TIM4,ENABLE);
  delay_ms(9);
  TIM_Cmd(TIM4,DISABLE);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
  delay_ms(4);
  delay_us(500);
  for(i=0;i<32;i++)
  {
    if(code&0x80000000)             //"1"
    {
      TIM_Cmd(TIM4,ENABLE);
      delay_us(560);
      TIM_Cmd(TIM4,DISABLE);
      //GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
      delay_ms(1);
      delay_us(680);
    }
    else                         //"0"
    {                      
      TIM_Cmd(TIM4,ENABLE);
      delay_us(560);
      TIM_Cmd(TIM4,DISABLE);
      //GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
      delay_us(560);
    }
    code = code<<1;
  }
   TIM_Cmd(TIM4,ENABLE);
   delay_ms(200);
   TIM_Cmd(TIM4,DISABLE);
}


void Cont_Send(void)
{
  if(send_f)
  {
    u8 i,j,k,p;
    send_f = 0;
    //count = 0;
    for(j=0;j<times;j++)
    {
      p = saddress/4;
      for(i=saddress;i<=eaddress;i+=4)
      {
        if(send_f)
          return;
        I2C_EE_BufferRead(rcode,i,4);
        //printf("mcode=%d  mcode_n=%d  ucode=%d  ucode_n=%d\r\n",rcode[0],rcode[1],rcode[2],rcode[3]);
        printf("%d-%d!",p,rcode[2]);
        Remote_Send(rcode[0]<<24|rcode[1]<<16|rcode[2]<<8|rcode[3]);
        for(k=0;k<nTime;k++)
          delay_ms(500);
        if(rTime)
          delay_ms(rTime);
        p++;
      }
    }
    
  }
}