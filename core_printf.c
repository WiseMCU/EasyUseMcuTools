/**
 * @file      core_printf.c
 * @brief     信息输出相关函数
 * @version   1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "core_printf.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
 * SEGGER RTT组件数据发送互斥信号量
 */
static osMutexId_t segger_rtt_mutex;

/* Private function prototypes -----------------------------------------------*/

/* Exported function ---------------------------------------------------------*/
/**
 * SEGGER RTT组件初始化
 *
 * @return 初始化成功与否状态
 */
int32_t segger_rtt_init(void)
{
    /* 创建互斥信号量 */
    segger_rtt_mutex = osMutexNew(&(const osMutexAttr_t ){.name = "SEGGER RTT MUTEX"});

    /* 发送清屏指令，RTT通道0将自动初始化 */
    SEGGER_RTT_WriteString(0, RTT_CTRL_CLEAR);

    return 0;
}

/**
 * 将格式化数据打印到标准输出
 *
 * @param[in] fmt 定义同标准库printf函数
 * @note 不可在中断服务程序或中断回调函数中使用
 */
void core_printf(const char *fmt, ...)
{
    osMutexAcquire(segger_rtt_mutex, osWaitForever);
    va_list v_args;
    va_start(v_args, fmt);
    SEGGER_RTT_vprintf(0, fmt, &v_args);
    va_end(v_args);
    osMutexRelease(segger_rtt_mutex);
}

/**
 * 将格式化指令数据打印到标准输出
 *
 * @param[in] para 指令的参数指针
 * @param[in] paralen 指令参数长度
 * @param[in] fmt 定义同标准库printf函数
 * @note 不可在中断服务程序或中断回调函数中使用
 */
void cmd_printf(char* para, uint32_t paralen, char *fmt, ...)
{
    osMutexAcquire(segger_rtt_mutex, osWaitForever);
    va_list v_args;
    va_start(v_args, fmt);
    SEGGER_RTT_vprintf(0, fmt, &v_args);
    va_end(v_args);
    if(para != NULL)
    {
        for(int i = 0; i < paralen; i++)
        {
            SEGGER_RTT_printf(0, " 0x%02x", para[i]);
        }
    }
    SEGGER_RTT_printf(0, "\r\n");
    osMutexRelease(segger_rtt_mutex);
}

/* Private function ----------------------------------------------------------*/

/********************************* END OF FILE ********************************/
