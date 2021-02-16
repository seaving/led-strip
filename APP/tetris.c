#include "tetris.h"
#include "cache.h"
#include "typedef.h"
#include "player.h"

static void __tetris_set(flush_param_t *param, uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type, bool adorn_random)
{
    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0)
        param->fade_type = E_FADE_TYPE_KEEP;

    if (param->is_static_color)
    {
        param->mode = E_TETRIS_MODE_SINGLE;
    }

    flush_fade_init(param, action_index, param->mode == E_TETRIS_MODE_SINGLE);

    ctrl_set(param, 1, 1, 1, 5, action_index, start, end, position_type, 15, adorn_random, 5);
}

void tetris_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    data bool adorn_random = false;
    if (E_TETRIS_POSITION_LEFT1 <= param->position && param->position <= E_TETRIS_POSITION_CLOSE1)
    {
        adorn_random = true;
    }

    switch (param->position)
    {
        case E_TETRIS_POSITION_LEFT:
            __tetris_set(param, action_index, 0, 24, E_POSITION_TYPE_TETRIS_LEFT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 25, 49, E_POSITION_TYPE_TETRIS_LEFT, adorn_random);
            break;
        case E_TETRIS_POSITION_LEFT1:
            __tetris_set(param, action_index, 0, 16, E_POSITION_TYPE_TETRIS_LEFT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 17, 33, E_POSITION_TYPE_TETRIS_LEFT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 34, 49, E_POSITION_TYPE_TETRIS_LEFT, adorn_random);
            break;
        case E_TETRIS_POSITION_RIGHT:
            __tetris_set(param, action_index, 0, 24, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 25, 49, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random);
            break;
        case E_TETRIS_POSITION_RIGHT1:
            __tetris_set(param, action_index, 0, 16, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 17, 33, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 34, 49, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random);
            break;
        case E_TETRIS_POSITION_OPEN:
        case E_TETRIS_POSITION_OPEN1:
            __tetris_set(param, action_index, 0, 24, E_POSITION_TYPE_TETRIS_LEFT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 25, 49, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random);
            break;
        case E_TETRIS_POSITION_CLOSE:
        case E_TETRIS_POSITION_CLOSE1:
            __tetris_set(param, action_index, 0, 24, E_POSITION_TYPE_TETRIS_RIGHT, adorn_random); action_index ++;
            __tetris_set(param, action_index, 25, 49, E_POSITION_TYPE_TETRIS_LEFT, adorn_random);
            break;
        default:
            break;
    }

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
