#ifndef __FOUNTAIN_H__
#define __FOUNTAIN_H__

#include "action.h"
#include "action_com.h"

typedef enum __fountain_mode__
{
    E_FOUNTAIN_MODE_SINGLE = 0,
    E_FOUNTAIN_MODE_RANDOM
} fountain_mode_t;

typedef enum __fountain_posistion__
{
    E_FOUNTAIN_POSITION_LEFT = 0,
    E_FOUNTAIN_POSITION_RIGHT,
    E_FOUNTAIN_POSITION_OPEN,
    E_FOUNTAIN_POSITION_CLOSE,
    E_FOUNTAIN_POSITION_LEFT1,
    E_FOUNTAIN_POSITION_RIGHT1,
    E_FOUNTAIN_POSITION_OPEN1,
    E_FOUNTAIN_POSITION_CLOSE1
} fountain_posistion_t;

void fountain_set(flush_param_t *param);

#endif
