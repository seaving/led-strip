#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "action.h"
#include "action_com.h"

typedef enum __tetris_mode__
{
    E_TETRIS_MODE_SINGLE = 0,
    E_TETRIS_MODE_RANDOM
} tetris_mode_t;

typedef enum __tetris_posistion__
{
    E_TETRIS_POSITION_LEFT = 0,
    E_TETRIS_POSITION_RIGHT,
    E_TETRIS_POSITION_OPEN,
    E_TETRIS_POSITION_CLOSE,

    E_TETRIS_POSITION_LEFT1,
    E_TETRIS_POSITION_RIGHT1,
    E_TETRIS_POSITION_OPEN1,
    E_TETRIS_POSITION_CLOSE1,
} tetris_posistion_t;

void tetris_set(flush_param_t *param);

#endif
