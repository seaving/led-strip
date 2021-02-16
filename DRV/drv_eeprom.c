#include "drv_eeprom.h"
#if 0
sfr DATA1 = 0xC2;   //数据寄存器
sfr ADDRH = 0xC3;   //地址寄存器高八位
sfr ADDRL = 0xC4;   //地址寄存器低八位
sfr CMD = 0xC5;     //命令模式寄存器
sfr TRIG = 0xC6;    //命令触发寄存器
sfr CONTR = 0xC7;   //控制寄存器
sfr TPS = 0xF5;     //等待时间控制继存器

/*
由ISP工具设置eeprom大小，eeprom总是从后向前规划，
规划好后，读取和写入的地址是从相对地址开始（0开始）
此代码只支持512字节eeprom操作
*/

uint8_t drv_eeprom_read_byte(uint16_t address)
{
    CONTR = 0x82;           //定义控制寄存器工作时间，打开控制寄存器，晶体为35M
    TPS = 35;               //晶振为35MHZ
    CMD = 0x01;             //选择读的方式
    ADDRH = (address >>= 8);    //得到地址高八位
    ADDRL = address & (0x00ff); //得到地址低八位
    TRIG = 0x5a;            //控制码
    TRIG = 0xa5;            //控制码
    _nop_();                //延时
    CONTR = 0x00;           //复位控制寄存器
    TRIG = 0X00;            //复位方式寄存器
    return DATA1;           //将值返回
}

void drv_eeprom_write_byte(uint16_t address, uint8_t value)
{
    CONTR = 0x82;       //定义控制寄存器工作时间，打开控制寄存器，晶体为12M
    TPS = 35;           //晶振为35MHZ
    CMD = 0X02;         //选择写的方式
    ADDRH = (address >>= 8);    //得到地址高八位
    ADDRL = address & (0x00ff); //得到地址低八位
    DATA1 = value;      //将要存的数放进存数寄存器DATA
    TRIG = 0x5a;        //控制码
    TRIG = 0xa5;        //控制码
    _nop_();           //延时
    CONTR = 0x00;
    TRIG = 0X00;
}

void drv_eeprom_write(uint16_t address, uint8_t *buf, uint8_t len)
{
    uint8_t i = 0;
    for (i = 0; i < len; i ++)
    {
        drv_eeprom_write_byte(address + i, buf[i]);
    }
}

void drv_eeprom_erase_page(void)
{
    CONTR = 0x82;
    TPS = 35;           //晶振为35MHZ
    CMD = 0x03;         //选择寄存器方式
    ADDRH = 0x0000;
    ADDRL= 0x0000;
    TRIG = 0x5a;
    TRIG = 0xa5;
    _nop_();
    CONTR = 0x00;
    TRIG = 0X00;
}
#endif

#if 0

void delay(uint16_t Count)
{
  uint8_t i,j;
  while (Count--)	//Count形参控制延时次数
  {
    for(i=0;i<50;i++)
      for(j=0;j<20;j++);
  }
}

void main(void)
{
    uint16_t aa = 0;
    drv_uart_init(35000000, 9600);
    delay(1000);
    aa = drv_eeprom_read(0);
    if (aa == 126)
    {
        drv_eeprom_erase_page();
        drv_eeprom_write(0, 203);
        printf("----is 126---\r\n");
    }
    else
    {
        drv_eeprom_erase_page();
        drv_eeprom_write(0, 126);
        printf("----not 126---\r\n");
    }

    printf("----start---\r\n");
    while (1)
    {
        aa = drv_eeprom_read(0);
        printf("%u\r\n", aa);
        delay(1000);
    }
}

#endif
