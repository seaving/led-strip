#ifndef __CACHE_H__
#define __CACHE_H__

#include "action.h"
#include "hal_delay.h"
#include "hal_led.h"

typedef hal_ic_type_t   ic_type_t;

#define LED_MAX_NUM         50         //最大驱动的幻彩IC数量，太大的话，会耗时太久，阻塞太久

#define ACTION_COUNT        7

#define cache_get_pixel_r(index, addr)    cache_r[index][addr]
#define cache_get_pixel_g(index, addr)    cache_g[index][addr]
#define cache_get_pixel_b(index, addr)    cache_b[index][addr]

#define cache_set_pixel_r(index, addr, r)       cache_r[index][addr] = r
#define cache_set_pixel_g(index, addr, g)       cache_g[index][addr] = g
#define cache_set_pixel_b(index, addr, b)       cache_b[index][addr] = b

#define cache_refresh(index, i) \
    for (i = 0; i < ic_num; i ++) \
            hal_led_ctrl_color(cache_r[index][i], cache_g[index][i], cache_b[index][i]); \
        hal_delay(150);    //单片机频率较快，不延时会导致乱显示，延时30us以上

void cache_set(uint8_t index, uint8_t start, uint8_t len, uint8_t r, uint8_t g, uint8_t b);
void cache_set_pixel(uint8_t index, uint8_t addr, uint8_t r, uint8_t g, uint8_t b);
void cache_clear(uint8_t index);

extern xdata uint8_t   cache_r[2][LED_MAX_NUM];
extern xdata uint8_t   cache_g[2][LED_MAX_NUM];
extern xdata uint8_t   cache_b[2][LED_MAX_NUM];

extern xdata action_t actions[ACTION_COUNT];

extern data uint8_t    ic_num;
//extern data ic_type_t  ic_type;

#endif
