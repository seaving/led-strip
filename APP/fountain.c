#include "fountain.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __fountain_set(flush_param_t *param, uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
        param->fade_type = E_FADE_TYPE_KEEP;

    if (param->is_static_color)
    {
        param->mode = E_FOUNTAIN_MODE_SINGLE;
    }

    flush_fade_init(param, action_index, param->mode == E_FOUNTAIN_MODE_SINGLE);

    param->fade_interval *= 2;
    if (E_FOUNTAIN_POSITION_LEFT1 <= param->position && param->position <= E_FOUNTAIN_POSITION_CLOSE1)
    {
        ctrl_set(param, 12, 1, 1, 5, action_index, start, end, position_type, 0, 0, 0);
    } 
    else
    {
        ctrl_set(param, 2, 1, 1, 5, action_index, start, end, position_type, 0, 0, 0);
    }
}

void fountain_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    switch (param->position)
    {
        case E_FOUNTAIN_POSITION_LEFT:
        case E_FOUNTAIN_POSITION_LEFT1:
            __fountain_set(param, action_index, 0, 49, E_POSITION_TYPE_MOVE_LEFT);
            break;
        case E_FOUNTAIN_POSITION_RIGHT:
        case E_FOUNTAIN_POSITION_RIGHT1:
            __fountain_set(param, action_index, 0, 49, E_POSITION_TYPE_MOVE_RIGHT);
            break;
        case E_FOUNTAIN_POSITION_OPEN:
        case E_FOUNTAIN_POSITION_OPEN1:
            __fountain_set(param, action_index, 0, 24, E_POSITION_TYPE_MOVE_LEFT); action_index ++;
            __fountain_set(param, action_index, 25, 49, E_POSITION_TYPE_MOVE_RIGHT);
            break;
        case E_FOUNTAIN_POSITION_CLOSE:
        case E_FOUNTAIN_POSITION_CLOSE1:
            __fountain_set(param, action_index, 0, 24, E_POSITION_TYPE_MOVE_RIGHT); action_index ++;
            __fountain_set(param, action_index, 25, 49, E_POSITION_TYPE_MOVE_LEFT);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
