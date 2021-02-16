#ifndef __HAL_LED_H__
#define __HAL_LED_H__

#include "hal_sys.h"
#include "drv_adc.h"
#include "drv_uc1903_hs.h"

typedef enum __hal_ic_type__
{
    E_HAL_IC_TYPE_UC1903_HS = 0,
    E_HAL_IC_TYPE_UC1903_LS
} hal_ic_type_t;

void hal_led_init(/*hal_led_type_t type*/void);

#define hal_led_ctrl_color(r, g, b) drv_uc1903hs_write(r, g, b)

#endif
