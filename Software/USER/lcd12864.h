#ifndef _LCD12864_H
#define _LCD12864_H

#include "stm32f10x.h"
#include "delay.h"
//GPIO_InitTypeDef GPIO_InitStructure;

#define GPIO_LCD    GPIOF   
#define RCC_APB2Periph_GPIO_LCD    RCC_APB2Periph_GPIOF
#define LCD_Data_Pin  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define LCD_Cmd_Pin   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10

#define LCD_DataIn    {GPIO_InitStructure.GPIO_Pin = LCD_Data_Pin;GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;GPIO_Init(GPIO_LCD, &GPIO_InitStructure);}
#define LCD_DataOut   {GPIO_InitStructure.GPIO_Pin = LCD_Data_Pin;GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;GPIO_Init(GPIO_LCD, &GPIO_InitStructure);}
                      

#define LCD2MCU_Data  (u8)(GPIO_ReadInputData(GPIO_LCD)&0x00FF)
//#define MCU2LCD_Data  (u8)(GPIO_ReadOutputData(GPIO_LCD)&0x00FF)

#define LCD_RS_H    GPIO_LCD->BSRR = GPIO_Pin_8 
#define LCD_RS_L    GPIO_LCD->BRR = GPIO_Pin_8
#define LCD_RW_H    GPIO_LCD->BSRR = GPIO_Pin_9 
#define LCD_RW_L    GPIO_LCD->BRR = GPIO_Pin_9
#define LCD_EN_H    GPIO_LCD->BSRR = GPIO_Pin_10
#define LCD_EN_L    GPIO_LCD->BRR = GPIO_Pin_10


void Init_LCD_Pin(void);
void Delay_1ms(void);
void Write_Cmd(u8 cmd);
void  Write_Data(u8 dat);
unsigned char  Read_Data(void);
void Init_Lcd(void);
void Clear_GDRAM(void);
void Draw_TX(u8 Yaddr,u8 Xaddr,const u8 * dp);
void Draw_Pic(const u8  *adder);
void Draw_Point(u8 x,u8 y);
void Draw_Hline(u8 x1,u8 x2,u8 y);
void Draw_Vline(u8 x,u8 y1,u8 y2);
void Print(unsigned char X, unsigned char Y, unsigned char *String);



#endif 