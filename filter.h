#ifndef FILTER_H
#define FILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/* 宏定义滤波器最大阶数 */
#define FILTER_MAX_ORDER 10

typedef struct {
    uint8_t order;
    float a[FILTER_MAX_ORDER + 1];
    float b[FILTER_MAX_ORDER + 1];
    float zi[FILTER_MAX_ORDER];
} Filter;

/**
 * 一维数字滤波器,与matlab的Filter一致
 *
 * @param[in] filter   滤波器参数结构体指针
 * @param[in] sig      输入信号数组
 * @param[out] sout    输出信号数组
 * @param[in] data_len 输入输出信号长度
 */
extern void filter_calc(Filter *filter, float *sig, float *sout, uint32_t data_len);

/**
 * 零相位数字滤波,与matlab的FiltFilt基本一致
 *
 * @param[in] filter   滤波器参数结构体指针
 * @param[in] sig      输入信号数组
 * @param[out] sout    输出信号数组
 * @param[in] data_len 输入输出信号长度
 */
extern void filtfilt_clac(Filter *filter, float *sig, float *sout, uint32_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_H */