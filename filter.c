/**
 * @file      filter.c
 * @brief     滤波功能函数
 * @version   1.0
 */

/* Includes -----------------------------------------------------------------*/
#include "filter.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

/* Define   ------------------------------------------------------------------*/

/* Function ------------------------------------------------------------------*/
/**
 * 一维数字滤波器,与matlab的Filter一致
 *
 * @param[in] filter   滤波器参数结构体指针
 * @param[in] sig      输入信号数组
 * @param[out] sout    输出信号数组
 * @param[in] data_len 输入输出信号长度
 */
void filter_calc(Filter *filter, float *sig, float *sout, uint32_t data_len)
{
    float xn[filter->order + 1], yn[filter->order + 1];

    sout[0] = 0;
    memset(yn, 0, sizeof(yn));
    memset(xn, 0, sizeof(xn));

    for (uint32_t i = 1; i < data_len; i++)
    {
        xn[0] = sig[i];

        sout[i] = filter->b[0] * xn[0];

        for (uint8_t j = 1; j <= filter->order; j++)
        {
            sout[i] += filter->b[j] * xn[j];
            sout[i] -= filter->a[j] * yn[j];
        }

        yn[0] = sout[i];

        xn[2] = xn[1], xn[1] = xn[0];
        yn[2] = yn[1], yn[1] = yn[0];
    }
}

/**
 * 零相位数字滤波,与matlab的FiltFilt基本一致
 *
 * @param[in] filter   滤波器参数结构体指针
 * @param[in] sig      输入信号数组
 * @param[out] sout    输出信号数组
 * @param[in] data_len 输入输出信号长度
 */
void filtfilt_clac(Filter *filter, float *sig, float *sout, uint32_t data_len)
{
    /* 最大阶数限制 */
    if(filter->order > FILTER_MAX_ORDER)
    {
        filter->order = FILTER_MAX_ORDER;
    }

    uint32_t nfact = 3 * (filter->order);    // 延拓因子,与matlab一致
    uint32_t len_ext = data_len + nfact * 2; // 延拓后的信号长度

    /* 定义延拓后的信号数组和滤波后的信号数组 */
    float s_ext[len_ext];
    float s_fft[len_ext];

    /* 数据延拓 */
    for(uint32_t i = 0; i < nfact; i++)
    {
        s_ext[i] = 2 * sig[0] - sig[nfact - i];
        s_ext[data_len + nfact + i] = 2 * sig[data_len - 1] - sig[data_len - i - 2];
    }

    memcpy(&s_ext[nfact], sig, sizeof(float) * data_len);

    /* 数据滤波 */
    memcpy(s_fft, s_ext, sizeof(float) * len_ext);

    for (uint32_t i = 0; i < filter->order; i++) 
    {
        float tmp = filter->b[0] * s_fft[i];
        
        tmp += filter->zi[i] * s_ext[0];

        for (uint8_t j = 1; j < i + 1; j++)
        {
            tmp += filter->b[j] * s_ext[i - j];
            tmp -= filter->a[j] * s_fft[i - j];
        }

        s_fft[i] = tmp;
    }

    for (uint32_t i = filter->order; i < len_ext; i++)
    {
        float tmp = filter->b[0] * s_fft[i];

        for (uint8_t j = 1; j <= filter->order; j++)
        {
            tmp += filter->b[j] * s_ext[i - j];
            tmp -= filter->a[j] * s_fft[i - j];
        }

        s_fft[i] = tmp;
    }

    /* 数据翻转 */
    for (uint32_t i = 0; i < len_ext; i++)
    {
        s_ext[len_ext - i - 1] = s_fft[i];
    }

    /* 数据滤波 */
    memcpy(s_fft, s_ext, sizeof(float) * len_ext);

    for (uint32_t i = 0; i < filter->order; i++) 
    {
        float tmp = filter->b[0] * s_fft[i];
        
        tmp += filter->zi[i] * s_ext[0];

        for (uint8_t j = 1; j < i + 1; j++)
        {
            tmp += filter->b[j] * s_ext[i - j];
            tmp -= filter->a[j] * s_fft[i - j];
        }

        s_fft[i] = tmp;
    }

    for (uint32_t i = filter->order; i < len_ext; i++)
    {
        float tmp = filter->b[0] * s_fft[i];

        for (uint8_t j = 1; j <= filter->order; j++)
        {
            tmp += filter->b[j] * s_ext[i - j];
            tmp -= filter->a[j] * s_fft[i - j];
        }

        s_fft[i] = tmp;
    }

    /* 数据翻转 */
    for (uint32_t i = 0; i < data_len; i++)
    {
        sout[i] = s_fft[len_ext - nfact - i];
    }
}
