#include "action_com.h"
#include "color_ctrl.h"
#include "position_ctrl.h"
#include "cache.h"

void flush_fade_init(flush_param_t *param, uint8_t action_index, bit is_single)
{
    actions[action_index].hsv.v = param->adjust;

    if (is_single)
    {
        actions[action_index].hsv.h = COLOR1_HSV_H(param->color_index);
        actions[action_index].hsv.s = COLOR1_HSV_S(param->color_index);
        actions[action_index].hsv.v = COLOR1_HSV_V(param->color_index);

        if (param->fade_type == E_FADE_TYPE_KEEP)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SINGLE_KEEP);
        }
        else if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SINGLE_BREATH_V);
        }
        else if (param->fade_type == E_FADE_TYPE_FADE)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SINGLE_BREATH_S);
        }
        else if (param->fade_type == E_FADE_TYPE_SMOOTH)
        {
            actions[action_index].hsv.s = 100;
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H);
        }
    }
    else
    {
        if (param->fade_type == E_FADE_TYPE_KEEP)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_RANDOM_KEEP);
        }
        else if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_RANDOM_BREATH_V);
        }
        else if (param->fade_type == E_FADE_TYPE_FADE)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_RANDOM_BREATH_S);
        }
        else if (param->fade_type == E_FADE_TYPE_SMOOTH)
        {
            actions[action_index].hsv.s = 100;
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H);
        }
    }
}

void flush_fade_music_init(flush_param_t *param, uint8_t action_index, bit is_single)
{
    actions[action_index].hsv.v = param->adjust;

    if (is_single)
    {
        actions[action_index].hsv.h = COLOR1_HSV_H(param->color_index);
        actions[action_index].hsv.s = COLOR1_HSV_S(param->color_index);
        actions[action_index].hsv.v = COLOR1_HSV_V(param->color_index);

        if (param->fade_type == E_FADE_TYPE_KEEP)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_MUSIC_SINGLE_KEEP);
        }
        else if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_MUSIC_SINGLE_BREATH_V);
        }
        else if (param->fade_type == E_FADE_TYPE_FADE)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SINGLE_BREATH_S);
        }
        else if (param->fade_type == E_FADE_TYPE_SMOOTH)
        {
            actions[action_index].hsv.s = 100;
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H);
        }
    }
    else
    {
        if (param->fade_type == E_FADE_TYPE_KEEP)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_MUSIC_RANDOM_KEEP);
        }
        else if (param->fade_type == E_FADE_TYPE_BREATH)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_MUSIC_RANDOM_BREATH_V);
        }
        else if (param->fade_type == E_FADE_TYPE_FADE)
        {
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_RANDOM_BREATH_S);
        }
        else if (param->fade_type == E_FADE_TYPE_SMOOTH)
        {
            actions[action_index].hsv.s = 100;
            COLOR_CTRL_SET_CTRL_TYPE(&actions[action_index].color_ctrl, E_COLOR_CTRL_TYPE_SMOOTH_BREATH_H);
        }
    }
}

void ctrl_set(flush_param_t *param, uint8_t random_interval_level, uint8_t d_h, uint8_t d_s, uint8_t d_v, 
    uint8_t action_index, uint8_t start, uint8_t end, position_type_t position_type, 
    uint8_t adorn_index, bit is_adorn_random, uint8_t adorn_random_level)
{
    COLOR_CTRL_SET_BREATH_INTERVAL(&actions[action_index].color_ctrl, param->fade_interval);
    COLOR_CTRL_SET_RANDOM_INTERVAL_LEVEL(&actions[action_index].color_ctrl, random_interval_level);
    COLOR_CTRL_SET_BRIGHT_ADJUST(param->is_raise_adjust, param->adjust);
    COLOR_CTRL_SET_BREATH_SET_D_H(&actions[action_index].color_ctrl, d_h);
    COLOR_CTRL_SET_BREATH_SET_D_S(&actions[action_index].color_ctrl, d_s);
    COLOR_CTRL_SET_BREATH_SET_D_V(&actions[action_index].color_ctrl, d_v);
    COLOR_CTRL_SET_BREATH_SET_MAX_H_LEVEL(&actions[action_index].color_ctrl, 100);
    COLOR_CTRL_SET_BREATH_SET_MIN_S_LEVEL(&actions[action_index].color_ctrl, 80);
    COLOR_CTRL_SET_BREATH_SET_MAX_V(&actions[action_index].color_ctrl, param->adjust);

    COLOR_CTRL_SET_ADORN_INDEX(&actions[action_index].color_ctrl, adorn_index);
    COLOR_CTRL_SET_ADORN_RANDOM(&actions[action_index].color_ctrl, is_adorn_random);
    COLOR_CTRL_SET_ADORN_INVERVAL_LEVEL(&actions[action_index].color_ctrl, adorn_random_level);

    POSITION_CTRL_SET_INTERVAL(&actions[action_index].position_ctrl, param->position_interval);
    POSITION_CTRL_SET_INTERVAL_LEN(&actions[action_index].position_ctrl, param->bright_interval_len, param->dark_interval_len);
    POSITION_CTRL_SET_SECTION(&actions[action_index].position_ctrl, start, end);

    POSITION_CTRL_SET_TYPE(&actions[action_index].position_ctrl, position_type);
}
