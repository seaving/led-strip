#ifndef __POSITION_CTRL_H__
#define __POSITION_CTRL_H__

#include "typedef.h"
#include "hsv.h"
#include "color_ctrl.h"

typedef enum __position_type__
{
    /**
     * 取值必须保持连续，必须从0开始，最大15
    */
    E_POSITION_TYPE_KEEP = 0,           //保持不动
    E_POSITION_TYPE_MOVE_LEFT,          //位移向左
    E_POSITION_TYPE_MOVE_RIGHT,         //位移向右

    E_POSITION_TYPE_STRETCH_LEFT,       //伸缩向左
    E_POSITION_TYPE_STRETCH_RIGHT,      //伸缩向右

    E_POSITION_TYPE_INTERVAL_B,         //间隔显示，组合，连续显示多少个led后，再连续不显示多少个led，ooo--ooo--ooo--
    E_POSITION_TYPE_INTERVAL_D,         //间隔显示，组合，连续不显示多少个led后，再连续显示多少个led，--ooo--ooo--ooo

    E_POSITION_TYPE_RANDOM,             //随机位置

    E_POSITION_TYPE_TETRIS_LEFT,          //俄罗斯方块向左
    E_POSITION_TYPE_TETRIS_RIGHT,         //俄罗斯方块向右

    /***************** 音乐 ******************/
    E_POSITION_TYPE_MUSIC_KEEP,               //保持不动
    E_POSITION_TYPE_MUSIC_MOVE_LEFT,          //位移向左
    E_POSITION_TYPE_MUSIC_MOVE_RIGHT,         //位移向右

    E_POSITION_TYPE_MUSIC_STRETCH_LEFT,       //伸缩向左
    E_POSITION_TYPE_MUSIC_STRETCH_RIGHT,      //伸缩向右

    E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_LEFT,   //带悬浮的伸缩向左
    E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT,  //带悬浮的伸缩向右

    E_POSITION_TYPE_MUSIC_FLOAT_LEFT,           //悬浮向左
    E_POSITION_TYPE_MUSIC_FLOAT_RIGHT,          //悬浮向右

    E_POSITION_TYPE_MUSIC_INTERVAL_B,         //间隔显示，组合，连续显示多少个led后，再连续不显示多少个led，ooo--ooo--ooo--
    E_POSITION_TYPE_MUSIC_INTERVAL_D,         //间隔显示，组合，连续不显示多少个led后，再连续显示多少个led，--ooo--ooo--ooo

    E_POSITION_TYPE_MUSIC_RANDOM,             //随机位置
} position_type_t;

typedef struct __stretch__
{
    uint8_t             dir;                //伸缩偏移方向，type为伸缩模式有效
    uint8_t             offset;             //伸缩偏移，type为伸缩模式有效
} stretch_t;

typedef struct __interval__
{
    uint8_t        bright_interval_len:4;  //间隔长度，用于间隔显示(高四位用于连续亮的个数，低四位用于连续灭的个数)(0-14)
    uint8_t        dark_interval_len:4;    //间隔长度，用于间隔显示(高四位用于连续亮的个数，低四位用于连续灭的个数)(0-14)
    uint8_t        bright_interval_offset:4; //间隔长度计数器
    uint8_t        dark_interval_offset:4; //间隔长度计数器
} interval_t;

typedef struct __tetris__
{
    uint8_t        tetris_len;           //俄罗斯方块模式下，记录方块叠加的数量
    uint8_t        tetris_offset;        //俄罗斯方块模式下，偏移地址
} tetris_t;

typedef struct __music_stretch__
{
    uint8_t        music_stretch_offset;  //音乐柱形拉伸偏移
    uint8_t        music_float_offset;    //音乐柱形悬浮物偏移
} music_stretch_t;

typedef struct __position_ctrl__
{
    uint8_t             start;              //灯条中实际的段落起始位置
    uint8_t             end;                //灯条中实际的段落结束位置
    uint8_t             position_type;      //位移方式(0-31)
    uint8_t             interval;           //位移时间间隔(0-100)
    uint8_t             cache_index;        //显存索引
    union position_ctrl
    {
        uint8_t         random_offset;          //记录上一次的随机位置，用于随机模式
        stretch_t       stretch;                //伸缩显示参数
        interval_t      interval;               //间隔显示参数
        tetris_t        tetris;                 //俄罗斯方块参数
        music_stretch_t music_stretch;          //音乐柱形参数
    } v;

} position_ctrl_t;

void position_ctrl(hsv_t *hsv, position_ctrl_t *position_ctrl, color_ctrl_t *color_ctrl, uint8_t fre_index);

#define POSITION_CTRL_INIT(ctrl)    memset(ctrl, 0, sizeof(position_ctrl_t))

#define POSITION_CTRL_SET_CACHE_INDEX(ctrl, _cache_index)       (ctrl)->cache_index = _cache_index

#define POSITION_CTRL_GET_SECTION(ctrl, _start, _end)   _start = (ctrl)->start; _end = (ctrl)->end
#define POSITION_CTRL_GET_SECTION_START(ctrl, _start)   _start = (ctrl)->start;
#define POSITION_CTRL_GET_SECTION_END(ctrl, _end)       _end = (ctrl)->end;
#define POSITION_CTRL_GET_SECTION(ctrl, _start, _end)   _start = (ctrl)->start; _end = (ctrl)->end
#define POSITION_CTRL_GET_TYPE(ctrl)                    (ctrl)->position_type

#define POSITION_CTRL_SET_SECTION(ctrl, _start, _end)    (ctrl)->start = _start; (ctrl)->end = _end
#define POSITION_CTRL_SET_TYPE(ctrl, _position_type)    (ctrl)->position_type = _position_type
#define POSITION_CTRL_SET_INTERVAL(ctrl, _interval)     (ctrl)->interval = _interval

#define POSITION_CTRL_SET_STRETCH(ctrl, _stretch)                       (ctrl)->v.stretch = _stretch
#define POSITION_CTRL_SET_INTERVAL_LEN(ctrl, bright_len, dark_len)      (ctrl)->v.interval.bright_interval_len = bright_len; (ctrl)->v.interval.dark_interval_len = dark_len

#define POSITION_CTRL_LEN(ctrl)                                         ((ctrl)->end - (ctrl)->start + 1)

#endif
