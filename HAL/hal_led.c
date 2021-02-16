#include "hal_led.h"
#include "drv_delay.h"
#include "drv_uc1903_hs.h"

void hal_led_init(/*hal_led_type_t type*/void)
{
    idata uint8_t i = 0;
#if 0
    if (type == E_HAL_LED_TYPE_UC1903_HS)
    {
#endif
        drv_uc1903hs_init();
        for (i = 0; i < 255; i ++)
            drv_uc1903hs_write(0, 0, 0);
        drv_delay(150);
#if 0
	}
#endif
#if 0
    else if (type == E_HAL_LED_TYPE_UC1903_LS)
    {
        drv_uc1903ls_init();
        for (i = 0; i < 255; i ++)
            drv_uc1903ls_write(0, 0, 0);
    }
#endif
}

#if 0
void hal_led_ctrl_color(hal_led_rgb_t *rgb, hal_led_type_t type)
{
    if (type == E_HAL_LED_TYPE_UC1903_HS)
    {
        drv_uc1903hs_write(rgb->red, rgb->green, rgb->blue);

	}
    else if (type == E_HAL_LED_TYPE_UC1903_LS)
    {
        drv_uc1903ls_write(rgb->red, rgb->green, rgb->blue);
    }
}
#endif
