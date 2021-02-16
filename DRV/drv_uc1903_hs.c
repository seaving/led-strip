#include "drv_uc1903_hs.h"
#include "drv_delay.h"

sbit led_pwm_pin = P3^3;

#define DRV_LED_PIN_INIT()              P3M0 |= 0x08;P3M1 &= 0xF7
#define DRV_LED_PIN_H()                 led_pwm_pin = 1
#define DRV_LED_PIN_L()                 led_pwm_pin = 0

void drv_uc1903hs_init(void)
{
    data uint8_t drv_uc1903hs_reset_delay = 0;
    DRV_LED_PIN_INIT();
    DRV_LED_PIN_L();
    for (drv_uc1903hs_reset_delay = 0; 
        drv_uc1903hs_reset_delay < 50; 
        drv_uc1903hs_reset_delay ++)
    {
        drv_delay(200);
    }
}

void drv_uc1903hs_write_byte(uint8_t color)
{
#define __drv_uc1903hs_send_bit__(b) \
    if (b) \
    { \
        DRV_LED_PIN_H(); \
        _nop_(); _nop_(); _nop_(); _nop_(); \
        drv_delay(1); \
        DRV_LED_PIN_L(); \
        _nop_(); _nop_(); \
    } \
    else \
    { \
        DRV_LED_PIN_H(); \
        _nop_();_nop_();_nop_();_nop_();_nop_(); _nop_(); \
        DRV_LED_PIN_L(); \
        _nop_(); _nop_(); \
        drv_delay(1); \
    }

#define __drv_uc1903hs_write_byte__(color) \
	__drv_uc1903hs_send_bit__(color & 0x80); \
    __drv_uc1903hs_send_bit__(color & 0x40); \
    __drv_uc1903hs_send_bit__(color & 0x20); \
    __drv_uc1903hs_send_bit__(color & 0x10); \
    __drv_uc1903hs_send_bit__(color & 0x08); \
    __drv_uc1903hs_send_bit__(color & 0x04); \
    __drv_uc1903hs_send_bit__(color & 0x02); \
    __drv_uc1903hs_send_bit__(color & 0x01);

    __drv_uc1903hs_write_byte__(color);

#undef _drv_uc1903hs_write_byte
#undef __drv_uc1903hs_send_bit__
}

void drv_uc1903hs_write(uint8_t red, uint8_t green, uint8_t blue)
{
    drv_uc1903hs_write_byte(blue);
    drv_uc1903hs_write_byte(red);
    drv_uc1903hs_write_byte(green);
}
