#include "ir.h"
#include "cache.h"
#include "init.h"

#include "line.h"
#include "fountain.h"
#include "interval.h"
#include "random.h"
#include "tetris.h"

#include "line_music.h"
#include "fountain_music.h"
#include "stretch_music.h"
#include "interval_music.h"
#include "random_music.h"

#include "drv_uart.h"

typedef struct __work_mode__
{
    uint8_t     is_music_mode:1;    //是否为音乐模式
    uint8_t     is_power_on:1;      //是否开机
    uint8_t     flush_index:6;      //动画索引
    uint8_t     bright_adjust:4;    //亮度调节
    uint8_t     fade_type:4;        //渐变模式
    uint8_t     color_index:4;      //颜色索引
    uint8_t     breath_interval:4;  //呼吸间隔
    uint8_t     fade_interval:4;    //渐变间隔
    uint8_t     smooth_interval:4;  //平滑间隔
    uint8_t     is_static_color:1;    //是否静态颜色
    uint8_t     position_index:4;   //位置控制方式
} work_mode_t;

typedef void (*flush_action_cb)(flush_param_t *param);

typedef struct __flush__
{
    flush_action_cb         cb;
    uint8_t                 action_mode;
    uint8_t                 color_index;
    uint8_t                 position_interval;
    uint8_t                 bright_interval_len:4;
    uint8_t                 dark_interval_len:4;
    uint8_t                 position_len;
    uint8_t                 position_type[5];
} flush_t;

#define FLUSH_MODE_SIZE         43
static code flush_t _flush_action[FLUSH_MODE_SIZE] = {

    /*************************************** 非音乐模式 **************************************/
    /**
     * 线条模式
    */
    {line_set, E_LINE_MODE_SINGLE, 5, 0, 0, 0, 1, {E_LINE_POSITION_KEEP}},
    {line_set, E_LINE_MODE_RANDOM, 0, 0, 0, 0, 1, {E_LINE_POSITION_KEEP}},
    /**
     * 俄罗斯方块模式
    */
    {tetris_set, E_TETRIS_MODE_SINGLE, 6, 4, 0, 0, 4, {E_TETRIS_POSITION_LEFT, E_TETRIS_POSITION_RIGHT, E_TETRIS_POSITION_OPEN, E_TETRIS_POSITION_CLOSE}},
    {tetris_set, E_TETRIS_MODE_RANDOM, 0, 4, 0, 0, 4, {E_TETRIS_POSITION_LEFT, E_TETRIS_POSITION_RIGHT, E_TETRIS_POSITION_OPEN, E_TETRIS_POSITION_CLOSE}},

    {tetris_set, E_TETRIS_MODE_SINGLE, 5, 4, 0, 0, 4, {E_TETRIS_POSITION_LEFT1, E_TETRIS_POSITION_RIGHT1, E_TETRIS_POSITION_OPEN1, E_TETRIS_POSITION_CLOSE1}},
    {tetris_set, E_TETRIS_MODE_RANDOM, 0, 4, 0, 0, 4, {E_TETRIS_POSITION_LEFT1, E_TETRIS_POSITION_RIGHT1, E_TETRIS_POSITION_OPEN1, E_TETRIS_POSITION_CLOSE1}},
    /**
     * 喷泉模式
    */
    {fountain_set, E_FOUNTAIN_MODE_SINGLE, 5, 8, 2, 2, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_SINGLE, 5, 5, 8, 3, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 8, 2, 2, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 5, 8, 3, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 8, 0, 0, 4, {E_FOUNTAIN_POSITION_LEFT1, E_FOUNTAIN_POSITION_RIGHT1, E_FOUNTAIN_POSITION_OPEN1, E_FOUNTAIN_POSITION_CLOSE1}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 3, 0, 0, 4, {E_FOUNTAIN_POSITION_LEFT1, E_FOUNTAIN_POSITION_RIGHT1, E_FOUNTAIN_POSITION_OPEN1, E_FOUNTAIN_POSITION_CLOSE1}},
    /**
     * 追逐模式
    */
    {fountain_set, E_FOUNTAIN_MODE_SINGLE, 2, 4, 1, 5, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_SINGLE, 2, 1, 14, 14, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_SINGLE, 2, 2, 14, 14, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 3, 1, 5, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 1, 14, 14, 4, {E_FOUNTAIN_POSITION_LEFT1, E_FOUNTAIN_POSITION_RIGHT1, E_FOUNTAIN_POSITION_OPEN1, E_FOUNTAIN_POSITION_CLOSE1}},
    {fountain_set, E_FOUNTAIN_MODE_RANDOM, 0, 2, 14, 14, 4, {E_FOUNTAIN_POSITION_LEFT, E_FOUNTAIN_POSITION_RIGHT, E_FOUNTAIN_POSITION_OPEN, E_FOUNTAIN_POSITION_CLOSE}},
    /**
     * 间隔模式
    */
    {interval_set, E_INTERVAL_MODE_SINGLE, 3, 10, 2, 2, 2, {E_INTERVAL_POSITION_INTERVAL_B, E_INTERVAL_POSITION_INTERVAL_D}},
    {interval_set, E_INTERVAL_MODE_SINGLE, 3, 10, 8, 3, 2, {E_INTERVAL_POSITION_INTERVAL_B, E_INTERVAL_POSITION_INTERVAL_D}},
    {interval_set, E_INTERVAL_MODE_RANDOM, 0, 10, 2, 2, 2, {E_INTERVAL_POSITION_INTERVAL_B, E_INTERVAL_POSITION_INTERVAL_D}},
    {interval_set, E_INTERVAL_MODE_RANDOM, 0, 10, 8, 3, 2, {E_INTERVAL_POSITION_INTERVAL_B, E_INTERVAL_POSITION_INTERVAL_D}},
    /**
     * 随机星星模式
    */
    {random_set, E_RANDOM_MODE_SINGLE, 8, 33, 0, 0, 2, {E_RANDOM_POSITION_RANDOM, E_RANDOM_POSITION_RANDOM1}},
    {random_set, E_RANDOM_MODE_RANDOM, 0, 33, 0, 0, 2, {E_RANDOM_POSITION_RANDOM, E_RANDOM_POSITION_RANDOM1}},

    /*************************************** 音乐模式 **************************************/
    /**
     * 线条模式
    */
    {line_music_set, E_LINE_MUSIC_MODE_SINGLE, 11, 0, 0, 0, 1, {E_LINE_MUSIC_POSITION_KEEP}},
    {line_music_set, E_LINE_MUSIC_MODE_RANDOM, 0, 0, 0, 0, 1, {E_LINE_MUSIC_POSITION_KEEP}},
    /**
     * 间隔模式
    */
    {interval_music_set, E_INTERVAL_MODE_SINGLE, 3, 1, 2, 2, 2, {E_INTERVAL_MUSIC_POSITION_INTERVAL_B, E_INTERVAL_MUSIC_POSITION_INTERVAL_D}},
    {interval_music_set, E_INTERVAL_MODE_SINGLE, 3, 1, 8, 3, 2, {E_INTERVAL_MUSIC_POSITION_INTERVAL_B, E_INTERVAL_MUSIC_POSITION_INTERVAL_D}},
    {interval_music_set, E_INTERVAL_MODE_RANDOM, 0, 1, 2, 2, 2, {E_INTERVAL_MUSIC_POSITION_INTERVAL_B, E_INTERVAL_MUSIC_POSITION_INTERVAL_D}},
    {interval_music_set, E_INTERVAL_MODE_RANDOM, 0, 1, 8, 3, 2, {E_INTERVAL_MUSIC_POSITION_INTERVAL_B, E_INTERVAL_MUSIC_POSITION_INTERVAL_D}},
    /**
     * 柱形模式
    */
    {stretch_music_set, E_STRETCH_MUSIC_MODE_SINGLE, 5, 1, 0, 0, 4, {E_STRETCH_MUSIC_POSITION_LEFT, E_STRETCH_MUSIC_POSITION_RIGHT, E_STRETCH_MUSIC_POSITION_OPEN, E_STRETCH_MUSIC_POSITION_CLOSE}},
    {stretch_music_set, E_STRETCH_MUSIC_MODE_RANDOM, 0, 1, 0, 0, 4, {E_STRETCH_MUSIC_POSITION_LEFT, E_STRETCH_MUSIC_POSITION_RIGHT, E_STRETCH_MUSIC_POSITION_OPEN, E_STRETCH_MUSIC_POSITION_CLOSE}},
    {stretch_music_set, E_STRETCH_MUSIC_MODE_SINGLE, 0, 1, 0, 0, 1, {E_STRETCH_MUSIC_POSITION_OPEN2}},
    /**
     * 带浮点柱形模式
    */
    {stretch_music_set, E_STRETCH_MUSIC_MODE_SINGLE, 5, 1, 0, 0, 4, {E_STRETCH_MUSIC_POSITION_LEFT1, E_STRETCH_MUSIC_POSITION_RIGHT1, E_STRETCH_MUSIC_POSITION_OPEN1, E_STRETCH_MUSIC_POSITION_CLOSE1}},
    {stretch_music_set, E_STRETCH_MUSIC_MODE_RANDOM, 0, 1, 0, 0, 4, {E_STRETCH_MUSIC_POSITION_LEFT1, E_STRETCH_MUSIC_POSITION_RIGHT1, E_STRETCH_MUSIC_POSITION_OPEN1, E_STRETCH_MUSIC_POSITION_CLOSE1}},
    
    /**
     * 浮点模式
    */
    {stretch_music_set, E_STRETCH_MUSIC_MODE_SINGLE, 5, 1, 0, 0, 1, {E_STRETCH_MUSIC_POSITION_LEFT2}},
    {stretch_music_set, E_STRETCH_MUSIC_MODE_RANDOM, 0, 1, 0, 0, 1, {E_STRETCH_MUSIC_POSITION_LEFT2}},
    /**
     * 喷泉模式
    */
    {fountain_music_set, E_FOUNTAIN_MUSIC_MODE_SINGLE, 2, 4, 0, 0, 4, {E_FOUNTAIN_MUSIC_POSITION_LEFT, E_FOUNTAIN_MUSIC_POSITION_RIGHT, E_FOUNTAIN_MUSIC_POSITION_OPEN, E_FOUNTAIN_MUSIC_POSITION_CLOSE}},
    {fountain_music_set, E_FOUNTAIN_MUSIC_MODE_RANDOM, 0, 4, 0, 0, 4, {E_FOUNTAIN_MUSIC_POSITION_LEFT, E_FOUNTAIN_MUSIC_POSITION_RIGHT, E_FOUNTAIN_MUSIC_POSITION_OPEN, E_FOUNTAIN_MUSIC_POSITION_CLOSE}},
    /**
     * 随机星星模式
    */
    {random_music_set, E_RANDOM_MUSIC_MODE_SINGLE, 8, 10, 0, 0, 2, {E_RANDOM_MUSIC_POSITION_RANDOM, E_RANDOM_MUSIC_POSITION_RANDOM1}},
    {random_music_set, E_RANDOM_MUSIC_MODE_RANDOM, 0, 10, 0, 0, 2, {E_RANDOM_MUSIC_POSITION_RANDOM, E_RANDOM_MUSIC_POSITION_RANDOM1}},
    {random_music_set, E_RANDOM_MUSIC_MODE_SINGLE, 2, 1, 0, 0, 1, {E_RANDOM_MUSIC_POSITION_RANDOM1}},
    {random_music_set, E_RANDOM_MUSIC_MODE_RANDOM, 0, 1, 0, 0, 1, {E_RANDOM_MUSIC_POSITION_RANDOM1}},
};

static code uint8_t _color_static[24] = {
     0,  0,  0,  0, 
     0,  1,  2, 15, 
     3,  4,  5,  0, 
     6,  7,  8,  0, 
     9, 10, 11,  0, 
    12, 13, 14,  0, 
};

static xdata work_mode_t _work_mode = {0};

static void _work_mode_init(void)
{
    _work_mode.is_music_mode = false;
    _work_mode.is_power_on = false;
    _work_mode.flush_index = 0;
    _work_mode.fade_type = 0;
    _work_mode.color_index = 15;
    _work_mode.breath_interval = 0;
    _work_mode.fade_interval = 0;
    _work_mode.smooth_interval = 0;
    _work_mode.bright_adjust = 10;
    COLOR_CTRL_SET_BRIGHT_ADJUST(true, 100);
}

static void _flush(bit reset)
{
    data flush_param_t param = {0};
    data uint8_t fade_interval;

    if (reset)
    {
        memset(actions, 0, sizeof(action_t) * ACTION_COUNT);
    }

    if (_work_mode.fade_type == E_FADE_TYPE_KEEP)
    {
        fade_interval = 0;
        _work_mode.breath_interval = 0;
        _work_mode.fade_interval = 0;
        _work_mode.smooth_interval = 0;
    }
    else if (_work_mode.fade_type == E_FADE_TYPE_BREATH)
    {
        fade_interval = _work_mode.breath_interval;
        _work_mode.fade_interval = 0;
        _work_mode.smooth_interval = 0;
    }
    else if (_work_mode.fade_type == E_FADE_TYPE_FADE)
    {
        fade_interval = _work_mode.fade_interval;
        _work_mode.breath_interval = 0;
        _work_mode.smooth_interval = 0;
    }
    else if (_work_mode.fade_type == E_FADE_TYPE_SMOOTH)
    {
        fade_interval = _work_mode.smooth_interval;
        _work_mode.breath_interval = 0;
        _work_mode.fade_interval = 0;
    }

    param.mode = _flush_action[_work_mode.flush_index].action_mode;
    param.position = _flush_action[_work_mode.flush_index].position_type[_work_mode.position_index];
    param.fade_type = _work_mode.fade_type;
    param.fade_interval = fade_interval;
    param.is_static_color = _work_mode.is_static_color;
    param.color_index = _work_mode.color_index;
    param.is_raise_adjust = true;
    param.adjust = _work_mode.bright_adjust * 10;

    param.position_interval = _flush_action[_work_mode.flush_index].position_interval;
    param.bright_interval_len = _flush_action[_work_mode.flush_index].bright_interval_len;
    param.dark_interval_len = _flush_action[_work_mode.flush_index].dark_interval_len;

    _flush_action[_work_mode.flush_index].cb(&param);
}

void ir(void)
{
    data uint8_t ir_code;

    hal_ir_data(ir_code);

#if 0
    if (ir_code >= 0)
        drv_uart_send_byte(ir_code);
#endif

    switch (ir_code)
    {
        /**
         * 亮度调节
        */
        case E_IR_CODE_BRIGHT:
        {
            _work_mode.bright_adjust += 1;
            if (_work_mode.bright_adjust > 10)
                _work_mode.bright_adjust = 10;
            _flush(false);
            break;
        }
        case E_IR_CODE_DARKEN:
        {
            if (_work_mode.bright_adjust > 1)
                _work_mode.bright_adjust -= 1;
            else
                _work_mode.bright_adjust = 1;
            _flush(false);
            break;
        }
        /**
         * 电源控制
        */
        case E_IR_CODE_POWER_OFF:
        {
            player_clear();
            _work_mode_init();
            //hal_sys_pd();   //掉电模式
            hal_delay(150);
            break;
        }
        case E_IR_CODE_POWER_ON:
        {
            if (_work_mode.is_power_on == false)
            {
                init();
                _work_mode_init();
                _work_mode.is_power_on = true;
                _flush(true);
            }
            else
            {
                if (_work_mode.is_music_mode) _work_mode.flush_index = 0;
                else _work_mode.flush_index = 24;
                _work_mode.is_music_mode = ~_work_mode.is_music_mode;
                _flush(true);
            }
            break;
        }
        /**
         * 静态颜色
        */
        case E_IR_CODE_RED:
        case E_IR_CODE_RED1:
        case E_IR_CODE_RED2:
        case E_IR_CODE_RED3:
        case E_IR_CODE_RED4:
        case E_IR_CODE_GREEN:
        case E_IR_CODE_GREEN1:
        case E_IR_CODE_GREEN2:
        case E_IR_CODE_GREEN3:
        case E_IR_CODE_GREEN4:
        case E_IR_CODE_BLUE:
        case E_IR_CODE_BLUE1:
        case E_IR_CODE_BLUE2:
        case E_IR_CODE_BLUE3:
        case E_IR_CODE_BLUE4:
        case E_IR_CODE_WHITE:
        {
            _work_mode.is_static_color = true;
            _work_mode.color_index = _color_static[ir_code];
            _flush(false);
            break;
        }
        /**
         * 刷新特效
        */
        case E_IR_CODE_FLUSH:
        {
            if (_work_mode.is_power_on == true)
            {
                _work_mode.fade_type = E_FADE_TYPE_KEEP;
                if (_work_mode.is_static_color == false) _work_mode.position_index ++;
                if (_work_mode.position_index >= _flush_action[_work_mode.flush_index].position_len)
                {
                    _work_mode.position_index = 0;
                    _work_mode.flush_index ++;
                    if (_work_mode.flush_index >= FLUSH_MODE_SIZE)
                    {
                        _work_mode.flush_index = 0;
                    }
                }

                _work_mode.fade_type = 0;
                _work_mode.color_index = _flush_action[_work_mode.flush_index].color_index;
                _work_mode.is_static_color = false;
                _flush(true);
            }
            break;
        }
        /**
         * 呼吸速度
        */
        case E_IR_CODE_STROBE:
        {
            _work_mode.fade_type = E_FADE_TYPE_BREATH;
            _work_mode.breath_interval ++;
            if (_work_mode.breath_interval > 3)
                _work_mode.breath_interval = 0;
            _flush(false);
            break;
        }
        /**
         * 渐变速度
        */
        case E_IR_CODE_FADE:
        {
            _work_mode.fade_type = E_FADE_TYPE_FADE;
            _work_mode.fade_interval ++;
            if (_work_mode.fade_interval > 3)
                _work_mode.fade_interval = 0;
            _flush(false);
            break;
        }
        /**
         * 平滑速度
        */
        case E_IR_CODE_SMOOTH:
        {
            _work_mode.fade_type = E_FADE_TYPE_SMOOTH;
            _work_mode.smooth_interval ++;
            if (_work_mode.smooth_interval > 3)
                _work_mode.smooth_interval = 0;
            _flush(false);
            break;
        }
        default:
            break;
    }
}
