#ifndef _MAIN_H
#define _MAIN_H

#include "delay.h"
#include "remote.h"
#include "USART.h"
#include "i2c_ee.h"
#include "i2c_sensor.h"
#include "i2c_rf430cl330h.h"
#include "led.h"
#include "pwm.h"
//#include "gpio.h"
#include "gpio_0.h"

extern unsigned char USART_Receive_queue[USART_Queue_Size];
extern unsigned char USART_Read_ptr;
extern unsigned char USART_Write_ptr;
extern unsigned char USART_fifo_full;
extern unsigned char USART_fifo_empty;


#endif































