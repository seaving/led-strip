#ifndef __STRETCH_MUSIC_H__
#define __STRETCH_MUSIC_H__

#include "action.h"
#include "action_com.h"

typedef enum __stretch_music_mode__
{
    E_STRETCH_MUSIC_MODE_SINGLE = 0,
    E_STRETCH_MUSIC_MODE_RANDOM
} stretch_music_mode_t;

typedef enum __stretch_music_posistion__
{
    E_STRETCH_MUSIC_POSITION_LEFT = 0,
    E_STRETCH_MUSIC_POSITION_RIGHT,
    E_STRETCH_MUSIC_POSITION_OPEN,
    E_STRETCH_MUSIC_POSITION_CLOSE,

    E_STRETCH_MUSIC_POSITION_LEFT1,
    E_STRETCH_MUSIC_POSITION_RIGHT1,
    E_STRETCH_MUSIC_POSITION_OPEN1,
    E_STRETCH_MUSIC_POSITION_CLOSE1,

    E_STRETCH_MUSIC_POSITION_LEFT2,
    E_STRETCH_MUSIC_POSITION_OPEN2,
} stretch_music_posistion_t;

void stretch_music_set(flush_param_t *param);

#endif
