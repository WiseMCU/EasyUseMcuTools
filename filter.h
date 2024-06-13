/**
 * @file      svpwm.h
 * @brief     SVPWM相关函数
 * @version   1.0
 *------------------------------------------------------------------------------
 * @attention 使用示例：
 *              ① 初始化SVPWM模块：svpwm_t svpwm = SVPWM_DEFAULTS;
 *              ② 配置SVPWM模块输入参数,并计算打印：
 *              #define PI    3.1415926535897f
 *              svpwm.para_type = US_ANGLE;
 *              svpwm.u_dc = 10.0f;
 *              svpwm.counter_period = 1000;
 *              svpwm.vector_value = 1.0f;
 *              svpwm.vector_angle = PI / 3;
 *              svpwm.m_calc(&svpwm);
 *              DEBUG("Angle: %f, A: %f, B: %f, C: %f", svpwm.vector_angle, 
 *                  svpwm.a_pwm_duty, svpwm.b_pwm_duty, svpwm.b_pwm_duty);
 *              ③ 终端显示：
 *              [00:00:00.000] Angle: 1.047198, A: 575.000061, B: 575.000061, C: 575.000061
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
/* 宏定义滤波器最大阶数 */
#define FILTER_MAX_ORDER 10

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
extern void filtfilt_clac(Filter *filter, float *sig, float *sout, uint32_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_H */