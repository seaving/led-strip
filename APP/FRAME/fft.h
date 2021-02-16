#ifndef __FFT_H__
#define __FFT_H__

#include "hal_sys.h"

#define SAMPLE_NUM              32
#define NUM_TO_LOG              5       //SAMPLE_NUM 采样的 2次幂
#define FFT_OUT_MIN             3

#define FFT_MAX_DIV             50

#define FFT_A_SIZE              16      //SAMPLE_NUM / 2

void fft(void);
void fft_div(uint8_t div);

#define fft_amplitude_get(offset)   fft_a[offset]                 //幅值

uint8_t fft_pillars_get(uint8_t offset);                            //柱体长度

extern data uint8_t fft_a[FFT_A_SIZE];

#endif
