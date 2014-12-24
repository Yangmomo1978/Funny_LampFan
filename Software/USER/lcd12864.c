#include "lcd12864.h"


GPIO_InitTypeDef GPIO_InitStructure;
/*******************************************
函数名称：Init_LCD_Pin
功    能：初始化LCD接口
参    数：无
返回值  ：无
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
函数名称：Write_Cmd
功    能：向液晶中写控制命令
参    数：cmd--控制命令
返回值  ：无
********************************************/
void Write_Cmd(u8 cmd)
{
    u8 lcdtemp = 0;
    u16  tmpData = 0; 
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;  
    do                       //判忙
    {    
        LCD_EN_H;
        delay_us(1);     
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;
        
    }
    while(lcdtemp & 0x80); 
    
    LCD_DataOut;    
    LCD_RW_L;
    tmpData = GPIO_ReadOutputData(GPIO_LCD); // 读端口数据 
    tmpData &= 0xFF00;                       // 清掉低8位 
    GPIO_Write(GPIO_LCD, (tmpData|((u16)cmd))); 
    //GPIO_LCD->ODR = cmd;
    LCD_EN_H;
    delay_us(1);           
    LCD_EN_L;
}
/*******************************************
函数名称：Write_Data
功    能：向液晶中写显示数据
参    数：dat--显示数据
返回值  ：无
********************************************/
void  Write_Data(u8 dat)
{
    u8   lcdtemp = 0;   
    u16  tmpData = 0; 
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //判忙
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
    tmpData = GPIO_ReadOutputData(GPIO_LCD); // 读端口数据 
    tmpData &= 0xFF00;                       // 清掉低8位 
    GPIO_Write(GPIO_LCD, (tmpData|((u16)dat)));    
    //GPIO_LCD->ODR = dat;
    LCD_EN_H;
    delay_us(1);
    LCD_EN_L;
}  

/*******************************************
函数名称：Read_Data
功    能：读取液晶中的数据
参    数：
返回值  ：shuju
********************************************/
unsigned char  Read_Data(void)
{
  u8 lcdtemp = 0;
  u8 shuju;
  LCD_RS_L;
  LCD_RW_H;
  LCD_DataIn;  
  do                       //判忙
  {    
      LCD_EN_H;
      delay_us(1);
      lcdtemp = LCD2MCU_Data; 
      LCD_EN_L;
      
  }
  while(lcdtemp & 0x80);
  LCD_DataIn;        //设置为输入 
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
函数名称：Init_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Init_Lcd(void)
{                  
    //LCD_CMDOut;    //液晶控制端口设置为输出
    Init_LCD_Pin();
   // Delay_Nms(500);
    Write_Cmd(0x30);   //基本指令集
    delay_ms(1);
    Write_Cmd(0x02);   // 地址归位
    delay_ms(1);
    Write_Cmd(0x0c);   //整体显示打开,游标关闭
    delay_ms(1);
    Write_Cmd(0x01);   //清除显示
    delay_ms(1);
    Write_Cmd(0x06);   //游标右移
    delay_ms(1);
    Write_Cmd(0x80);   //设定显示的起始地址
}
/*******************************************
函数名称:Clear_GDRAM
功    能:清除液晶GDRAM中的随机数据
参    数:无
返回值  :无
********************************************/
void Clear_GDRAM(void)
{
    u8 i,j,k;
    
 Write_Cmd(0x34);        //打开扩展指令集
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
 Write_Cmd(0x30);        //回到基本指令集
}
/*******************************************
函数名称：Draw_TX
功    能：显示一个16*16大小的图形
参    数：Yaddr--Y地址
          Xaddr--X地址
          dp--指向图形数据存放地址的指针
返回值  ：无
********************************************/
void Draw_TX(u8 Yaddr,u8 Xaddr,const u8 * dp) 
{
    u8 j;
    u8 k=0;
 
    Write_Cmd(0x01);  //清屏,只能清除DDRAM
    Write_Cmd(0x34);  //使用扩展指令集，关闭绘图显示
    for(j=0;j<16;j++)
    {
        Write_Cmd(Yaddr++);       //Y地址
        Write_Cmd(Xaddr);    //X地址
        Write_Data(dp[k++]);  
        Write_Data(dp[k++]);
    }
    Write_Cmd(0x36);  //打开绘图显示
    Write_Cmd(0x30);  //回到基本指令集模式
}



/*******************************************
函数名称：Draw_Pic
功    能：显示一个128*64大小的图形
参    数：adder--指向图形数据存放地址的指针
返回值  ：无
********************************************/

void Draw_Pic(const u8  *adder)
{
  u8 i,j; 
  Write_Cmd(0x01);  //清屏,只能清除DDRAM
  Write_Cmd(0x34);  //使用扩展指令集，关闭绘图显示
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
  Write_Cmd(0x36);  //打开绘图显示
  Write_Cmd(0x30);  //回到基本指令集模式
}

/*******************************************
函数名称：Draw_Point
功    能：在12864的任意点处画点且同一字节宽度内不同位的点可同时显示
参    数：u8 x,u8 y  点的横纵坐标
返回值  ：无
********************************************/

void Draw_Point(u8 x,u8 y)
{
     u8 x_dyte,x_byte;//横坐标在哪一个字节，哪一位   
     u8 y_dyte,y_byte;//纵坐标在哪一个字节，哪一位  
     u8 high,low;
     Write_Cmd(0x34);   
     Write_Cmd(0x36);	      //x,y的坐标互换，即普通的x，y坐标   
     x_dyte=x/16;       //算出它在哪一个字节（地址）一个地址是16位的   
     x_byte=x&0x0f;      //算出他在哪一位   
     y_dyte=y/32;       //确定是上半屏还是下半屏     0:上半屏，1下半屏   
     y_byte=y&0x1f;      //计算0~31当中的哪一行，即确定是在第几行       
     Write_Cmd(0x80+y_byte);//先写垂直坐标(最高位必须为1)   
     Write_Cmd(0x80+x_dyte+8*y_dyte);//水平坐标  	
     Read_Data();
     high = Read_Data();
     low = Read_Data();
     delay_ms(1);
     Write_Cmd(0x80+y_byte);//先写垂直坐标(最高位必须为1)   
     Write_Cmd(0x80+x_dyte+8*y_dyte);//水平坐标 
     if(x_byte<8)    
     {	 	 
        Write_Data(high|(0x01<<(7-x_byte)));//写高字节因为坐标是从左到右的     	 
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
函数名称：Draw_Hline
功    能：在12864的任意两点间画横线
参    数：u8 x1,u8 x2，u8 y  两点的横纵坐标
返回值  ：无
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
函数名称：Draw_Vline
功    能：在12864的任意两点间画垂直线
参    数：u8 x,u8 y1，u8 y2  两点的横纵坐标
返回值  ：无
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