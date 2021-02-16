#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "action.h"
#include "action_com.h"

typedef enum __random_mode__
{
    E_RANDOM_MODE_SINGLE = 0,
    E_RANDOM_MODE_RANDOM,
} random_mode_t;

typedef enum __random_posistion__
{
    E_RANDOM_POSITION_RANDOM = 0,
    E_RANDOM_POSITION_RANDOM1
} random_posistion_t;

void random_set(flush_param_t *param);

#endif
