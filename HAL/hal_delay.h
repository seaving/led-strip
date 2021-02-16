#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__

#include "hal_sys.h"
#include "drv_delay.h"

#define hal_delay(nop_cnt)      drv_delay(nop_cnt)

#endif
