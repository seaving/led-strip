#include "color_ctrl.h"
#include "timer.h"
#include "colors.h"
#include "cache.h"

data bright_adjust_t bright_adjust = {0};

#if 0
static void __bright_adjust(uint8_t *obj)
{
    if (bright_adjust.adjust <= 0)
        return;

    if (bright_adjust.is_raise)
    {
        //增加亮度
        *obj += bright_adjust.adjust;
        if (*obj > 100) *obj = 100;
    }
    else
    {
        //减小亮度
        if (*obj > bright_adjust.adjust) *obj = *obj - bright_adjust.adjust;
        else *obj = 0;
    }
}
#else
#define __bright_adjust(obj) if (*obj > bright_adjust.adjust) *obj = bright_adjust.adjust;
#endif

#define _color_adorn_random(ctrl) \
    if (ctrl->adorn_random == true && IS_TIME_TO(ctrl->random_interval_level * 10)) \
    { \
        ctrl->adorn_index = (ctrl->adorn_index + 1) % COLOR_HSV_ARRAY_ROWS; \
    }

static bool _color_random(hsv_t *hsv, color_ctrl_t *ctrl, bool need_h, bool need_s, bool need_v)
{
    if (IS_TIME_TO(ctrl->random_interval_level * 10))
    {
        ctrl->random_index = (ctrl->random_index + 1) % COLOR_HSV_ARRAY_ROWS;

        if (need_h) hsv->h = COLOR_HSV_H(ctrl->random_index);
        if (need_s) hsv->s = COLOR_HSV_S(ctrl->random_index);
        if (need_v) hsv->v = COLOR_HSV_V(ctrl->random_index);

        return true;
    }

    return false;
}

/**
 * 颜色控制
 * 方式参考 color_ctrl_type_t
*/

static bool __color_ctrl(hsv_t *hsv, color_ctrl_t *ctrl)
{
    data bool ret = false;

    BREATH_SET_ENABLE(&ctrl->breath, true);

    switch (ctrl->ctrl_type)
    {
        /**
         * 单色
        */
        case E_COLOR_CTRL_TYPE_SINGLE_KEEP:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_SINGLE_BREATH_V:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;
        case E_COLOR_CTRL_TYPE_SINGLE_HARMONIC_V:
            ret = harmonic_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;
        case E_COLOR_CTRL_TYPE_SINGLE_BREATH_S:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_S);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_SINGLE_HARMONIC_S:
            ret = harmonic_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_S);
            __bright_adjust(&hsv->v);
            break;

        /**
         * 随机颜色
        */
        case E_COLOR_CTRL_TYPE_RANDOM_KEEP:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            ret = _color_random(hsv, ctrl, true, true, true);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_RANDOM_BREATH_V:
            _color_random(hsv, ctrl, true, true, false);
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;
        case E_COLOR_CTRL_TYPE_RANDOM_HARMONIC_V:
            _color_random(hsv, ctrl, true, true, false);
            ret = harmonic_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;
        case E_COLOR_CTRL_TYPE_RANDOM_BREATH_S:
            _color_random(hsv, ctrl, true, false, true);
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_S);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_RANDOM_HARMONIC_S:
            _color_random(hsv, ctrl, true, false, true);
            ret = harmonic_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_S);
            __bright_adjust(&hsv->v);
            break;

        /**
         * 颜色渐变
        */
        case E_COLOR_CTRL_TYPE_SMOOTH_HSV:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_H);
            breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_S);
            breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_H);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_SMOOTH_BREATH_HV:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_H);
            breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;
        case E_COLOR_CTRL_TYPE_SMOOTH_HARMONIC_V:
            ret = breath_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_H);
            harmonic_lamp(hsv, &ctrl->breath, E_BREATH_TYPE_V);
            break;

        default:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            break;
    }

    return ret;
}

static bool __color_ctrl_music(hsv_t *hsv, color_ctrl_t *ctrl, uint8_t fre_index, uint8_t fre_div)
{
    data bool ret = false;

    BREATH_SET_ENABLE(&ctrl->breath, true);

    switch (ctrl->ctrl_type)
    {
        /**
         * 单色
        */
        case E_COLOR_CTRL_TYPE_MUSIC_SINGLE_KEEP:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_SINGLE_BREATH_V:
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_SINGLE_HARMONIC_V:
            ret = harmonic_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_SINGLE_BREATH_S:
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_S, fre_index, fre_div);
            __bright_adjust(&hsv->v);
            break;

        /**
         * 随机颜色
        */
        case E_COLOR_CTRL_TYPE_MUSIC_RANDOM_KEEP:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            ret = _color_random(hsv, ctrl, true, true, true);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_RANDOM_BREATH_V:
            _color_random(hsv, ctrl, true, true, false);
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_RANDOM_HARMONIC_V:
            _color_random(hsv, ctrl, true, true, false);
            ret = harmonic_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_RANDOM_BREATH_S:
            _color_random(hsv, ctrl, true, false, true);
            ret = harmonic_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_S, fre_index, fre_div);
            __bright_adjust(&hsv->v);
            break;

        /**
         * 颜色渐变
        */
        case E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_BREATH_H:
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_H, fre_index, fre_div);
            __bright_adjust(&hsv->v);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_BREATH_HV:
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_H, fre_index, fre_div);
            breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;
        case E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_HARMONIC_V:
            ret = breath_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_H, fre_index, fre_div);
            harmonic_lamp_music(hsv, &ctrl->breath, E_BREATH_TYPE_V, fre_index, fre_div);
            break;

        default:
            BREATH_SET_ENABLE(&ctrl->breath, false);
            break;
    }

    return ret;
}

bool color_ctrl(hsv_t *hsv, color_ctrl_t *ctrl, uint8_t fre_index, uint8_t fre_div)
{
    data uint8_t max;

    max = BREATH_GET_MAX_V(&ctrl->breath);
    __bright_adjust(&max);
    BREATH_SET_MAX_V(&ctrl->breath, max);

    //装饰色随机处理
    _color_adorn_random(ctrl);

    if (ctrl->ctrl_type >= E_COLOR_CTRL_TYPE_MUSIC_SINGLE_KEEP)
    {
        return __color_ctrl_music(hsv, ctrl, fre_index, fre_div);
    }

    return __color_ctrl(hsv, ctrl);
}

void color_ctrl_get_adorn_hsv(hsv_t *hsv, color_ctrl_t *ctrl)
{
    hsv->h = COLOR_HSV_H(ctrl->adorn_index);
    hsv->s = COLOR_HSV_S(ctrl->adorn_index);
    hsv->v = COLOR_HSV_V(ctrl->adorn_index);
    __bright_adjust(&hsv->v);
}
