#include "drv_ir.h"

#define IRC_110592		0

static data bit _ir_start_flag = 0;			//位变量Start_Flag用于指示是否开始处理
static data uint8_t _ir_time_cnt = 0;		//下降沿之间的时间计数器
static data uint8_t _ir_bit_num = 0;		//变量IR_bit用于指示红外数据的位数
//static data uint32_t _ir_code;			//用户码+用户码+数据码+数据反码
static data uint16_t _ir_code;				//为了节省内存，这里使用的是数据码+数据反码

xdata uint8_t ir_data = 0xFF;				//保持值，0xFF 无效

void drv_ir_init(void)
{
	//定时计数器0初始化函数
	AUXR &= 0x7F;	        //定时器时钟12T模式
	TMOD &= 0xF0;	        //T0工作在工作方式2，8位自动重装载模式
	TMOD |= 0x02;	        //T0工作在工作方式2，8位自动重装载模式
	TL0 = 0x00;		        //定时初值为0，则T0定时时间达到最大（11.0592MB约0.278ms， 35MB约0.088ms）
	TH0 = 0x00;		        //定时初值为0，则T0定时时间达到最大（约0.278ms）
	TF0 = 0;			    //清除TF0标志
	ET0 = 1;				//使能T0的溢出中断
	//TR0 = 1;				//开启T0运行控制位，允许T0开始计数
	                        //定时时间计算：t = 256 * 12 / 35(或11.0592) = 87.77us(或277.77us)

	//外部中断0初始化函数
	IT0 = 1;				//配置外部中断0信号触发方式为边沿触发（下降沿有效）
	EX0 = 1;				//使能INT0中断
	EA = 1;					//使能单片机总中断开关EA
}

void drv_ir_int_isr() interrupt 0
{
	//IE0 = 0;				//清除外部中断标记

	if (_ir_start_flag)
	{
#if IRC_110592
		if (32 < _ir_time_cnt && _ir_time_cnt < 70)	//判断引导码（9ms+4.5ms）
#else
		if (102 < _ir_time_cnt && _ir_time_cnt < 221)	//判断引导码（9ms+4.5ms）
#endif
		{
			//_ir_bit_num大约要溢出32次(9ms/0.278ms)到70次(可以大于(9+4.5)/0.278)
			_ir_bit_num = 0;						//清除位数变量，确保当前IR_bit为0，表示引导码
			_ir_code = 0;
			_ir_time_cnt = 0;

			ir_data = 0xFF;
		}
		else
		{
#if IRC_110592
			if (_ir_time_cnt > 7)				//若“时间间隔”比7大那肯定是“1码”反之为“0码”
#else
			if (_ir_time_cnt > 22)				//若“时间间隔”比7大那肯定是“1码”反之为“0码”
#endif
			{
				_ir_code |= 0x8000;
			}

			if (_ir_bit_num < 31)
			{
				_ir_code >>= 1;
			}

			_ir_time_cnt = 0;							//清零时间宽度计数值

			_ir_bit_num ++;								//位数变量自增
			if(_ir_bit_num == 32)						//如果达到了32位（32个数据位）
			{
				TR0 = 0;									//关闭T0运行控制位，停止T0计数
				_ir_start_flag = 0;						//结束，重新下一轮

				_ir_bit_num = 0;						//位数变量清零
				if ((((_ir_code >> 8) & 0xFF) | (_ir_code & 0xFF)) == 0xFF)
				{
					ir_data = _ir_code & 0xFF;
				}
			}
		}
	}
	else											//外部中断0检测到下降沿，即将开始引导码
	{
		TR0 = 1;									//开启T0运行控制位，允许T0开始计数

		_ir_start_flag = 1;							//红外数据产生第一次下降沿，意味着数据即将开始

		_ir_bit_num = 0;
		_ir_code = 0;
		_ir_time_cnt = 0;

		ir_data = 0xFF;
	}
}

void drv_ir_timer_isr() interrupt 1
{
    /*每过0.278ms则T0溢出一次，就是想要用溢出次数去衡量两个下降沿之间
    的时间长短：前导码9ms+4.5ms，则IR_time大约要溢出32次(9ms/0.278ms)
    到70次(可以大于(9+4.5)/0.278)。如果是“0码”，则周期是1.125ms，则
    IR_time大约要溢出4次(1.125ms/0.278ms)，如果是“1码”，周期是2.25ms，
    则IR_time大约要溢出8次(2.25ms/0.278ms)。这样就可以区分不同的红外位
    含义了.*/
	_ir_time_cnt ++;
}
