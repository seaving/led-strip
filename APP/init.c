#include "init.h"

extern void play_test(void);

void init(void)
{
    ir_init();
    mic_init();
    play_init();

    //drv_uart_init(35000000, 115200);

    //play_test();
}
