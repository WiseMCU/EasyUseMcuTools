/**
 * @file      pid.c
 * @brief     PID相关函数
 * @version   1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"

/* Define   ------------------------------------------------------------------*/

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
void pid_init(pid_t *pid_tcb, float kp, float ki, float kd, float filter_para, float cycle_time)
{
    pid_tcb->kp = kp;
    pid_tcb->ki = ki;
    pid_tcb->kd = kd;
    pid_tcb->filter_para = filter_para;
    pid_tcb->cycle_time = cycle_time;
    pid_tcb->integrator_data = 0.0f;
    pid_tcb->filter_data = 0.0f;
}

/**
 * PID控制器计算
 *
 * @param[in] pid       PID控制器指针
 * @param[in] target    期望值
 * @param[in] current   当前值
 * @return              控制器输出
 */
float pid_calc(pid_t *pid_tcb, float target, float current)
{
    /* 计算误差 */
    float error = target - current;

    /* 计算滤波 */
    float filter_buf = (pid_tcb->kd * error - pid_tcb->filter_data) * pid_tcb->filter_para;

    /* 计算PID输出 */
    float pid_out = (pid_tcb->kp * error + pid_tcb->integrator_data) + filter_buf;

    /* 更新积分 */
    pid_tcb->integrator_data += pid_tcb->ki * error * pid_tcb->cycle_time;

    /* 更新滤波器 */
    pid_tcb->filter_data += pid_tcb->cycle_time * filter_buf;

    return pid_out;
}

/**
 * 设置 PID 参数
 *
 * @param[in] pid   PID控制器指针
 * @param[in] kp    比例系数
 * @param[in] ki    积分系数
 * @param[in] kd    微分系数
 */
void pid_set(pid_t *pid_tcb, float kp, float ki, float kd)
{
    pid_tcb->kp = kp;   // 更新比例系数
    pid_tcb->ki = ki;   // 更新积分系数
    pid_tcb->kd = kd;   // 更新微分系数
}