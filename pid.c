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
 * @param[in] output_ramp 输出变化率限制
 * @param[in] limit 输出限制
 */
void pid_init(pid_t *pid, float kp, float ki, float kd, float output_ramp, float limit)
{
    pid->kp = kp; // 设置比例系数
    pid->ki = ki; // 设置积分系数
    pid->kd = kd; // 设置微分系数
    pid->output_ramp = output_ramp; // 设置输出变化率限制
    pid->limit = limit; // 设置输出限制
    pid->prev_error = 0.0f; // 初始化上一次误差
    pid->prev_output = 0.0f; // 初始化上一次输出
    pid->prev_integral = 0.0f; // 初始化上一次积分值
    pid->prev_timestamp = HAL_GetTick(); // 假设使用 HAL 库获取系统时间
}

/**
 * PID控制器计算
 *
 * @param[in] pid   PID控制器指针
 * @param[in] error 当前误差
 * @return          控制器输出
 */
float pid_calc(pid_t *pid, float error)
{
    uint64_t timestamp_now = HAL_GetTick();
    float ts = (timestamp_now - pid->prev_timestamp) / 1000.0f; // 时间差转换为秒
    if (ts <= 0.0f || ts > 0.5f) ts = 0.001f; // 时间差异常处理

    float proportional = pid->kp * error; // 计算比例项
    float integral = pid->prev_integral + pid->ki * ts * (error + pid->prev_error) / 2.0f; // 计算积分项，使用梯形积分近似
    integral = CONSTRAIN(integral, -pid->limit, pid->limit); // 积分限制

    float derivative = ts > 0.0f ? pid->kd * (error - pid->prev_error) / ts : 0.0f; // 计算微分项

    float output = proportional + integral + derivative; // 计算总输出
    output = CONSTRAIN(output, -pid->limit, pid->limit); // 限制输出

    if (pid->output_ramp > 0.0f) {
        float output_rate = (output - pid->prev_output) / ts; // 计算输出变化率
        output_rate = CONSTRAIN(output_rate, -pid->output_ramp, pid->output_ramp); // 限制输出变化率
        output = pid->prev_output + output_rate * ts; // 应用输出变化率限制
    }

    pid->prev_integral = integral; // 更新积分值
    pid->prev_output = output; // 更新输出
    pid->prev_error = error; // 更新误差
    pid->prev_timestamp = timestamp_now; // 更新时间戳

    return output;
}

/**
 * 设置 PID 参数
 *
 * @param[in] pid         PID控制器指针
 * @param[in] kp          比例系数
 * @param[in] ki          积分系数
 * @param[in] kd          微分系数
 * @param[in] output_ramp 输出变化率限制
 */
void pid_set(pid_t *pid, float kp, float ki, float kd, float output_ramp)
{
    pid->kp = kp; // 更新比例系数
    pid->ki = ki; // 更新积分系数
    pid->kd = kd; // 更新微分系数
    pid->output_ramp = output_ramp; // 更新输出变化率限制
}