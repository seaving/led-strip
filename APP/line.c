#include "line.h"
#include "colors.h"
#include "color_ctrl.h"
#include "position_ctrl.h"
#include "cache.h"
#include "player.h"

void line_set(flush_param_t *param)
{
    data uint8_t action_index = 0;

    param->color_index = param->color_index % COLOR_HSV_ARRAY1_ROWS;

    if (param->fade_interval == 0) param->fade_type = E_FADE_TYPE_KEEP;

    ctrl_set(param, 10, 1, 1, 1, action_index, 0, 49, E_POSITION_TYPE_KEEP, 0, 0, 0);

    actions[action_index].hsv.v = param->adjust;

    if (param->is_static_color)
    {
        param->mode = E_LINE_MODE_SINGLE;
    }

    flush_fade_init(param, action_index, param->mode == E_LINE_MODE_SINGLE);

    //把配置的action设置到播放器
    player_set(0, 0, action_index, 0, true);
}
