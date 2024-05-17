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
#ifndef SVPWM_H
#define SVPWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/* Define   ------------------------------------------------------------------*/
/* SVPWM模块输入参数模式 */
typedef enum
{
    /* 电矢量正交分量输入模式 */
    ALPHA_BETA  = 0x00U,
    /* 电矢量输入模式 */
    US_ANGLE    = 0x01U
} para_type_t;

/* SVPWM模块结构体 */
typedef struct sSVPWM
{
    /* 必要输入 */
    para_type_t para_type;  // SVPWM模块参数输入模式
    float   u_dc;           // 母线电压,单位伏
    float   counter_period; // 定时器周期计数值,即定时器PWM满占空比时的值

    /* 电矢量正交分量输入模式,当 para_type == ALPHA_BETA 生效 */
    float   u_alpha;        // Alpha轴的模,单位伏
    float   u_beta;         // Beta轴的模,单位伏

    /* 电矢量输入模式,当 para_type == US_ANGLE 生效 */
    float   vector_value;   // 电矢量的模,单位伏
    float   vector_angle;   // 电矢量的角度,单位弧度,输入范围 [0, 2 * PI)

    /* 三相输出的定时器周期占空比值 */
    float   a_pwm_duty;      // A相的PWM的比较器值，即比较器的配置值
    float   b_pwm_duty;      // B相的PWM的比较器值，即比较器的配置值
    float   c_pwm_duty;      // C相的PWM的比较器值，即比较器的配置值

    /* 计算三相占空比输出值的函数指针 */
    void (*m_calc)(struct sSVPWM*);
} svpwm_t;

/* 宏定义SVPWM模块初始值 */
#define SVPWM_DEFAULTS {            \
    .para_type      = ALPHA_BETA,   \
    .u_alpha        = 0.0f,         \
    .u_beta         = 0.0f,         \
    .vector_value   = 0.0f,         \
    .vector_angle   = 0.0f,         \
    .u_dc           = 0.0f,         \
    .counter_period = 0.0f,         \
    .a_pwm_duty     = 0.0f,         \
    .b_pwm_duty     = 0.0f,         \
    .c_pwm_duty     = 0.0f,         \
    .m_calc         = svpwm_calc    \
}

/* Function ------------------------------------------------------------------*/
/**
 * 根据输入电矢量计算对应三相的PWM占空比
 *
 * @param[in] svpwm_handle SVPWM模块句柄
 * @note 通过结构体里面的函数指针调用,也可以直接调用函数接口
 */
void svpwm_calc(svpwm_t* svpwm_handle);

#ifdef __cplusplus
}
#endif

#endif /* SVPWM_H */
