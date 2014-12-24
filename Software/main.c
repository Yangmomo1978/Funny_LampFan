#include "stm32f10x.h"
#include "main.h"

unsigned char NDEF_Application_Data[] =
{
    //NDEF Tag Application Name

    //NDEF Tag Application Name
    0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,
    //Capability Container ID
    0xE1, 0x03,
    //Capability Container
    0x00, 0x0F, //CCLEN
    0x20, //Mapping version 2.0
    0x00, 0x3B, //MLe (49 bytes); Maximum R-APDU data size
    0x00, 0x34, //MLc (52 bytes); Maximum C-APDU data size
    0x04, //Tag, File Control TLV (4 = NDEF file)
    0x06, //Length, File Control TLV (6 = 6 bytes of data for this tag)
    0xE1, 0x04, //File Identifier
    0x0C, 0x02, //Max NDEF size (3072 bytes)
    0x00, //NDEF file read access condition, read access without any security
    0x00, //NDEF file write access condition; write access without any security
    //NDEF File ID
    0xE1, 0x04,
    0x00, 0x44, //NLEN; NDEF length (68 byte long message)
    0xD2, //MB=1b, ME=1b, CF=0b, SR=1b, IL=0b, TNF=010b
    0x20, //Record Type Length: 32 octets
    0x21, //payload length: 33 octets;
    0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x76,
    0x6E, 0x64, 0x2E, 0x62, 0x6C, 0x75, 0x65, 0x74, 0x6F, 0x6F, 0x74, 0x68, 0x2E,
    0x65, 0x70, 0x2E, 0x6F, 0x6F, 0x62, //Record Type Name: application/vnd.blue
    //tooth.ep.oob
    0x21, 0x00, //OOB optional data length: 33 octets
    //0x20, 0x13, 0x09, 0x23, 0x12, 0x87, //bluetooth device address:
    0x87,0x12,0x23,0x09,0x13,0x20,      
    //01:02:03:04:05:06 (example address only)
    0x0D, //EIR Data Length: 13 octets
    0x09, //EIR Data Type: Complete Local Name
    0x48, 0x65, 0x61, 0x64, 0x53, 0x65, 0x74, 0x20, 0x4E, 0x61,0x6D, 0x65, //
    //Bluetooth Local Name: HeadSet Name
    0x04, //EIR Data Length: 4 octets
    0x0D, //EIR Data Type: Class of device
    0x04, 0x04, 0x20, //Class of Device: 0x20:Service Class=
    //Audio, 0x04:Major Device Class=Audio/Video, 0x04: Minor Device Class=Wearable
    //Headset Device
    0x05, //EIR Data Length: 5 octets
    0x03, //EIR Data type: 16-bit Service Class UUID list (complete)
    0x1E, 0x11, 0x0B, 0x11 //16-bit Service Class UUID list (complete) ;0x111E 脙茠脗垄脙垄芒鈧?    //HFP-HF, 0x011B 脙茠脗垄脙垄芒鈧?A2DP-SNK
};

unsigned char func_retrim_osc[] = {
        0xB2, 0x40, 0x11, 0x96, 0x10, 0x01,
        0xB2, 0x40, 0x60, 0x03, 0x18, 0x01,
        0x30, 0x41
};

#define  detect_src_length  2
#define  detect_dst_length  2
extern   unsigned char USART_fifo_full;
extern   unsigned char USART_fifo_empty;
extern unsigned char USART_Rec_Data[USART_Queue_Size];
unsigned char detect_src[detect_src_length]={0x6f,0x70};
unsigned char detect_dst[detect_dst_length];
unsigned char detect_dst_pre[detect_dst_length];

extern unsigned int pulse_width1;

void TIM1_PWM_IN1();

 void GPIOA_10_Init(void);

int main(void)
{ 
  int j=0;
  unsigned char ch=0,ch_pre=0;
  unsigned char brightness=0;//这个值记录所有与台灯相关的亮度信息
  
  SystemInit();
  delay_init(72);
  
  //1、指示灯
  LED_Init();
  LED(OFF,ALL);
  
  //2.1、控制台灯亮度
  PWM_Init(2400,0,0);
  
  //2.2、用于采集台灯的控制信号
  TIM1_PWM_IN1();  
  
  //3、GPIO初始化用于控制风扇
  gpio_0_init();
 
//  //4、NFC的TAG初始化设置
//  NFC初始化
//  Write_Register(CONTROL_REG, SW_RESET);  
//  delay_ms(5);
//  Write_Continuous(0, NDEF_Application_Data, 104);
//  delay_ms(5);
//  Write_Register(INT_ENABLE_REG, EOW_INT_ENABLE + EOR_INT_ENABLE);
//  delay_ms(5);
//  Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE);
  
  //5、蓝牙串口
  Init_USART3();
  //Init_USART1();
  
  while(1)
  {    
    delay_ms(10);
    
    if(pulse_width1!=0)
    {
      if(9==pulse_width1/10)
      {
        ch=2;
      }
      if(37==pulse_width1/10)
      {
        ch=3;
      }
      pulse_width1=0;
    }      
    else
    {
      if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9))
        {
           ch=1;
        }
      
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9))
        {
           ch=4;
        }  
    }
       
    if(ch!=ch_pre)
    {
      if(1==ch)
      {      
        PWM_Init(2400,30,30);
        brightness=30;
      }
      else if(2==ch)
      {
        PWM_Init(2400,60,60);
        brightness=60;
      }
      else if(3==ch)
      {      
        PWM_Init(2400,100,100);
        brightness=100;
      }
      else
      {
        PWM_Init(2400,0,0);
        brightness=0;
      }
    }
    
    ch_pre=ch;
    
    //1、串口缓冲区已经满了，开始解析数据
    if(USART_fifo_full)
    {
      //保存上一次的值
      for(int i=0;i<detect_dst_length;i++)
      {
        detect_dst_pre[i]=detect_dst[i];
      }
      USART_fifo_full=0;
      
      //读取串口队列中的数据
      j=0;
      while(!USART_fifo_empty)
      {
         USART_Rec_Data[j]=USART_fifo_read();
         j++;
      }
      
     USART_fifo_empty=0;
      
     Data_frame_handle(detect_src,detect_src_length,detect_dst,detect_dst_length);//分析数据
      
     //串口打印接收数据
//     for (int i = 0; i < USART_Queue_Size; ++i)
//     {
//       USART_SendData(USART1, detect_dst[i]);
//       delay_ms(10);
//     }
    }
    

    
    // detect_dst[0]  .....灯
    //   0~100        .....占空比数据
    //     10         .....开灯
    //     11         .....关灯
    //     12         .....调亮
    //     13         .....调暗
    
    // detect_dst[1]  .....风扇
    //       0             关闭
    //       1             1档
    //       2             2档
    //       3             3档
    
    //2、根据有效内容，控制灯和风扇
//    if(detect_dst_pre[0]!=detect_dst[0])
//    {
//      //PWM_Init(2400,detect_dst[0],detect_dst[0]);
//      PWM1_Duty_Set(detect_dst[0]);
//    }
    if(0!=detect_dst[0])
    {
      if(0x10==detect_dst[0])
      {
        PWM1_Duty_Set(60); //开灯，调节到始终的亮度
        brightness=60;
      }
      
      if(0x11==detect_dst[0])
      {
        PWM1_Duty_Set(0); //关灯
        brightness=0;
      }
      
      if(0x12==detect_dst[0])
      {
        if(brightness<100)
        {
          brightness=brightness+10;
        }
        PWM1_Duty_Set(brightness); //调亮
      }
      
      if(0x13==detect_dst[0])
      {
        if(brightness>0)
        {
          brightness=brightness-10;
        }
        PWM1_Duty_Set(brightness); //调暗
      }
      
      detect_dst[0]=0;//置位，等待下一次检测
    }
    
    if(0!=detect_dst[1])
    {
      switch(detect_dst[1])
      {
        case 0x41:
          gpio_1(PB0,0);
          gpio_1(PB1,0);
          gpio_1(PB2,0);
          LED(ON,ALL);
          break;
        case 0x42:
          gpio_1(PB0,1);
          gpio_1(PB1,0);
          gpio_1(PB2,0);
          LED(OFF,ALL);
          LED(ON,D2);
          break;
        case 0x43:
          gpio_1(PB0,0);
          gpio_1(PB1,1);
          gpio_1(PB2,0);
          LED(OFF,ALL);
          LED(ON,D3);
          break;
        case 0x44:
          gpio_1(PB0,0);
          gpio_1(PB1,0);
          gpio_1(PB2,1);
          LED(OFF,ALL);
          LED(ON,D4);
          break;
        default:
          break;
      }
      
      detect_dst[1]=0;
      
    }
    
    
  }
}

/*1111111111111111111111111111111111111111111111111111111111111111111111111111
11111111111111111111111111111111111111111111111111111111111111111111111111111111
11111111111111111111111111111111111111111111111111111111111111111111111111111*/
 /**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
   */
 void RCC_Cfg1(void)
 {
   /* TIM1 clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 
   /* GPIOA clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 }
 
 /**
   * @brief  Configure the GPIO Pins.
   * @param  None
   * @retval None
   */
 void GPIO_Cfg1(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM1 channel 2 pin (PA.09) configuration */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//GPIO_Mode_IPD; //GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
 }
 
 /**
   * @brief  Configure the nested vectored interrupt controller.
   * @param  None
   * @retval None
   */
void NVIC_Cfg1(void)
 {
   NVIC_InitTypeDef NVIC_InitStructure;
 
   /* Enable the TIM1 global Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

void Tim1_cfg1()
{
   TIM_ICInitTypeDef  TIM_ICInitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

   /* TIM3 configuration: PWM Input mode ------------------------
      The external signal is connected to TIM4 CH2 pin (PB.07), 
      The Rising edge is used as active edge,
      The TIM4 CCR2 is used to compute the frequency value 
      The TIM4 CCR1 is used to compute the duty cycle value
   ------------------------------------------------------------ */
 
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
   TIM_ICInitStructure.TIM_ICFilter = 0x0;
   TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);
   
   TIM_TimeBaseStructure.TIM_Period = 0xFFFF;     //周期0～FFFF
   TIM_TimeBaseStructure.TIM_Prescaler = 32-1;    //时钟分频，分频数为4分频(尽可以测0~2.7ms的脉宽，故仅适用于舵机信号解码)
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//模式
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//基本初始化
 
   /* Select the TIM3 Input Trigger: TI2FP2 */
   TIM_SelectInputTrigger(TIM1, TIM_TS_TI2FP2);//设置IC2为中断触发源，这个导致一个定时器只能捕捉1个PWM
 
   /* Select the slave Mode: Reset Mode */
   TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
 
   /* Enable the Master/Slave Mode */
   TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
 
   /* TIM enable counter */
   TIM_Cmd(TIM1, ENABLE);
 
   /* Enable the CC2 Interrupt Request */
   TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
}

void TIM1_PWM_IN1()
{
  RCC_Cfg1(); 
  GPIO_Cfg1();
  NVIC_Cfg1();
  Tim1_cfg1();     
}












     






