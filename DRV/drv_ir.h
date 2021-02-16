#ifndef __DRV_IR_H__
#define __DRV_IR_H__

#include "drv_sys.h"

#define drv_ir_data(ir_dat)     ir_dat = 0xFF; if (ir_data != 0xFF) { ir_dat = ir_data; ir_data = 0xFF; }

void drv_ir_init(void);

extern xdata uint8_t ir_data;

#endif
