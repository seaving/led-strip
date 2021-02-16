#ifndef __HSV_H__
#define __HSV_H__

#include "hal_sys.h"

typedef struct __hsv__
{
    uint8_t         h;
    uint8_t         s;
    uint8_t         v;
} hsv_t;

//void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, hsv_t *hsv);
void hsv_to_rgb(hsv_t *hsv, uint8_t *red, uint8_t *green, uint8_t *blue);

#endif
