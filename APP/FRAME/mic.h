#ifndef __MIC_H__
#define __MIC_H__

#include "hal_sys.h"
#include "hal_adc.h"

#define mic_init()              hal_adc_init()
#define mic_value(value)        value = hal_adc_value(); if (value < mic_valid_min) value = 0

extern data uint8_t mic_valid_min;

#endif
