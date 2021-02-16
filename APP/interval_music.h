#ifndef __INTERVAL_MUSIC_H__
#define __INTERVAL_MUSIC_H__

#include "action.h"
#include "action_com.h"

typedef enum __interval_music_mode__
{
    E_INTERVAL_MUSIC_MODE_SINGLE = 0,
    E_INTERVAL_MUSIC_MODE_RANDOM
} interval_music_mode_t;

typedef enum __interval_music_posistion__
{
    E_INTERVAL_MUSIC_POSITION_INTERVAL_B = 0,
    E_INTERVAL_MUSIC_POSITION_INTERVAL_D
} interval_music_posistion_t;

void interval_music_set(flush_param_t *param);

#endif
