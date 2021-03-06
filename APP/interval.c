#include "interval.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __interval_set(flush_param_t *param, uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
        param->fade_type = E_FADE_TYPE_KEEP;

    if (param->is_static_color)
    {
        param->mode = E_INTERVAL_MODE_SINGLE;
    }

    flush_fade_init(param, action_index, param->mode == E_INTERVAL_MODE_SINGLE);

    ctrl_set(param, 10, 1, 1, 1, action_index, start, end, position_type, 0, 0, 0);
}

void interval_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    switch (param->position)
    {
        case E_INTERVAL_POSITION_INTERVAL_B:
            __interval_set(param, action_index, 0, 49, E_POSITION_TYPE_INTERVAL_B);
            break;
        case E_INTERVAL_POSITION_INTERVAL_D:
            __interval_set(param, action_index, 0, 49, E_POSITION_TYPE_INTERVAL_D);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
