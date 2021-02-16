#include "stretch_music.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __stretch_music_set(flush_param_t *param, uint8_t action_index, uint8_t start, 
    uint8_t end, position_type_t position_type, uint8_t fre_index, uint8_t cache_index)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
        param->fade_type = E_FADE_TYPE_KEEP;

    if (param->is_static_color)
    {
        param->mode = E_STRETCH_MUSIC_MODE_SINGLE;
    }
    actions[action_index].fre_index = fre_index;
    POSITION_CTRL_SET_CACHE_INDEX(&actions[action_index].position_ctrl, cache_index);
    flush_fade_music_init(param, action_index, param->mode == E_STRETCH_MUSIC_MODE_SINGLE);

    if (E_STRETCH_MUSIC_POSITION_LEFT1 <= param->position 
        && param->position <= E_STRETCH_MUSIC_POSITION_CLOSE1)
    {
        ctrl_set(param, 10, 1, 1, 2, action_index, start, end, position_type, 15, param->mode == E_STRETCH_MUSIC_MODE_RANDOM, 1);
    }
    else
    {
        ctrl_set(param, 10, 1, 1, 2, action_index, start, end, position_type, 0, 0, 0);
    }
}

void stretch_music_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    cache_clear(0);
    cache_clear(1);

    switch (param->position)
    {
        case E_STRETCH_MUSIC_POSITION_LEFT:
            __stretch_music_set(param, action_index, 0, 49, E_POSITION_TYPE_MUSIC_STRETCH_LEFT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_LEFT1:
            __stretch_music_set(param, action_index, 0, 49, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_LEFT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_LEFT2:
            __stretch_music_set(param, action_index, 0, 6, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 7, 13, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 3, 0); action_index ++;
            __stretch_music_set(param, action_index, 14, 20, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 5, 0); action_index ++;
            __stretch_music_set(param, action_index, 21, 27, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 7, 0); action_index ++;
            __stretch_music_set(param, action_index, 28, 34, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 9, 0); action_index ++;
            __stretch_music_set(param, action_index, 35, 42, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 11, 0); action_index ++;
            __stretch_music_set(param, action_index, 43, 49, E_POSITION_TYPE_MUSIC_FLOAT_LEFT, 13, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_RIGHT:
            __stretch_music_set(param, action_index, 0, 49, E_POSITION_TYPE_MUSIC_STRETCH_RIGHT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_RIGHT1:
            __stretch_music_set(param, action_index, 0, 49, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_OPEN:
            __stretch_music_set(param, action_index, 0, 24, E_POSITION_TYPE_MUSIC_STRETCH_LEFT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 25, 49, E_POSITION_TYPE_MUSIC_STRETCH_RIGHT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_OPEN1:
            __stretch_music_set(param, action_index, 0, 24, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_LEFT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 25, 49, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_OPEN2:
            __stretch_music_set(param, action_index, 0, 24, E_POSITION_TYPE_MUSIC_STRETCH_LEFT, 12, 1); action_index ++;
            __stretch_music_set(param, action_index, 25, 49, E_POSITION_TYPE_MUSIC_STRETCH_RIGHT, 12, 1); action_index ++;

            param->is_static_color = true;
            param->fade_interval = 0;
            param->color_index = (param->color_index + 1) % COLOR_HSV_ARRAY1_ROWS;
            __stretch_music_set(param, action_index, 16, 24, E_POSITION_TYPE_MUSIC_STRETCH_LEFT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 25, 33, E_POSITION_TYPE_MUSIC_STRETCH_RIGHT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_CLOSE:
            __stretch_music_set(param, action_index, 0, 24, E_POSITION_TYPE_MUSIC_STRETCH_RIGHT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 25, 49, E_POSITION_TYPE_MUSIC_STRETCH_LEFT, 1, 0);
            break;
        case E_STRETCH_MUSIC_POSITION_CLOSE1:
            __stretch_music_set(param, action_index, 0, 24, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_RIGHT, 1, 0); action_index ++;
            __stretch_music_set(param, action_index, 25, 49, E_POSITION_TYPE_MUSIC_STRETCH_FLOAT_LEFT, 1, 0);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
