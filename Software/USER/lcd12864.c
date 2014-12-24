#include "lcd12864.h"


GPIO_InitTypeDef GPIO_InitStructure;
/*******************************************
�������ƣ�Init_LCD_Pin
��    �ܣ���ʼ��LCD�ӿ�
��    ������
����ֵ  ����
********************************************/
void Init_LCD_Pin(void)
{
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LCD, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = LCD_Data_Pin|LCD_Cmd_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIO_LCD, &GPIO_InitStructure);
}


/*******************************************
�������ƣ�Write_Cmd
��    �ܣ���Һ����д��������
��    ����cmd--��������
����ֵ  ����
********************************************/
void Write_Cmd(u8 cmd)
{
    u8 lcdtemp = 0;
    u16  tmpData = 0; 
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;  
    do                       //��æ
    {    
        LCD_EN_H;
        delay_us(1);     
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;
        
    }
    while(lcdtemp & 0x80); 
    
    LCD_DataOut;    
    LCD_RW_L;
    tmpData = GPIO_ReadOutputData(GPIO_LCD); // ���˿����� 
    tmpData &= 0xFF00;                       // �����8λ 
    GPIO_Write(GPIO_LCD, (tmpData|((u16)cmd))); 
    //GPIO_LCD->ODR = cmd;
    LCD_EN_H;
    delay_us(1);           
    LCD_EN_L;
}
/*******************************************
�������ƣ�Write_Data
��    �ܣ���Һ����д��ʾ����
��    ����dat--��ʾ����
����ֵ  ����
********************************************/
void  Write_Data(u8 dat)
{
    u8   lcdtemp = 0;   
    u16  tmpData = 0; 
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //��æ
    {    
        LCD_EN_H;
        delay_us(1);      
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;      
    }
    while(lcdtemp & 0x80);  
    
    LCD_DataOut; 
    LCD_RS_H;
    LCD_RW_L;  
    tmpData = GPIO_ReadOutputData(GPIO_LCD); // ���˿����� 
    tmpData &= 0xFF00;                       // �����8λ 
    GPIO_Write(GPIO_LCD, (tmpData|((u16)dat)));    
    //GPIO_LCD->ODR = dat;
    LCD_EN_H;
    delay_us(1);
    LCD_EN_L;
}  

/*******************************************
�������ƣ�Read_Data
��    �ܣ���ȡҺ���е�����
��    ����
����ֵ  ��shuju
********************************************/
unsigned char  Read_Data(void)
{
  u8 lcdtemp = 0;
  u8 shuju;
  LCD_RS_L;
  LCD_RW_H;
  LCD_DataIn;  
  do                       //��æ
  {    
      LCD_EN_H;
      delay_us(1);
      lcdtemp = LCD2MCU_Data; 
      LCD_EN_L;
      
  }
  while(lcdtemp & 0x80);
  LCD_DataIn;        //����Ϊ���� 
  LCD_RS_H;
  LCD_RW_H;
  LCD_EN_H;
  delay_us(1);
  delay_us(1);
  shuju =  LCD2MCU_Data;
  LCD_EN_L;
  return shuju;
} 
/*******************************************
�������ƣ�Init_Lcd
��    �ܣ���ʼ��Һ��ģ��
��    ������
����ֵ  ����
********************************************/
void Init_Lcd(void)
{                  
    //LCD_CMDOut;    //Һ�����ƶ˿�����Ϊ���
    Init_LCD_Pin();
   // Delay_Nms(500);
    Write_Cmd(0x30);   //����ָ�
    delay_ms(1);
    Write_Cmd(0x02);   // ��ַ��λ
    delay_ms(1);
    Write_Cmd(0x0c);   //������ʾ��,�α�ر�
    delay_ms(1);
    Write_Cmd(0x01);   //�����ʾ
    delay_ms(1);
    Write_Cmd(0x06);   //�α�����
    delay_ms(1);
    Write_Cmd(0x80);   //�趨��ʾ����ʼ��ַ
}
/*******************************************
��������:Clear_GDRAM
��    ��:���Һ��GDRAM�е��������
��    ��:��
����ֵ  :��
********************************************/
void Clear_GDRAM(void)
{
    u8 i,j,k;
    
 Write_Cmd(0x34);        //����չָ�
 i = 0x80;            
 for(j = 0;j < 32;j++)
 {
        Write_Cmd(i++);
        Write_Cmd(0x80);
    for(k = 0;k < 16;k++)
    {
        Write_Data(0x00);
    }
 }
 i = 0x80;
  for(j = 0;j < 32;j++)
 {
      Write_Cmd(i++);
        Write_Cmd(0x88);    
    for(k = 0;k < 16;k++)
    {
         Write_Data(0x00);
     } 
 }   
 Write_Cmd(0x30);        //�ص�����ָ�
}
/*******************************************
�������ƣ�Draw_TX
��    �ܣ���ʾһ��16*16��С��ͼ��
��    ����Yaddr--Y��ַ
          Xaddr--X��ַ
          dp--ָ��ͼ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
********************************************/
void Draw_TX(u8 Yaddr,u8 Xaddr,const u8 * dp) 
{
    u8 j;
    u8 k=0;
 
    Write_Cmd(0x01);  //����,ֻ�����DDRAM
    Write_Cmd(0x34);  //ʹ����չָ����رջ�ͼ��ʾ
    for(j=0;j<16;j++)
    {
        Write_Cmd(Yaddr++);       //Y��ַ
        Write_Cmd(Xaddr);    //X��ַ
        Write_Data(dp[k++]);  
        Write_Data(dp[k++]);
    }
    Write_Cmd(0x36);  //�򿪻�ͼ��ʾ
    Write_Cmd(0x30);  //�ص�����ָ�ģʽ
}



/*******************************************
�������ƣ�Draw_Pic
��    �ܣ���ʾһ��128*64��С��ͼ��
��    ����adder--ָ��ͼ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
********************************************/

void Draw_Pic(const u8  *adder)
{
  u8 i,j; 
  Write_Cmd(0x01);  //����,ֻ�����DDRAM
  Write_Cmd(0x34);  //ʹ����չָ����رջ�ͼ��ʾ
  for(i=0;i<32;i++)
  { 
     Write_Cmd(0x80 + i);       
     Write_Cmd(0x80);
     for(j=0;j<16;j++)     
     {
       Write_Data(*adder);
       adder++;
     }
   } 
  for(i=0;i<32;i++)
  {
      Write_Cmd(0x80 + i);
      Write_Cmd(0x88); 
      for(j=0;j<16;j++)
      {
        Write_Data(*adder);
        adder++;
      }
  }
  Write_Cmd(0x36);  //�򿪻�ͼ��ʾ
  Write_Cmd(0x30);  //�ص�����ָ�ģʽ
}

/*******************************************
�������ƣ�Draw_Point
��    �ܣ���12864������㴦������ͬһ�ֽڿ���ڲ�ͬλ�ĵ��ͬʱ��ʾ
��    ����u8 x,u8 y  ��ĺ�������
����ֵ  ����
********************************************/

void Draw_Point(u8 x,u8 y)
{
     u8 x_dyte,x_byte;//����������һ���ֽڣ���һλ   
     u8 y_dyte,y_byte;//����������һ���ֽڣ���һλ  
     u8 high,low;
     Write_Cmd(0x34);   
     Write_Cmd(0x36);	      //x,y�����껥��������ͨ��x��y����   
     x_dyte=x/16;       //���������һ���ֽڣ���ַ��һ����ַ��16λ��   
     x_byte=x&0x0f;      //���������һλ   
     y_dyte=y/32;       //ȷ�����ϰ��������°���     0:�ϰ�����1�°���   
     y_byte=y&0x1f;      //����0~31���е���һ�У���ȷ�����ڵڼ���       
     Write_Cmd(0x80+y_byte);//��д��ֱ����(���λ����Ϊ1)   
     Write_Cmd(0x80+x_dyte+8*y_dyte);//ˮƽ����  	
     Read_Data();
     high = Read_Data();
     low = Read_Data();
     delay_ms(1);
     Write_Cmd(0x80+y_byte);//��д��ֱ����(���λ����Ϊ1)   
     Write_Cmd(0x80+x_dyte+8*y_dyte);//ˮƽ���� 
     if(x_byte<8)    
     {	 	 
        Write_Data(high|(0x01<<(7-x_byte)));//д���ֽ���Ϊ�����Ǵ����ҵ�     	 
        Write_Data(low);
      }    
    else    
    {     
        Write_Data(high);
        Write_Data(low|(0x01<<(15-x_byte)));      
    }    	 	 
     Write_Cmd(0x30);
}


/*******************************************
�������ƣ�Draw_Hline
��    �ܣ���12864����������仭����
��    ����u8 x1,u8 x2��u8 y  ����ĺ�������
����ֵ  ����
********************************************/
void Draw_Hline(u8 x1,u8 x2,u8 y)
{
  u8 tmp,i;
  if(x2 < x1)
  {
    tmp = x1;
    x1 = x2;
    x2 = tmp;
  }
  for(i = x1; i <= x2; i++)
  {
    Draw_Point(i,y);
  }
}


/*******************************************
�������ƣ�Draw_Vline
��    �ܣ���12864����������仭��ֱ��
��    ����u8 x,u8 y1��u8 y2  ����ĺ�������
����ֵ  ����
********************************************/

void Draw_Vline(u8 x,u8 y1,u8 y2)
{
  u8 tmp,i;
  if(y2 < y1)
  {
    tmp = y1;
    y1 = y2;
    y2 = tmp;
  }
  for(i = y1; i <= y2; i++)
  {
    Draw_Point(x,i);
  }
}

void Print(unsigned char X, unsigned char Y, unsigned char *String)
{
  unsigned char *pi;
  switch (X)
  {
  case 0:
    Write_Cmd(0x80 + Y);
    break;
  case 1:
    Write_Cmd(0x90 + Y);
    break;
  case 2:
    Write_Cmd(0x88 + Y);
    break;
  case 3:
    Write_Cmd(0x98 + Y);
    break;
  default:
    break;
  }
  pi = String;
  while((*pi) != '\0')
    Write_Data(*pi++);   
}