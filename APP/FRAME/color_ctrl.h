#ifndef __COLOR_CTRL_H__
#define __COLOR_CTRL_H__

#include "breath.h"
#include "hsv.h"
#include "colors.h"

typedef enum __ctrl_level__
{
    E_CTRL_LEVEL_0 = 0x00,
    E_CTRL_LEVEL_1 = 0x01,
    E_CTRL_LEVEL_2 = 0x02,
    E_CTRL_LEVEL_3 = 0x03,
    E_CTRL_LEVEL_4 = 0x04,
    E_CTRL_LEVEL_5 = 0x05,
    E_CTRL_LEVEL_6 = 0x06,
    E_CTRL_LEVEL_7 = 0x07,
    E_CTRL_LEVEL_8 = 0x08,
    E_CTRL_LEVEL_9 = 0x09,
    E_CTRL_LEVEL_A = 0x0A,
    E_CTRL_LEVEL_B = 0x0B,
    E_CTRL_LEVEL_C = 0x0C,
    E_CTRL_LEVEL_D = 0x0D,
    E_CTRL_LEVEL_E = 0x0E,
    E_CTRL_LEVEL_F = 0x0F
} ctrl_level_t;

typedef enum __color_ctrl_type__
{
    /********************* 非音乐模式 ****************************/

    /**
     * 单色控制，单色模式简谐和呼吸
    */
    E_COLOR_CTRL_TYPE_SINGLE_KEEP = 0,      //直接显示
    E_COLOR_CTRL_TYPE_SINGLE_BREATH_V,      //亮度呼吸
    E_COLOR_CTRL_TYPE_SINGLE_HARMONIC_V,    //亮度简谐
    E_COLOR_CTRL_TYPE_SINGLE_BREATH_S,      //对比度呼吸
    E_COLOR_CTRL_TYPE_SINGLE_HARMONIC_S,    //对比度简谐

    /**
     * 随机颜色控制，随机颜色模式简谐和呼吸只能针对亮度
    */
    E_COLOR_CTRL_TYPE_RANDOM_KEEP,              //直接显示
    E_COLOR_CTRL_TYPE_RANDOM_BREATH_V,          //亮度呼吸
    E_COLOR_CTRL_TYPE_RANDOM_HARMONIC_V,        //亮度简谐
    E_COLOR_CTRL_TYPE_RANDOM_BREATH_S,          //对比度呼吸
    E_COLOR_CTRL_TYPE_RANDOM_HARMONIC_S,        //对比度简谐

    /**
     * 颜色渐变，颜色渐变简谐和呼吸可以为hsv中任何一个维度
    */
    E_COLOR_CTRL_TYPE_SMOOTH_HSV,           //颜色HSV渐变，调节到合适的hsv呼吸参数更柔和
    E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H,      //颜色渐变
    E_COLOR_CTRL_TYPE_SMOOTH_BREATH_HV,     //颜色渐变伴随亮度呼吸
    E_COLOR_CTRL_TYPE_SMOOTH_HARMONIC_V,    //颜色渐变伴随亮度简谐

    /********************* 音乐模式 ****************************/
    /**
     * 单色控制，单色模式简谐和呼吸
    */
    E_COLOR_CTRL_TYPE_MUSIC_SINGLE_KEEP,            //直接显示
    E_COLOR_CTRL_TYPE_MUSIC_SINGLE_BREATH_V,        //跟随音乐亮度呼吸，音乐节奏快，亮度大，音乐节奏慢或者无，则不亮
    E_COLOR_CTRL_TYPE_MUSIC_SINGLE_HARMONIC_V,      //跟随音乐亮度简谐
    E_COLOR_CTRL_TYPE_MUSIC_SINGLE_BREATH_S,        //跟随音乐对比度呼吸

    /**
     * 随机颜色控制，随机颜色模式简谐和呼吸只能针对亮度
    */
    E_COLOR_CTRL_TYPE_MUSIC_RANDOM_KEEP,              //直接显示
    E_COLOR_CTRL_TYPE_MUSIC_RANDOM_BREATH_V,          //跟随音乐亮度呼吸，音乐节奏快，亮度大，音乐节奏慢或者无，则不亮
    E_COLOR_CTRL_TYPE_MUSIC_RANDOM_HARMONIC_V,        //跟随音乐亮度简谐
    E_COLOR_CTRL_TYPE_MUSIC_RANDOM_BREATH_S,          //对比度呼吸

    /**
     * 颜色渐变，颜色渐变简谐和呼吸可以为hsv中任何一个维度
    */
    E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_BREATH_H,      //颜色渐变
    E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_BREATH_HV,     //颜色渐变伴随亮度呼吸
    E_COLOR_CTRL_TYPE_MUSIC_SMOOTH_HARMONIC_V,    //颜色渐变伴随亮度简谐

} color_ctrl_type_t;

typedef struct __color_ctrl__
{
    uint8_t                 ctrl_type:7;                    //颜色控制方式(0-127)
    uint8_t                 adorn_random:1;                 //装饰色是否随机(0-1)
    uint8_t                 adorn_index:4;                  //装饰色索引(0-15)
    uint8_t                 adorn_level:4;                  //装饰色随机切换速度等级，每个等级10，总共15级，等级越高速度越慢
    uint8_t                 random_index:4;                 //随机模式颜色选择索引
    uint8_t                 random_interval_level:4;        //随机切换速度等级，每个等级10，总共15级，等级越高速度越慢
    breath_t                breath;
} color_ctrl_t;

typedef struct __bright_adjust__
{
    uint8_t                 is_raise:1;         //true增加，false减少
    uint8_t                 adjust:7;           //值
} bright_adjust_t;

bool color_ctrl(hsv_t *hsv, color_ctrl_t *ctrl, uint8_t fre_index, uint8_t fre_div);
void color_ctrl_get_adorn_hsv(hsv_t *hsv, color_ctrl_t *ctrl);

#define COLOR_CTRL_INIT(color_ctrl)    memset(color_ctrl, 0, sizeof(color_ctrl_t))

/**
 * ctrl
*/
#define COLOR_CTRL_SET_CTRL_TYPE(color_ctrl, type)                  (color_ctrl)->ctrl_type = type
#define COLOR_CTRL_SET_RANDOM_INTERVAL_LEVEL(color_ctrl, level)     (color_ctrl)->random_interval_level = level

#define COLOR_CTRL_SET_ADORN_RANDOM(color_ctrl, is_random)              (color_ctrl)->adorn_random = is_random
#define COLOR_CTRL_SET_ADORN_INDEX(color_ctrl, _adorn_index)            (color_ctrl)->adorn_index = _adorn_index
#define COLOR_CTRL_SET_ADORN_INVERVAL_LEVEL(color_ctrl, _adorn_level)   (color_ctrl)->adorn_level = _adorn_level

/**
 * breath
*/
#define COLOR_CTRL_SET_BREATH_INTERVAL(color_ctrl, interval)       BREATH_SET_INTERVAL(&((color_ctrl)->breath), interval)

#define COLOR_CTRL_SET_BREATH_SET_D_H(color_ctrl, d)                BREATH_SET_D_H(&((color_ctrl)->breath), d)
#define COLOR_CTRL_SET_BREATH_SET_D_S(color_ctrl, d)                BREATH_SET_S_H(&((color_ctrl)->breath), d)
#define COLOR_CTRL_SET_BREATH_SET_D_V(color_ctrl, d)                BREATH_SET_V_H(&((color_ctrl)->breath), d)

#define COLOR_CTRL_SET_BREATH_SET_MAX_H_LEVEL(color_ctrl, level)    BREATH_SET_MAX_H_LEVEL(&((color_ctrl)->breath), level)
#define COLOR_CTRL_SET_BREATH_SET_MIN_S_LEVEL(color_ctrl, level)    BREATH_SET_MIN_S_LEVEL(&((color_ctrl)->breath), level)
#define COLOR_CTRL_SET_BREATH_SET_MAX_V(color_ctrl, v)              BREATH_SET_MAX_V(&((color_ctrl)->breath), v)

/**
 * adjust
*/
#define COLOR_CTRL_SET_BRIGHT_ADJUST(_is_raise, _adjust)            bright_adjust.is_raise = _is_raise; bright_adjust.adjust = _adjust

extern data bright_adjust_t bright_adjust;

#endif
