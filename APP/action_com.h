#ifndef __ACTION_COM_H__
#define __ACTION_COM_H__

#include "hsv.h"
#include "position_ctrl.h"

typedef enum __fade_type__
{
    E_FADE_TYPE_KEEP = 0,
    E_FADE_TYPE_BREATH,
    E_FADE_TYPE_FADE,
    E_FADE_TYPE_SMOOTH
} fade_type_t;

typedef struct __flush_param__
{
    uint8_t         mode;
    uint8_t         position;
    uint8_t         fade_type;
    uint8_t         fade_interval;
    uint8_t         is_static_color:1;
    uint8_t         color_index:7;
    uint8_t         is_raise_adjust:1;
    uint8_t         adjust:7;
    uint8_t         position_interval;
    uint8_t         bright_interval_len:4;
    uint8_t         dark_interval_len:4;
} flush_param_t;

typedef struct __action_conf__
{
    hsv_t           hsv;                        //颜色
    uint8_t         start;                      //区间范围起点
    uint8_t         end;                        //区间范围终点

    uint8_t         color_type:7;               //特效颜色控制方式(0-127)
    uint8_t         adorn_random:1;             //装饰色是否随机(0-1)
    uint8_t         adorn_index:4;              //装饰色索引(0-15)
    uint8_t         adorn_random_level:4;       //装饰色随机切换速度等级(0-15)

    uint8_t         position_type;              //位置控制方式(0-31)

    uint8_t         position_interval;          //位置控制时间间隔(0-100)
    uint8_t         bright_interval_len:4;      //连续亮的个数(0-14)
    uint8_t         dark_interval_len:4;        //连续暗的个数(0-14)

    uint8_t         breath_interval;            //呼吸间隔等级(0-31)

    uint8_t         random_interval_level:4;    //随机颜色间隔等级(0-15)
    uint8_t         d_h:4;                      //呼吸公差(0-15)
    uint8_t         d_s:3;                      //呼吸公差等级(0-6)
    uint8_t         d_v:5;                      //呼吸公差等级(0-31)

    uint8_t         max_h_level:6;              //H峰值等级(0-50)
    uint8_t         min_s_level:2;              //S峰谷等级(0-2)，饱和度呼吸最高值固定100，最低值由用户设置，因为100稍微偏低一点，就已经效果不明显了
    uint8_t         max_v;                      //峰值(0-100)

} action_conf_t;

void flush_fade_init(flush_param_t *param, uint8_t action_index, bit is_single);
void flush_fade_music_init(flush_param_t *param, uint8_t action_index, bit is_single);

void ctrl_set(flush_param_t *param, uint8_t random_interval_level, uint8_t d_h, uint8_t d_s, uint8_t d_v, 
    uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type, 
    uint8_t adorn_index, bit is_adorn_random, uint8_t adorn_random_level);

#define HSV(conf, _h, _s, _v)                          conf.hsv.h = _h; conf.hsv.s = _s; conf.hsv.v = _v
#define SECTION(conf, _start , _end)                   conf.start = _start; conf.end = _end
#define COLOR_TYPE(conf, _color_type)                  conf.color_type  = _color_type
#define ADORN_RANDOM(conf, _adorn_random)              conf.adorn_random  = _adorn_random
#define ADORN_INDEX(conf, _adorn_index)                conf.adorn_index  = _adorn_index
#define ADORN_RANDOM_LEVEL(conf, _adorn_random_level)  conf.adorn_random_level  = _adorn_random_level
#define POSTION_TYPE(conf, _position_type)             conf.position_type = _position_type
#define POSITON_INTERVAL(conf, _position_interval)     conf.position_interval = _position_interval
#define BRIGHT_INTERVAL_LEN(conf, _bright_interval_len) conf.bright_interval_len = _bright_interval_len
#define DARK_INTERVAL_LEN(conf, _dark_interval_len)     conf.dark_interval_len = _dark_interval_len
#define BREATH_INTERVAL(conf, _breath_interval)        conf.breath_interval = _breath_interval
#define RANDOM_INTERVAL_LEVEL(conf, _random_interval_level)    conf.random_interval_level = _random_interval_level
#define D_H(conf, _d_h)                                conf.d_h = _d_h
#define D_S(conf, _d_s)                                conf.d_s = _d_s
#define D_V(conf, _d_v)                                conf.d_v = _d_v
#define MAX_H_LEVEL(conf, _max_h_level)                conf.max_h_level = _max_h_level
#define MIN_S_LEVEL(conf, _min_s_level)                conf.min_s_level = _min_s_level
#define MIN_V(conf, _max_v)                            conf.max_v = _max_v

#if 0

/*颜色*/                        HSV(conf, h, s, v);
/*区间*/                        SECTION(conf, start ,end);
/*颜色控制方式*/                COLOR_TYPE(conf, color_type);
/*装饰色是否随机(0-1)*/         ADORN_RANDOM(conf, adorn_random);
/*装饰色索引(0-15)*/            ADORN_INDEX(conf, adorn_index);
/*装饰色随机切换速度等级(0-15)*/ ADORN_RANDOM_LEVEL(conf, adorn_random_level);
/*位置控制方式(0-31)*/          POSTION_TYPE(conf, position_type);
/*位置控制间隔(0-7)*/           POSITON_INTERVAL(conf, position_interval);
/*连续亮的个数(0-14)*/          BRIGHT_INTERVAL_LEN(conf, bright_interval_len);
/*连续暗的个数(0-14)*/          DARK_INTERVAL_LEN(conf, dark_interval_len);
/*呼吸间隔(0-31)*/              BREATH_INTERVAL(conf, breath_interval);
/*随机颜色切换间隔等级(0-15)*/  RANDOM_INTERVAL_LEVEL(conf, random_interval_level);
/*H 呼吸等差(0-15)*/            D_H(conf, d_h);
/*S 呼吸等差(0-6)*/             D_S(conf, d_s);
/*V 呼吸等差(0-31)*/            D_V(conf, d_v);
/*H 呼吸峰值等级(0-50)*/        MAX_H_LEVEL(conf, max_h_level);
/*S 呼吸峰谷等级(0-2)*/         MIN_S_LEVEL(conf, min_s_level);
/*V 呼吸峰值(0-100)*/           MIN_V(conf, max_v);

#endif

#endif
