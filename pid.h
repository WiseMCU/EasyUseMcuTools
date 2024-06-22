/**
 * @file      pid.h
 * @brief     PID相关函数
 * @version   1.0
 *------------------------------------------------------------------------------
 * @attention 使用示例：
 *              ① 初始化PID模块：pid_t pid;
 *              ② 配置PID模块输入参数,并计算打印：
 *              pid_calc(&pid, focdrv.id);
 *              ③ 终端显示：
 *              [00:00:00.000] Angle: 1.047198, A: 575.000061, B: 575.000061, C: 575.000061
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Define   ------------------------------------------------------------------*/
/* 定义 PID 结构体 */
typedef struct
{
    float kp;               // 比例系数
    float ki;               // 积分系数
    float kd;               // 微分系数
    float filter_para;      // 滤波系数
    /* 若10ms计算一次PID则 cycle_time = 0.01f */
    float cycle_time;       // PID计算周期时间s
    float filter_data;      // 滤波数据
    float integrator_data;  // 积分数据
}pid_t;

/* Function ------------------------------------------------------------------*/
/**
 * PID控制器初始化
 *
 * @param[in] pid PID控制器指针
 * @param[in] kp  比例系数
 * @param[in] ki  积分系数
 * @param[in] kd  微分系数
 * @param[in] filter_para 滤波器系数
 * @param[in] cycle_time  PID计算周期时间s
 */
extern void pid_init(pid_t *pid_tcb, float kp, float ki, float kd, float filter_para, float cycle_time);

/**
 * PID控制器计算
 *
 * @param[in] pid       PID控制器指针
 * @param[in] target    期望值
 * @param[in] current   当前值
 * @return              控制器输出
 */
extern float pid_calc(pid_t *pid_tcb, float target, float current);

/**
 * 设置 PID 参数
 *
 * @param[in] pid   PID控制器指针
 * @param[in] kp    比例系数
 * @param[in] ki    积分系数
 * @param[in] kd    微分系数
 */
extern void pid_set(pid_t *pid_tcb, float kp, float ki, float kd);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */