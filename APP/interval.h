#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include "action.h"
#include "action_com.h"

typedef enum __interval_mode__
{
    E_INTERVAL_MODE_SINGLE = 0,
    E_INTERVAL_MODE_RANDOM
} interval_mode_t;

typedef enum __interval_posistion__
{
    E_INTERVAL_POSITION_INTERVAL_B = 0,
    E_INTERVAL_POSITION_INTERVAL_D
} interval_posistion_t;

void interval_set(flush_param_t *param);

#endif
