/**
 * @file      filter.h
 * @brief     Filter相关函数
 * @version   1.0
 *------------------------------------------------------------------------------
 * @attention 使用示例：
 *              ① 初始化Filter模块：Filter filter = {
 *                 .order  = 2,
 *                 .a      = {1.0f,-1.955578240315035f,  0.956543676511203f},
 *                 .b      = {0.000241359049041f, 0.000482718098083f, 0.000241359049041f},
 *                 .zi     = {0.999758640950951f, -0.956302317462154f},
 *              };
 *              ② 准备好待滤波数据,并计算打印：
 *              float input_data[len], output_data[len];
 *              filtfilt_calc(&filter, input_data, output_data, len);
 *              for (int i = 0; i < len; i++)
 *              {
 *                  DEBUG("input: %f, output: %f", input_data[i],  output_data[i]);
 *              }
 *              ③ 终端显示：
 *              [00:00:00.000] input: -6.315612, output: -6.550910
 *              ...
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FILTER_H
#define FILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>

/* Define   ------------------------------------------------------------------*/
/**
 * 宏定义滤波器最大阶数
 */
#ifndef FILTER_MAX_ORDER
    /* 默认滤波器最大阶数10阶 */
    #define FILTER_MAX_ORDER    10
#endif

/* 滤波器结构体 */
typedef struct {
    uint8_t order;
    float a[FILTER_MAX_ORDER + 1];
    float b[FILTER_MAX_ORDER + 1];
    float zi[FILTER_MAX_ORDER];
} Filter;

/* Function ------------------------------------------------------------------*/
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
extern void filtfilt_calc(Filter *filter, float *sig, float *sout, uint32_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_H */
