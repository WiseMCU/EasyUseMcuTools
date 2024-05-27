/**
 * @file      svpwm.c
 * @brief     SVPWM相关函数
 * @version   1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "svpwm.h"

/* Define   ------------------------------------------------------------------*/
#define M_Pi    3.1415926535897f
#define M_Pi_3  (M_Pi/3.0f)

const uint8_t au8PermuataionMatrix[6][3] =
{    
    { 1, 2, 0 },
    { 3, 1, 0 },
    { 0, 1, 2 },
    { 0, 3, 1 },
    { 2, 0, 1 },
    { 1, 0, 3 }
};

/* Function ------------------------------------------------------------------*/
/**
 * 根据输入电矢量计算对应三相的PWM占空比
 *
 * @param[in] svpwm_handle SVPWM模块句柄
 * @note 通过结构体里面的函数指针调用,也可以直接调用函数接口
 */
void svpwm_calc(svpwm_t* svpwm_handle)
{    
    uint8_t sector;
    float fMaxUs, fScaledUs, fBeta, fTb1, fTb2, afTi[4];
    
    fMaxUs = svpwm_handle->u_dc * (1.0f/sqrtf(3.0f));
    
    switch(svpwm_handle->para_type)
    {
        case ALPHA_BETA:
            svpwm_handle->vector_value = hypotf(svpwm_handle->u_beta, svpwm_handle->u_alpha);
            svpwm_handle->vector_angle = atan2f(svpwm_handle->u_beta, svpwm_handle->u_alpha);
            svpwm_handle->vector_angle += M_Pi;
        break;
        
        case US_ANGLE:
            svpwm_handle->vector_value = fabsf(svpwm_handle->vector_value);
        break;
        
        default: return;
    }
    
    if(svpwm_handle->vector_value > fMaxUs) svpwm_handle->vector_value = fMaxUs;
    
    fScaledUs = svpwm_handle->vector_value / fMaxUs;
    
    sector = (uint8_t)(svpwm_handle->vector_angle * (1.0f / M_Pi_3));
    
    fBeta = svpwm_handle->vector_angle - M_Pi_3 * sector;
    
    fTb1 = fScaledUs * sinf(M_Pi_3 - fBeta);
    fTb2 = fScaledUs * sinf(fBeta);
    
    afTi[0] = (1.0f - fTb1 - fTb2)*0.5f;
    afTi[1] = fTb1 + fTb2 + afTi[0];
    afTi[2] = fTb2 + afTi[0];
    afTi[3] = fTb1 + afTi[0];
        
    svpwm_handle->a_pwm_duty = svpwm_handle->counter_period * afTi[au8PermuataionMatrix[sector][0]];
    svpwm_handle->b_pwm_duty = svpwm_handle->counter_period * afTi[au8PermuataionMatrix[sector][1]];
    svpwm_handle->c_pwm_duty = svpwm_handle->counter_period * afTi[au8PermuataionMatrix[sector][2]];
}
