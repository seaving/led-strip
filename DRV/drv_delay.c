#include "drv_delay.h"

void drv_delay(uint16_t nop_cnt)
{
    data uint16_t i = 0;
    for (i = 0; i < nop_cnt; i ++)
    {
        _nop_(); //35MB 76ns
    }
}
