#include "random_music.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __random_music_set(flush_param_t *param, uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type, uint8_t fre_index)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
    {
        param->fade_type = E_FADE_TYPE_BREATH;
        param->fade_interval = 1;
    }

    if (param->is_static_color)
    {
        param->mode = E_RANDOM_MUSIC_MODE_SINGLE;
    }

    if (param->mode == E_RANDOM_MUSIC_MODE_SINGLE)
    {
        flush_fade_music_init(param, action_index, true);
    }
    else if (param->mode == E_RANDOM_MUSIC_MODE_RANDOM)
    {
        flush_fade_music_init(param, action_index, false);
    }

    param->fade_interval *= 2;
    ctrl_set(param, 1, 1, 1, 10, action_index, start, end, position_type, 0, 0, 0);
    actions[action_index].fre_index = fre_index;
}

void random_music_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    cache_clear(0);
    cache_clear(1);

    switch (param->position)
    {
        case E_RANDOM_MUSIC_POSITION_RANDOM:
            __random_music_set(param, action_index, 0, 16, E_POSITION_TYPE_MUSIC_RANDOM, 1); action_index ++;
            __random_music_set(param, action_index, 17, 32, E_POSITION_TYPE_MUSIC_RANDOM, 8); action_index ++;
            __random_music_set(param, action_index, 33, 49, E_POSITION_TYPE_MUSIC_RANDOM, 14);
            break;
        case E_RANDOM_MUSIC_POSITION_RANDOM1:
            __random_music_set(param, action_index, 0, 6, E_POSITION_TYPE_MUSIC_RANDOM, 1); action_index ++;
            __random_music_set(param, action_index, 7, 13, E_POSITION_TYPE_MUSIC_RANDOM, 3); action_index ++;
            __random_music_set(param, action_index, 14, 20, E_POSITION_TYPE_MUSIC_RANDOM, 5); action_index ++;
            __random_music_set(param, action_index, 21, 27, E_POSITION_TYPE_MUSIC_RANDOM, 7); action_index ++;
            __random_music_set(param, action_index, 28, 34, E_POSITION_TYPE_MUSIC_RANDOM, 9); action_index ++;
            __random_music_set(param, action_index, 35, 42, E_POSITION_TYPE_MUSIC_RANDOM, 11); action_index ++;
            __random_music_set(param, action_index, 43, 49, E_POSITION_TYPE_MUSIC_RANDOM, 13);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
