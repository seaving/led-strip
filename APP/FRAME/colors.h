#ifndef __COLORS_H__
#define __COLORS_H__

#include "typedef.h"

#define COLOR_HSV_ARRAY_ROWS     8
#define COLOR_HSV_ARRAY_COL      3

#define COLOR_HSV_ARRAY1_ROWS    16

typedef enum __color_index__
{
    E_COLOR_RED = 0,            //红
    E_COLOR_ORANGE = 1,         //橙
    E_COLOR_YELLOW = 2,         //黄
    E_COLOR_GREEN = 3,          //绿
    E_COLOR_BLUE = 4,           //蓝
    E_COLOR_INDIGO = 5,         //靛
    E_COLOR_PURPLE = 6          //紫
} color_index_t;

#define COLOR_HSV_H(color)      color_hsv_array[color][0]
#define COLOR_HSV_S(color)      color_hsv_array[color][1]
#define COLOR_HSV_V(color)      color_hsv_array[color][2]

#define COLOR1_HSV_H(color)      color_hsv_array1[color][0]
#define COLOR1_HSV_S(color)      color_hsv_array1[color][1]
#define COLOR1_HSV_V(color)      color_hsv_array1[color][2]

extern code uint8_t color_hsv_array[COLOR_HSV_ARRAY_ROWS][COLOR_HSV_ARRAY_COL];
extern code uint8_t color_hsv_array1[COLOR_HSV_ARRAY1_ROWS][COLOR_HSV_ARRAY_COL];

#endif
