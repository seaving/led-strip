#ifndef __RANDOM_MUSIC_H__
#define __RANDOM_MUSIC_H__

#include "action.h"
#include "action_com.h"

typedef enum __random_music_mode__
{
    E_RANDOM_MUSIC_MODE_SINGLE = 0,
    E_RANDOM_MUSIC_MODE_RANDOM,
} random_music_mode_t;

typedef enum __random_music_posistion__
{
    E_RANDOM_MUSIC_POSITION_RANDOM = 0,
    E_RANDOM_MUSIC_POSITION_RANDOM1,
} random_music_posistion_t;

void random_music_set(flush_param_t *param);

#endif
