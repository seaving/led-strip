#ifndef __LINE_MUSIC_H__
#define __LINE_MUSIC_H__

#include "typedef.h"
#include "action_com.h"

typedef enum __line_music_mode__
{
    E_LINE_MUSIC_MODE_SINGLE = 0,     //单色模式
    E_LINE_MUSIC_MODE_RANDOM,         //随机模式
} line_music_mode_t;

typedef enum __line_music_posistion__
{
    E_LINE_MUSIC_POSITION_KEEP = 0,
} line_music_posistion_t;

void line_music_set(flush_param_t *param);

#endif
