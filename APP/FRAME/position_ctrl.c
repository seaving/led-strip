#include "position_ctrl.h"
#include "timer.h"
#include "cache.h"
#include "fft.h"
#include "color_ctrl.h"

#define POSITION_RANDOM_SIZE        8
static code uint8_t _position_random[POSITION_RANDOM_SIZE][LED_MAX_NUM] = {
    {
        26, 14, 43, 9, 14, 16, 31, 40, 4, 41, 48, 37, 7, 46, 16, 10, 39, 11, 20, 29, 15, 20, 30, 
        21, 43, 18, 33, 13, 34, 4, 35, 41, 14, 17, 28, 18, 28, 8, 44, 6, 38, 40, 32, 21, 0, 10, 33, 45, 29, 29
    }, {
        36, 35, 38, 21, 34, 33, 21, 43, 18, 21, 38, 7, 6, 25, 46, 32, 7, 2, 17, 9, 47, 37, 46, 48, 
        1, 18, 21, 7, 0, 2, 24, 43, 18, 37, 34, 25, 45, 12, 36, 8, 9, 42, 9, 9, 26, 31, 38, 41, 7, 2
    }, {
        24, 43, 7, 14, 18, 25, 25, 1, 32, 1, 30, 11, 38, 15, 22, 13, 36, 39, 47, 24, 31, 24, 14, 8, 23, 
        17, 4, 29, 42, 28, 14, 27, 12, 20, 21, 24, 35, 45, 41, 24, 2, 34, 48, 5, 38, 47, 5, 8, 30, 15
    }, {
        27, 31, 33, 30, 18, 47, 14, 14, 15, 21, 45, 49, 43, 42, 44, 39, 43, 7, 36, 36, 27, 10, 21, 21, 28, 49, 
        30, 35, 2, 28, 46, 10, 17, 0, 22, 28, 12, 33, 5, 0, 41, 29, 11, 27, 5, 22, 16, 26, 41, 37
    }, {
        48, 15, 6, 31, 3, 28, 2, 18, 14, 16, 18, 3, 44, 34, 10, 22, 29, 42, 30, 19, 23, 28, 49, 12, 
        25, 28, 42, 35, 19, 26, 3, 32, 8, 2, 24, 14, 17, 30, 16, 25, 45, 37, 20, 45, 26, 26, 19, 28, 12, 6
    }, {
        9, 32, 2, 14, 4, 33, 5, 29, 24, 39, 32, 15, 16, 10, 28, 15, 6, 25, 19, 27, 29, 0, 27, 7, 27, 
        4, 8, 0, 42, 20, 27, 4, 35, 31, 14, 42, 19, 30, 47, 49, 36, 30, 22, 16, 46, 4, 1, 47, 36, 31
    }, {
        40, 4, 16, 18, 7, 47, 19, 13, 38, 2, 8, 25, 5, 9, 6, 12, 0, 48, 5, 43, 41, 34, 16, 22, 13, 
        7, 20, 13, 25, 31, 25, 33, 34, 40, 33, 46, 23, 46, 12, 44, 5, 40, 44, 48, 35, 22, 16, 10, 22, 17
    }, {
        36, 16, 1, 9, 39, 34, 36, 13, 0, 40, 19, 48, 21, 7, 47, 7, 9, 0, 41, 30, 11, 48, 48, 49, 46, 
        39, 39, 32, 5, 0, 38, 10, 47, 38, 7, 39, 31, 20, 47, 23, 12, 38, 7, 19, 44, 6, 8, 14, 38, 9
    },
};

static void __position_ctrl_move(position_ctrl_t *position_ctrl, uint8_t len, bool is_left, uint8_t r, uint8_t g, uint8_t b)
{
    data bit flag = false;
    data uint8_t offset, offset1, i;

    if (IS_TIME_TO(position_ctrl->interval) == false)
    {
        return;
    }

    len -= 1;

    if (position_ctrl->v.interval.dark_interval_offset == 0)
    {
        if (position_ctrl->v.interval.bright_interval_offset == 15)
            position_ctrl->v.interval.bright_interval_offset = 0;
        position_ctrl->v.interval.bright_interval_offset ++;
        if (position_ctrl->v.interval.bright_interval_offset >= position_ctrl->v.interval.bright_interval_len)
        {
            position_ctrl->v.interval.dark_interval_offset = 15;
            position_ctrl->v.interval.bright_interval_offset = 0;
            flag = true;
        }
    }
    if (flag == false && position_ctrl->v.interval.bright_interval_offset == 0) 
    {
        if (position_ctrl->v.interval.dark_interval_offset == 15)
            position_ctrl->v.interval.dark_interval_offset = 0;
        position_ctrl->v.interval.dark_interval_offset ++;
        if (position_ctrl->v.interval.dark_interval_offset >= position_ctrl->v.interval.dark_interval_len)
        {
            position_ctrl->v.interval.dark_interval_offset = 0;
            position_ctrl->v.interval.bright_interval_offset = 15;
        }

        if (position_ctrl->v.interval.dark_interval_len > 0)
        {
            r = g = b = 0;
        }
    }

    if (is_left == false)
    {
        offset = position_ctrl->start;
    }
    else
    {
        offset = position_ctrl->end;
    }

    cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);

    for (i = 0; i < len; i ++)
    {
        if (is_left == false)
        {
            offset = position_ctrl->end - i;
            offset1 = offset - 1;
        }
        else
        {
            offset = position_ctrl->start + i;
            offset1 = offset + 1;
        }

        r = cache_get_pixel_r(position_ctrl->cache_index, offset1);
        g = cache_get_pixel_g(position_ctrl->cache_index, offset1);
        b = cache_get_pixel_b(position_ctrl->cache_index, offset1);
        cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
    }
}

static void _position_ctrl_stretch(position_ctrl_t *position_ctrl, uint8_t len, uint8_t r, uint8_t g, uint8_t b)
{
    data uint8_t offset;

    if (IS_TIME_TO(position_ctrl->interval) == false)
    {
        return;
    }

    if (position_ctrl->position_type == E_POSITION_TYPE_STRETCH_LEFT)
    {
        if (position_ctrl->v.stretch.dir == true)
        {
            //缩回去
            r = g = b = 0;
            offset = position_ctrl->start + position_ctrl->v.stretch.offset;
        }
        else
        {
            //伸出去
            offset = position_ctrl->end - position_ctrl->v.stretch.offset;
        }
    }
    else
    {
        if (position_ctrl->v.stretch.dir == true)
        {
            //缩回去
            r = g = b = 0;
            offset = position_ctrl->end - position_ctrl->v.stretch.offset;
        }
        else
        {
            //伸出去
            offset = position_ctrl->start + position_ctrl->v.stretch.offset;
        }
    }

    position_ctrl->v.stretch.offset ++;
    if (position_ctrl->v.stretch.offset >= len)
    {
        position_ctrl->v.stretch.offset = 0;
        position_ctrl->v.stretch.dir = position_ctrl->v.stretch.dir ? false : true;
    }

    cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
}

static void _position_ctrl_interval(position_ctrl_t *position_ctrl, uint8_t len, uint8_t r, uint8_t g, uint8_t b)
{
    data uint8_t i;
    data bit flag = false;
    data bool dark_first= position_ctrl->position_type == E_POSITION_TYPE_INTERVAL_D;
    data uint8_t r1 = r, g1 = g, b1 = b;

    if (IS_TIME_TO(position_ctrl->interval) == false)
    {
        return;
    }

    cache_set(position_ctrl->cache_index, position_ctrl->start, len, 0, 0, 0);

    for (i = 0; i < len; i ++)
    {
        if (flag == dark_first)
        {
            position_ctrl->v.interval.bright_interval_offset ++;
            if (position_ctrl->v.interval.bright_interval_offset >= position_ctrl->v.interval.bright_interval_len)
            {
                position_ctrl->v.interval.dark_interval_offset = 0;
                position_ctrl->v.interval.bright_interval_offset = 0;
                flag = ~flag;
            }

            if (position_ctrl->v.interval.bright_interval_len > 0)
            {
                r = r1;
                g = g1;
                b = b1;
            }
        }
        else
        {
            position_ctrl->v.interval.dark_interval_offset ++;
            if (position_ctrl->v.interval.dark_interval_offset >= position_ctrl->v.interval.dark_interval_len)
            {
                position_ctrl->v.interval.dark_interval_offset = 0;
                position_ctrl->v.interval.bright_interval_offset = 0;
                flag = ~flag;
            }

            if (position_ctrl->v.interval.dark_interval_len > 0)
            {
                r = g = b = 0;
            }
        }

        cache_set_pixel(position_ctrl->cache_index, position_ctrl->start + i, r, g, b);
    }
}

static void _position_ctrl_random(position_ctrl_t *position_ctrl, uint8_t len, uint8_t r, uint8_t g, uint8_t b)
{
    data uint8_t offset = position_ctrl->v.random_offset;

    if (IS_TIME_TO(position_ctrl->interval))
    {
        cache_set(position_ctrl->cache_index, position_ctrl->start, len, 0, 0, 0);

        offset = _position_random[CUR_TIME_CNT() % POSITION_RANDOM_SIZE][CUR_TIME_CNT() % LED_MAX_NUM] % len;
    }

    cache_set_pixel(position_ctrl->cache_index, position_ctrl->start + position_ctrl->v.random_offset, r, g, b);
    cache_set_pixel(position_ctrl->cache_index, position_ctrl->start + offset, r, g, b);

    position_ctrl->v.random_offset = offset;
}

static void _position_ctrl_tetris(position_ctrl_t *position_ctrl, color_ctrl_t *color_ctrl, uint8_t len, uint8_t r, uint8_t g, uint8_t b)
{
    data hsv_t hsv;
    data uint8_t offset, offset1;

    if (IS_TIME_TO(position_ctrl->interval) == false)
    {
        return;
    }

    if (position_ctrl->position_type == E_POSITION_TYPE_TETRIS_LEFT)
    {
        offset = position_ctrl->end - position_ctrl->v.tetris.tetris_offset;
        offset1 = position_ctrl->end;
        if (offset != position_ctrl->end)
        {
            offset1 = offset + 1;
        }
    }
    else
    {
        offset = position_ctrl->start + position_ctrl->v.tetris.tetris_offset;
        offset1 = position_ctrl->start;
        if (offset != position_ctrl->start)
        {
            offset1 = offset - 1;
        }
    }

    cache_set_pixel(position_ctrl->cache_index, offset1, 0, 0, 0);
    cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);

    position_ctrl->v.tetris.tetris_offset ++;
    if (position_ctrl->v.tetris.tetris_offset >= len - position_ctrl->v.tetris.tetris_len)
    {
        position_ctrl->v.tetris.tetris_offset = 0;

        position_ctrl->v.tetris.tetris_len ++;
        if (position_ctrl->v.tetris.tetris_len >= len)
        {
            position_ctrl->v.tetris.tetris_len = 0;
        }

        color_ctrl_get_adorn_hsv(&hsv, color_ctrl);
        hsv_to_rgb(&hsv, &r, &g, &b);
        cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
    }
}

static void __position_ctrl(uint8_t r, uint8_t g, uint8_t b, uint8_t len, position_ctrl_t *position_ctrl, color_ctrl_t *color_ctrl)
{
    switch (position_ctrl->position_type)
    {
        case E_POSITION_TYPE_MOVE_LEFT:
        case E_POSITION_TYPE_MOVE_RIGHT:
            __position_ctrl_move(position_ctrl, len, position_ctrl->position_type != E_POSITION_TYPE_MOVE_RIGHT, r, g, b);
            break;
        case E_POSITION_TYPE_STRETCH_LEFT:
        case E_POSITION_TYPE_STRETCH_RIGHT:
            _position_ctrl_stretch(position_ctrl, len, r, g, b);
            break;
        case E_POSITION_TYPE_INTERVAL_B:
        case E_POSITION_TYPE_INTERVAL_D:
            _position_ctrl_interval(position_ctrl, len, r, g, b);
            break;
        case E_POSITION_TYPE_RANDOM:
            _position_ctrl_random(position_ctrl, len, r, g, b);
            break;
        case E_POSITION_TYPE_TETRIS_LEFT:
        case E_POSITION_TYPE_TETRIS_RIGHT:
            _position_ctrl_tetris(position_ctrl, color_ctrl, len, r, g, b);
            break;
        case E_POSITION_TYPE_KEEP:
        default:
            cache_set(position_ctrl->cache_index, position_ctrl->start, len, r, g, b);
            break;
    }
}

/************************************************ 音乐模式 **************************************************/

static void _position_ctrl_music_stretch(position_ctrl_t *position_ctrl, uint8_t len, color_ctrl_t *color_ctrl, bool is_float, uint8_t r, uint8_t g, uint8_t b, uint8_t pillars, bit only_float)
{
    data hsv_t hsv;
    data uint8_t offset, offset1, offset2, i;
    data bit is_right = position_ctrl->position_type == E_POSITION_TYPE_MUSIC_STRETCH_RIGHT 
        || position_ctrl->position_type == E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT 
        || position_ctrl->position_type == E_POSITION_TYPE_MUSIC_FLOAT_RIGHT;

    if (only_float)
    {
        cache_set(position_ctrl->cache_index, position_ctrl->start, len, 0, 0, 0);
    }

    //这个延时需要与浮点成公倍数
    if (IS_TIME_TO(position_ctrl->interval))
    {
        if (position_ctrl->v.music_stretch.music_stretch_offset < pillars)
        {
            position_ctrl->v.music_stretch.music_stretch_offset ++;
        }
        else if (position_ctrl->v.music_stretch.music_stretch_offset > pillars)
        {
            position_ctrl->v.music_stretch.music_stretch_offset --;
        }
    }

    if (is_right)
    {
        offset = position_ctrl->start;
        offset2 = position_ctrl->start;
        if (position_ctrl->v.music_stretch.music_stretch_offset > 0)
            offset2 = offset2 + position_ctrl->v.music_stretch.music_stretch_offset - 1;
        if (position_ctrl->v.music_stretch.music_stretch_offset != len)
        {
            cache_set(position_ctrl->cache_index, offset2 + 1, len - position_ctrl->v.music_stretch.music_stretch_offset, 0, 0, 0);
        }
    }
    else
    {
        offset = position_ctrl->end;
        offset2 = position_ctrl->end;
        if (position_ctrl->v.music_stretch.music_stretch_offset > 0)
            offset2 = offset2 - (position_ctrl->v.music_stretch.music_stretch_offset - 1);
        cache_set(position_ctrl->cache_index, position_ctrl->start, len - position_ctrl->v.music_stretch.music_stretch_offset, 0, 0, 0);
    }

    if (only_float == false)
    {
        if (position_ctrl->v.music_stretch.music_stretch_offset == 0) r = g = b = 0;
        cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
    }

    len = 0;
    if (position_ctrl->v.music_stretch.music_stretch_offset > 0)
        len = position_ctrl->v.music_stretch.music_stretch_offset - 1;

    for (i = 0; i < len && only_float == false; i ++)
    {
        if (is_right)
        {
            offset = offset2 - i;
            offset1 = offset - 1;
        }
        else
        {
            offset = offset2 + i;
            offset1 = offset + 1;
        }
        r = cache_get_pixel_r(position_ctrl->cache_index, offset1);
        g = cache_get_pixel_g(position_ctrl->cache_index, offset1);
        b = cache_get_pixel_b(position_ctrl->cache_index, offset1);
        cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
    }

    if (is_float)
    {
        if (only_float == false)
        {
            color_ctrl_get_adorn_hsv(&hsv, color_ctrl);
            hsv_to_rgb(&hsv, &r, &g, &b);
        }

        if (position_ctrl->v.music_stretch.music_stretch_offset >= position_ctrl->v.music_stretch.music_float_offset)
        {
            if (is_right)
            {
                offset = position_ctrl->start;
                if (position_ctrl->v.music_stretch.music_stretch_offset > 0)
                    offset = offset + position_ctrl->v.music_stretch.music_stretch_offset - 1;
            }
            else
            {
                offset = position_ctrl->end;
                if (position_ctrl->v.music_stretch.music_stretch_offset > 0)
                    offset = offset - (position_ctrl->v.music_stretch.music_stretch_offset - 1);
            }
            position_ctrl->v.music_stretch.music_float_offset = position_ctrl->v.music_stretch.music_stretch_offset;
            cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
        }

        if (position_ctrl->v.music_stretch.music_float_offset == 0)
            position_ctrl->v.music_stretch.music_float_offset = 1;

        if (is_right)
        {
            offset = position_ctrl->start + position_ctrl->v.music_stretch.music_float_offset - 1;
            offset1 = offset + 1;
        }
        else
        {
            offset = position_ctrl->end - (position_ctrl->v.music_stretch.music_float_offset - 1);
            offset1 = offset - 1;
        }

        if (IS_TIME_TO(position_ctrl->interval * 10))
        {
            position_ctrl->v.music_stretch.music_float_offset --;
        }

        cache_set_pixel(position_ctrl->cache_index, offset1, 0, 0, 0);
        cache_set_pixel(position_ctrl->cache_index, offset, r, g, b);
    }
}

static void _position_ctrl_music_random(position_ctrl_t *position_ctrl, uint8_t len, uint8_t r, uint8_t g, uint8_t b, uint8_t amplitude)
{
    data uint8_t offset = position_ctrl->v.random_offset;

    if (IS_TIME_TO(position_ctrl->interval))
    {
        offset = amplitude % len;
    }

    cache_set(position_ctrl->cache_index, position_ctrl->start, len, 0, 0, 0);

    cache_set_pixel(position_ctrl->cache_index, position_ctrl->start + position_ctrl->v.random_offset, r, g, b);
    cache_set_pixel(position_ctrl->cache_index, position_ctrl->start + offset, r, g, b);

    position_ctrl->v.random_offset = offset;
}

static void __position_ctrl_music(uint8_t r, uint8_t g, uint8_t b, uint8_t len, position_ctrl_t *position_ctrl, color_ctrl_t *color_ctrl, uint8_t fre_index)
{
    data uint8_t pillars, amplitude;

    fft_div(len);
    pillars = fft_pillars_get(fre_index);
    amplitude = fft_amplitude_get(fre_index);

    switch (position_ctrl->position_type)
    {
        case E_POSITION_TYPE_MUSIC_MOVE_LEFT:
        case E_POSITION_TYPE_MUSIC_MOVE_RIGHT:
            if (amplitude <= 50) r = g = b = 0;
            __position_ctrl_move(position_ctrl, len, position_ctrl->position_type != E_POSITION_TYPE_MUSIC_MOVE_RIGHT, r, g, b);
            break;
        case E_POSITION_TYPE_MUSIC_STRETCH_LEFT:
        case E_POSITION_TYPE_MUSIC_STRETCH_RIGHT:
            _position_ctrl_music_stretch(position_ctrl, len, color_ctrl, false, r, g, b, pillars, false);
            break;
        case E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_LEFT:
        case E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT:
            _position_ctrl_music_stretch(position_ctrl, len, color_ctrl, true, r, g, b, pillars, false);
            break;
        case E_POSITION_TYPE_MUSIC_FLOAT_LEFT:
        case E_POSITION_TYPE_MUSIC_FLOAT_RIGHT:
            _position_ctrl_music_stretch(position_ctrl, len, color_ctrl, true, r, g, b, pillars, true);
            break;
        case E_POSITION_TYPE_MUSIC_INTERVAL_B:
        case E_POSITION_TYPE_MUSIC_INTERVAL_D:
            _position_ctrl_interval(position_ctrl, len, r, g, b);
            break;
        case E_POSITION_TYPE_MUSIC_RANDOM:
            _position_ctrl_music_random(position_ctrl, len, r, g, b, amplitude);
            break;
        case E_POSITION_TYPE_MUSIC_KEEP:
        default:
            cache_set(position_ctrl->cache_index, position_ctrl->start, len, r, g, b);
            break;
    }
}

void position_ctrl(hsv_t *hsv, position_ctrl_t *position_ctrl, color_ctrl_t *color_ctrl, uint8_t fre_index)
{
    data uint8_t r, g, b;
    data uint8_t len = POSITION_CTRL_LEN(position_ctrl);

    hsv_to_rgb(hsv, &r, &g, &b);

    if (position_ctrl->position_type >= E_POSITION_TYPE_MUSIC_KEEP)
    {
        __position_ctrl_music(r, g, b, len, position_ctrl, color_ctrl, fre_index);
    }
    else
    {
        __position_ctrl(r, g, b, len, position_ctrl, color_ctrl);
    }
}
