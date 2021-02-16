#ifndef __DRV_UC1903_HS_H__
#define __DRV_UC1903_HS_H__

#include "drv_sys.h"

void drv_uc1903hs_init(void);
void drv_uc1903hs_write(uint8_t red, uint8_t green, uint8_t blue);

#endif
