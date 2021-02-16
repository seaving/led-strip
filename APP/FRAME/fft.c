#include "fft.h"
#include "mic.h"

static code uint8_t _br_table[SAMPLE_NUM] = {
    0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 
    1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31 
};

static code int8_t _sin_table[SAMPLE_NUM] = {
    0, 6, 12, 17, 22, 26, 29, 31, 32, 31, 29, 26, 22, 17, 12, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static code int8_t _cos_table[SAMPLE_NUM] = {
    32, 31, 29, 26, 22, 17, 12, 6, 0, -6, -12, -17, -22, -26, -29, -31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static data uint8_t _fft_sensitivity = 0;                        //调整频谱变化幅度，不要超过4

static xdata int16_t _fft_real[SAMPLE_NUM] = {0};
static xdata int16_t _fft_image[SAMPLE_NUM] = {0};

static data uint8_t _fft_fre_div = 0;
static data uint8_t _fft_a_max = 0;
static data float _fft_q = 0;

data uint8_t fft_a[FFT_A_SIZE] = {0};

void fft(void)
{
 	register uint8_t i = 0, j = 0, neighbour = 0;
    register uint16_t z = 0;
    register int16_t tr = 0, ti = 0, temp = 0;

    for (i = 0; i < SAMPLE_NUM; i ++)	                //此处可以加入自动增益
    {
        mic_value(_fft_real[_br_table[i]]);
        _fft_real[_br_table[i]] <<= _fft_sensitivity;   //调整频谱变化幅度，不要超过4
        _fft_image[i] = 0;
    }

#if 0
    printf("sssss--------\r\n");
    for (i = 0; i < SAMPLE_NUM; i ++)
        printf("%d, ", _fft_real[_br_table[i]]);
    printf("\r\n");
#endif

    temp = _fft_real[2] / 32;                 //提取等级数
    if ((7 < temp) && (temp <= 8))
    {
        _fft_sensitivity = 1;
    }
    else if ((4 < temp) && (temp <= 6))
    {
        _fft_sensitivity = 2;
    }
    else if ((2 < temp) && (temp <= 4))
    {
        _fft_sensitivity = 3;
    }
    else
    {
        _fft_sensitivity = 4;
    }

    for (i = 0; i < NUM_TO_LOG; i ++)
    {
        z = 0;
        for (j = 0; j < SAMPLE_NUM; j ++)
        {
            neighbour = 1 << i;
            if ((j / neighbour) % 2 == 1)
            {
                tr = _fft_real[j];
                ti = _fft_image[j];
                _fft_image[j] = ti * _cos_table[z] - tr * _sin_table[z];
                _fft_real[j] = tr * _cos_table[z] + ti * _sin_table[z];

                z += 1 << (NUM_TO_LOG - i - 1);
                neighbour = j - neighbour;
                tr = _fft_real[neighbour];
                ti = _fft_image[neighbour];

                //蝶形运算
                _fft_image[neighbour] = ti + _fft_image[j];
                _fft_real[neighbour] = tr + _fft_real[j];

                _fft_image[j] = ti - _fft_image[j];
                _fft_real[j] = tr - _fft_real[j];
            }
            else
            {
                z = 0;
            }
        }
    }

    _fft_real[0]= _fft_image[0] = 0;             //去掉直流分量

    //重新开始一轮分频
    _fft_a_max = 0;
    _fft_fre_div = 0;
    _fft_q = 0;

    for (i = 1; i <= FFT_A_SIZE; i ++)
    {
        j = i - 1;
        fft_a[j] = ((_fft_real[i] * _fft_real[i]) + (_fft_image[i] * _fft_image[i])) >> 8; //求各频段幅值，向量得膜
        if (fft_a[j] < FFT_OUT_MIN)                 //最小输出
        {
            fft_a[j] = 0;
        }
        else
        {
            fft_a[j] -= FFT_OUT_MIN;
        }

        if (fft_a[j] > _fft_a_max)
        {
            _fft_a_max = fft_a[j];
        }
    }

#if 0
    printf("ffta--------\r\n");
    for (i = 0; i < FFT_A_SIZE; i ++)
    {
        z = fft_a[i];
        printf("%d, ", z);
    }
    printf("\r\n");
    printf("zzz--------\r\n");
    for (i = 1; i < SAMPLE_NUM; i ++)
    {
        z = ((_fft_real[i] * _fft_real[i]) + (_fft_image[i] * _fft_image[i])) >> 8;
        printf("%d, ", z);
    }
    printf("\r\n");
#endif
}

void fft_div(uint8_t div)
{
    if (div == 0)
    {
        return;
    }

    if (_fft_fre_div != div)
    {
        _fft_fre_div = div;
        _fft_q = (float) _fft_a_max / (float) _fft_fre_div;
    }
}

uint8_t fft_pillars_get(uint8_t offset)
{
    if (_fft_q < 0.001)
    {
        return 0;
    }

    return (float) fft_a[offset] / _fft_q;
}
