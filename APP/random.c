#include "random.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __random_set(flush_param_t *param, uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
        param->fade_type = E_FADE_TYPE_KEEP;

    if (param->is_static_color)
    {
        param->mode = E_RANDOM_MODE_SINGLE;
    }

    if (param->mode == E_RANDOM_MODE_SINGLE)
    {
        flush_fade_init(param, action_index, true);
        if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SINGLE_HARMONIC_V);
        }
    }
    else if (param->mode == E_RANDOM_MODE_RANDOM)
    {
        flush_fade_init(param, action_index, false);
        if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_RANDOM_HARMONIC_V);
        }
    }

    ctrl_set(param, 1, 1, 1, 10, action_index, start, end, position_type, 0, 0, 0);
}

void random_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    switch (param->position)
    {
        case E_RANDOM_POSITION_RANDOM:
            __random_set(param, action_index, 0, 16, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 3;
            __random_set(param, action_index, 17, 32, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 4;
            __random_set(param, action_index, 33, 49, E_POSITION_TYPE_RANDOM);
            break;
        case E_RANDOM_POSITION_RANDOM1:
            __random_set(param, action_index, 0, 9, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 3;
            __random_set(param, action_index, 10, 19, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 4;
            __random_set(param, action_index, 20, 29, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 2;
            __random_set(param, action_index, 30, 39, E_POSITION_TYPE_RANDOM);
            action_index ++;
            param->position_interval += 1;
            __random_set(param, action_index, 40, 49, E_POSITION_TYPE_RANDOM);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
