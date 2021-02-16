#ifndef __BREATH_H__
#define __BREATH_H__

#include "typedef.h"
#include "hsv.h"

typedef enum __breath_type__
{
    E_BREATH_TYPE_V = 0,
    E_BREATH_TYPE_S = 1,
    E_BREATH_TYPE_H = 2
} breath_type_t;

typedef struct __breath__
{
    uint8_t         harmonic_n:3;     //震动次数记录
    uint8_t         interval:5;     //用于呼吸间隔(范围0-31)

    uint8_t         enable:1;         //是否启动呼吸
    uint8_t         state_h:1;        //h呼吸状态
    uint8_t         state_s:1;        //s呼吸状态
    uint8_t         state_v:1;        //v呼吸状态
    uint8_t         d_h:4;        //呼吸公差(0-15)
    uint8_t         d_s:3;        //呼吸公差等级(0-6)
    uint8_t         d_v:5;        //呼吸公差等级(0-31)

    uint8_t         max_h_level:6;      //H峰值等级(0-50)
    uint8_t         min_s_level:2;      //S峰谷等级(0-2)，饱和度呼吸最高值固定100，最低值由用户设置，因为100稍微偏低一点，就已经效果不明显了
    uint8_t         max_v;              //峰值等级(0-100)

    uint8_t         v_backup;         //备份当前亮度呼吸对象的值
} breath_t;

bool __harmonic_lamp(hsv_t *hsv, breath_t *breath, breath_type_t type, bool is_music, uint8_t fre_index, uint8_t fre_div);

bool breath_lamp(hsv_t *hsv, breath_t *breath, breath_type_t type);
#define harmonic_lamp(ptrhsv, ptrbreath, type)      __harmonic_lamp(ptrhsv, ptrbreath, type, false, 0, 0)

bool breath_lamp_music(hsv_t *hsv, breath_t *breath, breath_type_t type, uint8_t fre_index, uint8_t fre_div);
#define harmonic_lamp_music(ptrhsv, ptrbreath, type, fre_index, fre_div)    __harmonic_lamp(ptrhsv, ptrbreath, type, true, fre_index, fre_div)

#define BREATH_SET_ENABLE(breath, _enable)                      (breath)->enable = _enable
#define BREATH_SET_INTERVAL(breath, _interval)                  (breath)->interval = _interval

#define BREATH_SET_D_H(breath, d)                               (breath)->d_h = d
#define BREATH_SET_S_H(breath, d)                               (breath)->d_s = d
#define BREATH_SET_V_H(breath, d)                               (breath)->d_v = d

#define BREATH_SET_MAX_H_LEVEL(breath, _level)                  (breath)->max_h_level = _level;
#define BREATH_SET_MIN_S_LEVEL(breath, _level)                  (breath)->min_s_level = _level;
#define BREATH_SET_MAX_V(breath, v)                             (breath)->max_v = v;

#define BREATH_GET_MAX_H(breath)                                ((breath)->max_h_level * 2)
#define BREATH_GET_MIN_S(breath)                                ((breath)->min_s_level > 1 ? 90 : 80)
#define BREATH_GET_MAX_V(breath)                                (breath)->max_v

#define BREATH_H_TO_MAX_H_LEVEL(h, level)                     level = h / 2
#define BREATH_S_TO_MIN_S_LEVEL(s, level)                     level = s / 45

#endif
