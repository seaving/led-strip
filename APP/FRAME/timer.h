#ifndef __TIMER_H__
#define __TIMER_H__

#include "typedef.h"

#define CUR_TIME_CNT()               timer_cnt
#define IS_TIME_TO(interval_ms)      (interval_ms == 0 || timer_cnt % (interval_ms) == 0)

extern data uint32_t timer_cnt;

#endif
