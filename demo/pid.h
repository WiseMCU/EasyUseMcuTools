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
/* 定义限制函数宏 */
#define CONSTRAIN(amt, low, high) \
    ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

/* 定义 PID 结构体 */
typedef struct {
    float kp;                   // 比例系数
    float ki;                   // 积分系数
    float kd;                   // 微分系数
    float output_ramp;          // 输出变化率限制
    float limit;                // 输出限制
    float prev_error;           // 上一次误差
    float prev_output;          // 上一次输出
    float prev_integral;        // 上一次积分值
    uint64_t prev_timestamp;    // 上一次时间戳
} pid_t;

/* Function ------------------------------------------------------------------*/
/**
 * PID控制器初始化
 *
 * @param[in] pid PID控制器指针
 * @param[in] kp  比例系数
 * @param[in] ki  积分系数
 * @param[in] kd  微分系数
 * @param[in] output_ramp 输出变化率限制
 * @param[in] limit 输出限制
 */
extern void pid_init(pid_t *pid, float kp, float ki, float kd, float output_ramp, float limit);

/**
 * PID控制器计算
 *
 * @param[in] pid   PID控制器指针
 * @param[in] error 当前误差
 * @return          控制器输出
 */
extern float pid_calc(pid_t *pid, float error);

/**
 * 设置 PID 参数
 *
 * @param[in] pid         PID控制器指针
 * @param[in] kp          比例系数
 * @param[in] ki          积分系数
 * @param[in] kd          微分系数
 * @param[in] output_ramp 输出变化率限制
 */
extern void pid_set(pid_t *pid, float kp, float ki, float kd, float output_ramp);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */