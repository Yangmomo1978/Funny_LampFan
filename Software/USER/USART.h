#ifndef _USART_H
#define _USART_H


#include "stm32f10x.h"
#include <stdio.h>

#define   USART_Queue_Size   5

void Init_USART1(void);
void Init_USART3(void);
void UART1_SendByte(u16 Data);
int fputc(int ch, FILE *f);
int GetKey (void); 

void Uart_fifo_write(unsigned char byte_rec);
unsigned char USART_fifo_read();
unsigned char Data_frame_handle(unsigned char *src,unsigned char src_length,
                       unsigned char *dst,unsigned char dst_length);

#endif