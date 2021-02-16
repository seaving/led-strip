#ifndef __ACTION_H__
#define __ACTION_H__

#include <stdio.h>
#include <string.h>

#include "typedef.h"
#include "hsv.h"
#include "color_ctrl.h"
#include "position_ctrl.h"

typedef struct __action__
{
    hsv_t               hsv;                    //hsv颜色
    color_ctrl_t        color_ctrl;             //颜色控制
    position_ctrl_t     position_ctrl;          //位移控制

    uint8_t             fre_index;              //音乐频率
} action_t;

#endif
