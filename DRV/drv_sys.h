#ifndef __DRV_SYS_H__
#define __DRV_SYS_H__

#include <stdio.h>
#include <intrins.h>

#include "drv_stc.h"
#include "typedef.h"

//MCU 进入IDLE模式，INT0（遥控器）唤醒
#define drv_sys_idle()  _nop_(); _nop_(); _nop_(); PCON |= 0X01; _nop_(); _nop_(); _nop_()

//MCU 进入掉电模式，INT0（遥控器）唤醒
#define drv_sys_pd()    _nop_(); _nop_(); _nop_(); PCON |= 0x02; _nop_(); _nop_(); _nop_()

#endif
