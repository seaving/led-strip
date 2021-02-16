#ifndef __IR_H__
#define __IR_H__

#include "hal_sys.h"
#include "hal_ir.h"

/**
 * 遥控器分布
 * 00 01 02 03
 * 04 05 06 07
 * 08 09 0A 0B
 * 0C 0D 0E 0F
 * 10 11 12 13
 * 14 15 16 17
 * 
 * 亮度加   亮度减  关机    开机
 * 红色     绿色    蓝色    白色
 *  |        |      |      FLUSH
 *  |        |      |      STROBE
 *  |        |      |      FADE
 *  |        |      |      SMOOTH
 * -------------------------------
 *  |->16种静态颜色<-|
 * 说明：
 *  FLUSH   切换
 *  STROBE  呼吸效果，可以设置呼吸速度
 *  FADE    渐变效果，可以设置速度
 *  SMOOTH  平滑效果，可以设置速度
*/

typedef enum __ir_code__
{
    /**
     * 亮度调节
    */
    E_IR_CODE_BRIGHT = 0x00,
    E_IR_CODE_DARKEN = 0x01,

    /**
     * 电源控制
    */
    E_IR_CODE_POWER_OFF = 0x02,
    E_IR_CODE_POWER_ON = 0x03,

    /**
     * 静态颜色
    */
    E_IR_CODE_RED = 0x04,
    E_IR_CODE_GREEN = 0x05,
    E_IR_CODE_BLUE = 0x06,

    E_IR_CODE_RED1 = 0x08,
    E_IR_CODE_GREEN1 = 0x09,
    E_IR_CODE_BLUE1 = 0x0A,

    E_IR_CODE_RED2 = 0x0C,
    E_IR_CODE_GREEN2 = 0x0D,
    E_IR_CODE_BLUE2 = 0x0E,

    E_IR_CODE_RED3 = 0x10,
    E_IR_CODE_GREEN3 = 0x11,
    E_IR_CODE_BLUE3 = 0x12,

    E_IR_CODE_RED4 = 0x14,
    E_IR_CODE_GREEN4 = 0x15,
    E_IR_CODE_BLUE4 = 0x16,

    E_IR_CODE_WHITE = 0x07,
    /**
     * 功能键
    */
   E_IR_CODE_FLUSH = 0x0B,
   E_IR_CODE_STROBE = 0x0F,
   E_IR_CODE_FADE = 0x13,
   E_IR_CODE_SMOOTH = 0x17
} ir_code_t;

#define ir_init()       hal_ir_init()

void ir(void);

#endif
