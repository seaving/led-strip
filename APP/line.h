#ifndef __LINE_H__
#define __LINE_H__

#include "typedef.h"
#include "action_com.h"

typedef enum __line_mode__
{
    E_LINE_MODE_SINGLE = 0,     //单色模式
    E_LINE_MODE_RANDOM,         //随机模式
} line_mode_t;

typedef enum __line_posistion__
{
    E_LINE_POSITION_KEEP = 0,
} line_posistion_t;

void line_set(flush_param_t *param);

#endif
