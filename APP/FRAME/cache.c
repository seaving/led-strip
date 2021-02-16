#include "cache.h"

xdata uint8_t   cache_r[2][LED_MAX_NUM] = {0};     //显存
xdata uint8_t   cache_g[2][LED_MAX_NUM] = {0};     //显存
xdata uint8_t   cache_b[2][LED_MAX_NUM] = {0};     //显存

xdata action_t  actions[ACTION_COUNT] = {0};      //动画缓存

data uint8_t    ic_num = 50;                    //实际灯条IC数量
//data ic_type_t  ic_type = E_IC_TYPE_UCS1903_HS;    //IC型号

void cache_set_pixel(uint8_t index, uint8_t addr, uint8_t r, uint8_t g, uint8_t b)
{
    cache_set_pixel_r(index, addr, r);
    cache_set_pixel_g(index, addr, g);
    cache_set_pixel_b(index, addr, b);
}

void cache_clear(uint8_t index)
{
    memset(&cache_r[index][0], 0, ic_num);
    memset(&cache_r[index][0], 0, ic_num);
    memset(&cache_r[index][0], 0, ic_num);
}

void cache_set(uint8_t index, uint8_t start, uint8_t len, uint8_t r, uint8_t g, uint8_t b)
{
    memset(&cache_r[index][0] + start, r, len);
    memset(&cache_g[index][0] + start, g, len);
    memset(&cache_b[index][0] + start, b, len);  
}
