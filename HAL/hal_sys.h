#ifndef __HAL_SYS_H__
#define __HAL_SYS_H__

#include "drv_sys.h"
#include <stdlib.h>

//MCU 进入IDLE模式
#define hal_sys_idle()  drv_sys_idle()

//MCU 进入掉电模式
#define hal_sys_pd()    drv_sys_pd()

#endif
