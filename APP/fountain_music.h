#ifndef __FOUNTAIN_MUSIC_H__
#define __FOUNTAIN_MUSIC_H__

#include "action.h"
#include "action_com.h"

typedef enum __fountain_music_mode__
{
    E_FOUNTAIN_MUSIC_MODE_SINGLE = 0,
    E_FOUNTAIN_MUSIC_MODE_RANDOM
} fountain_music_mode_t;

typedef enum __fountain_music_posistion__
{
    E_FOUNTAIN_MUSIC_POSITION_LEFT = 0,
    E_FOUNTAIN_MUSIC_POSITION_RIGHT,
    E_FOUNTAIN_MUSIC_POSITION_OPEN,
    E_FOUNTAIN_MUSIC_POSITION_CLOSE
} fountain_music_posistion_t;

void fountain_music_set(flush_param_t *param);

#endif
