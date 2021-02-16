#include "breath.h"
#include "timer.h"
#include "fft.h"

/**
 * hsv结构
 * v表示亮度
 * s表示颜色深度
 * h表示颜色种类
 * 
 * 百度查看hsv模型便可以看出
 * h用于选择颜色，当h指向了一种颜色，调节s可以改变该颜色的深浅（对比度），s越大，颜色越深，s越小，颜色越浅
 * v则是亮度
*/

/**
 * 呼吸
 * 可以对h呼吸，可以对v呼吸，也可以对s呼吸
 * h呼吸指的是颜色渐变
 * s呼吸指的是颜色深浅渐变
 * v表示亮度渐变
 * 
 * 一个动画只能对一个维度进行呼吸
 * 
 * 呼-亮
 * 吸-暗
 * 
 * max 最大值       范围0-100
 * min 最小值       范围0-100
 * bright 渐亮速度，范围0-100
 * dark 渐暗速度， 范围0-100
*/

static bool __breath(uint8_t *obj, breath_t *breath, breath_type_t type, uint8_t max, uint8_t min, uint8_t bright, uint8_t dark)
{
    data bit state;
    data bool ret = false;

    if (breath->enable == false 
        || IS_TIME_TO(breath->interval) == false)
    {
        return false;
    }

    if (type == E_BREATH_TYPE_H)
    {
        state = breath->state_h;
    }
    else if (type == E_BREATH_TYPE_S)
    {
        state = breath->state_s;
    }
    else /*if (type == E_BREATH_TYPE_V)*/
    {
        state = breath->state_v;

        /**
         * 此举是为了视觉舒适
         * 如果当前亮度突然被调大，那么切换到上升过程
         * 如果当前亮度突然变小，那么切换到下降过程
        */
        if (*obj > breath->v_backup)
        {
            state = true;
        }
        else if (*obj < breath->v_backup)
        {
            state = false;
        }
    }

    if (state == false) //上升
    {
        if (bright > 0)
        {
            *obj += bright;
            if (*obj >= max)
            {
                *obj = max;
            }
        }

        if (*obj == max || bright == 0)
        {
            state = true;
        }
    }
    else        //下降
    {
        if (dark > 0)
        {
            if (*obj > dark)
            {
                *obj -= dark;
            }
            else
            {
                *obj = min;
            }
        }

        if (*obj == min || dark == 0)
        {
            state = false;
            ret = true;
        }
    }

    if (type == E_BREATH_TYPE_H) { breath->state_h = state; }
    else if (type == E_BREATH_TYPE_S) { breath->state_s = state; }
    else /*if (type == E_BREATH_TYPE_V)*/ { breath->state_v = state; breath->v_backup = *obj; }

    return ret;
}

static void _breath_get_parm(uint8_t **obj, hsv_t *hsv, breath_t *breath, breath_type_t type, uint8_t *max, uint8_t *min, uint8_t *d)
{
    if (type == E_BREATH_TYPE_H)
    {
        *min = 0;
        *max = breath->max_h_level * 2;
        *d = breath->d_h;
        *obj = &hsv->h;
    }
    else if (type == E_BREATH_TYPE_S)
    {
        *max = 100;
        *min = 80;
        if (breath->min_s_level > 1) *min = 90;
        *d = breath->d_s;
        *obj = &hsv->s;
    }
    else /*if (type == E_BREATH_TYPE_V)*/
    {
        *min = 0;
        *max = breath->max_v;
        *d = breath->d_v;
        *obj = &hsv->v;
    }
}

bool breath_lamp(hsv_t *hsv, breath_t *breath, breath_type_t type)
{
    data uint8_t *obj, max, min, d;

    _breath_get_parm(&obj, hsv, breath, type, &max, &min, &d);

    return __breath(obj, breath, type, max, min, d, d);
}

/**
 * 简谐振动呼吸
 * 对hsv中的某个维度模拟简谐效果
 * 简谐运动周期公式T = 2*PI*sqrt(l*g)
 * 其中l为吊绳长度，g为重力加速度
 * 在灯条中，sqrt(l*g)的值可以由hsv中的维度模拟
 * 
 * 每个呼吸周期的峰值计算公式：max_v - n * (max_v / 5), n表示第几个呼吸周期（n = 0， 1， 2， 3， 4）
 *     周期峰值需要做归5化处理，也就是要归化到5的倍数集合中，且尽量不失真
 * 
 * 简谐运动就是在固定的呼吸周期情况下，经过多次等周期呼吸，逐渐由亮熄灭
 * 为了让视觉舒适，当由亮到灭后，下一次新的运动时，不能立即从亮开始，会很晃眼睛不舒服
 * 下一次新的运动时，必须再从灭到亮
*/
bool __harmonic_lamp(hsv_t *hsv, breath_t *breath, breath_type_t type, bool is_music, uint8_t fre_index, uint8_t fre_div)
{
    data uint8_t t, max, min, d, *obj, amplitude;
    data float av;

    _breath_get_parm(&obj, hsv, breath, type, &max, &min, &d);

    if (is_music && breath->harmonic_n == 0)
    {
        fft_div(fre_div);
        av = fft_amplitude_get(fre_index);
        av = av / 2.55;
        amplitude = av;
        if (amplitude < max)
        {
            max = amplitude;
        }
        if (type == E_BREATH_TYPE_V)
        {
            breath->max_v = max;
        }
    }

    d = 1;

    t = max / 10;
    max = max - breath->harmonic_n * (max / 5);

    if (t > 0)
    {
        d = max / t;
    }

    if (__breath(obj, breath, type, max, min, d, d))
    {
        breath->harmonic_n = breath->harmonic_n + 1;
        if (breath->harmonic_n == 5)
        {
            breath->harmonic_n = 0;
            if (is_music)
            {
                if (type == E_BREATH_TYPE_V)  breath->max_v = 0;
            }
            return true;
        }
    }

    return false;
}

/******************************* 音乐 ********************************/

bool breath_lamp_music(hsv_t *hsv, breath_t *breath, breath_type_t type, uint8_t fre_index, uint8_t fre_div)
{
#define max fre_index
#define min fre_div
#define d   type
    data uint8_t *obj;
    data uint8_t amplitude;
    data float av;

    if (breath->enable == false 
        || IS_TIME_TO(breath->interval) == false)
    {
        return false;
    }

    fft_div(fre_div);
    av = fft_amplitude_get(fre_index);
    av = av / 2.55;
    amplitude = av;
    _breath_get_parm(&obj, hsv, breath, type, &max, &min, &d);

    if (*obj < amplitude)
    {
        *obj += d;
        if (*obj > max) *obj = max;
    }
    else if (*obj > amplitude)
    {
        if (*obj > d) *obj -= d;
        else *obj = min;

        if (*obj < min) *obj = min;
    }

    return true;
#undef max
#undef min
#undef d
}
