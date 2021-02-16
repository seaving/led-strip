#include "drv_uart.h"
#if 0
void drv_uart_init(uint32_t sys_clk, uint32_t baud)
{
    SCON = 0x50;	                        //8位数据,可变波特率
    AUXR |= 0x40;	                        //定时器1时钟为Fosc,即1T
    AUXR &= 0xFE;	                        //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F;	                        //设定定时器1为16位自动重装方式
    TL1 = (65536 - (sys_clk / 4 / baud));       //设置波特率重装值
    TH1 = (65536 - (sys_clk / 4 / baud)) >> 8; //设置波特率重装值
    ET1 = 0;			                    //禁止定时器1中断
    TR1 = 1;			                    //启动定时器1
    ES = 1;				                    //使能UART中断开关ES
    EA = 1;				                    //使能单片机总中断开关EA
}

void drv_uart_send_byte(uint8_t byte)
{
	SBUF = byte;		//发送数据到发送缓冲区内
	while (TI == 0);	//等待串口数据发送完毕
	TI = 0;				//清除发送完成标志位TI
}

#if 0
char putchar(char ch)
{ 
    drv_uart_send_byte((uint8_t) ch);		//将Printf内容发往串口
    return (ch);
}
#endif

//extern void uart_read_callback(uint8_t byte);
void drv_uart_isr() interrupt 4 using 1
{
    if (RI)
    {
        //uart_read_callback(SBUF);
        RI = 0;
    }

    if (TI)
    {
        TI = 0;
    }
}
#endif
