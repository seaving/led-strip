#include "drv_adc.h"

void drv_adc_init(void)
{
    //P5M0 &= 0xDF;			//P5.5引脚为高阻输入模式
    P5M0 &= 0xEF;			//P5.4引脚为高阻输入模式
    //P5M1 |= 0x20;			//P5.5引脚为高阻输入模式
    P5M1 |= 0x10;			//P5.4引脚为高阻输入模式
    //ADC_CONTR = 0x05;	    //选定P5.5引脚作为ADC模拟电压输入引脚
    ADC_CONTR = 0x04;	    //选定P5.4引脚作为ADC模拟电压输入引脚
    ADCCFG = 0x02;		    //转换结果左对齐且ADC转换需512个CPU时钟
    ADC_CONTR |= 0x80;      //打开ADC电源
    ADC_CONTR |= 0x40;      //启动ADC转换
}

uint16_t drv_adc_value(void)
{
    data uint16_t adc_value = 0;

    while (! (ADC_CONTR & 0x20)); //等待ADC转换结束
    ADC_CONTR &= 0xDF;          //清零ADC转换结束标志位

    adc_value = ADC_RES << 2;
    adc_value |= ADC_RESL >> 6;   //处理左对齐格式下的ADC结果

    ADC_CONTR |= 0x40;          //启动ADC转换

    return adc_value;
}

#if 0
#include "drv_uart.h"
static void _delay(uint16_t Count)
{
  uint8_t i,j;
  while (Count--)	//Count形参控制延时次数
  {
    for(i=0;i<50;i++)
      for(j=0;j<20;j++);
  }
}

void drv_adc_test(void)
{
    uint16_t adc_v = 0;
    float vin = 0;
    drv_uart_init(35000000, 9600);
    drv_adc_init();
    _delay(1000);
    printf("start>>>>>\r\n");
    while (1)
    {
        adc_v = drv_adc_value();
        vin = adc_v * 0.00322266;
        printf("%d >> ", adc_v);
        printf("%f \r\n", vin);
        _delay(1000);
    }
}
#endif
