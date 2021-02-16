#include "init.h"

int main(void)
{
    init();

    while (1)
    {
        ir();
        fft();
        play();
    }

    //return 0;
}
