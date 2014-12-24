#include "USART.h"

//设置接收数据保存的环形队列
unsigned char USART_Receive_queue[USART_Queue_Size];
unsigned char USART_Read_ptr=0;
unsigned char USART_Write_ptr=0;
unsigned char USART_fifo_full=0;
unsigned char USART_fifo_empty=0;

//串口接收数据备份
unsigned char USART_Rec_Data[USART_Queue_Size];


void Init_USART1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
      
    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure); 
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xf;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xf;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Init_USART3(void)
{
      NVIC_InitTypeDef NVIC_InitStructure;
      GPIO_InitTypeDef GPIO_InitStructure;
      USART_InitTypeDef USART_InitStructure;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口时钟配置

      // Configure USART3 Tx (PB.10) as alternate function push-pull 
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB,   &GPIO_InitStructure);
      // Configure USART3 Rx (PB.11) as input floating 
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      USART_InitStructure.USART_BaudRate = 38400;
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits = USART_StopBits_1;
      USART_InitStructure.USART_Parity = USART_Parity_No;
      USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
      USART_Init(USART3, &USART_InitStructure);
      // 使能 USART3
      USART_Cmd(USART3, ENABLE);
      
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xf;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xf;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
      USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
      
}

int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART3, (unsigned char) ch);
  while (!(USART3->SR & USART_FLAG_TXE));
 
  return (ch);
}

//读操作,检测队列是否为空
unsigned char USART_fifo_read()
{
    unsigned char USART_Read_out=0;

    if (USART_Read_ptr!=USART_Write_ptr)
    {
        USART_Read_out=USART_Receive_queue[USART_Read_ptr];
        USART_Read_ptr=(USART_Read_ptr+1)%USART_Queue_Size;
    }  
    else
    {
        USART_fifo_empty=1;
    }  

    return USART_Read_out;
}

//写操作，检测队列是否已满
//在本函数中,(USART_Write_ptr+USART_Queue_Size-1)，这是保留位。
void Uart_fifo_write(unsigned char byte_rec)
{
    if((USART_Write_ptr+1)%USART_Queue_Size!=USART_Read_ptr)
    {
      USART_Receive_queue[USART_Write_ptr]=byte_rec;
      USART_Write_ptr=(USART_Write_ptr+1)%USART_Queue_Size;
    } 
    else
    {
      USART_fifo_full=1;
    }   
}


/*作用：数据包的头检测，以及输出有效数据
输入：   待检测头的SRC数据，
        待检测数据长度
        有效数据存储的数组，
        有效数据的长度
输出：   0  没有找到有效数据
         1  已经找到有效数据  */
unsigned char Data_frame_handle(unsigned char *src,unsigned char src_length,
                       unsigned char *dst,unsigned char dst_length)
{
   unsigned char right_cnt=0;
   unsigned char right_pos=0;

   //寻找匹配的头，以及确认有效数据的开始位置
   for (int i = 0; i < USART_Queue_Size-src_length; ++i)
     {

        right_cnt=0;//先将统计的数据清零
        
        for(int j=0;j<src_length;j++)
        {
            if(*(src+j)==USART_Rec_Data[i+j])
            {
                right_cnt++;
            }
        }

        if(right_cnt==src_length)
        {
            right_pos=i+src_length;
        //    printf("right_pos=%d\r\n",right_pos);
            break;
        }
     }  


    if (0!=right_pos/*如果找到了正确的有效数据起始位置 */)
    {
        int j=0;
        for (int i = right_pos; i < right_pos+dst_length; ++i)
        {
        
            *(dst+j)=USART_Rec_Data[i];
            j++;
            /* code */
        }        
        /* code */
    }


    if (0!=right_pos)
    {
        return 1;
        /* code */
    }
    else
    {
        return 0;
    }

}






