/**
 * @file      core_printf.c
 * @brief     信息输出相关函数
 * @version   1.1
 */

/* Includes ------------------------------------------------------------------*/
#include "core_printf.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* 定义缓冲区大小，可以根据实际情况调整 */
#define RTT_PRINTF_BUFFER_SIZE 256

#ifndef RTT_TOOL_THREAD
    #error RTT_TOOL_THREAD is no define
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
 * SEGGER RTT组件数据发送互斥信号量
 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    static TX_MUTEX segger_rtt_mutex;
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    static osMutexId_t segger_rtt_mutex;
#endif

/**
 * 如果未开启打印则不定义缓存区节约内存
*/
#if(DEBUG_ENABLE)
/* 创建缓冲区 */
static char rtt_buffer[RTT_PRINTF_BUFFER_SIZE];
#endif /* DEBUG_ENABLE */

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
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    tx_mutex_create(&segger_rtt_mutex, "SEGGER RTT MUTEX", TX_INHERIT);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    segger_rtt_mutex = osMutexNew(&(const osMutexAttr_t ){.name = "SEGGER RTT MUTEX"});
#endif

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
    va_list v_args;
    
    /* 获取互斥体 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    tx_mutex_get(&segger_rtt_mutex, TX_WAIT_FOREVER);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    osMutexAcquire(segger_rtt_mutex, osWaitForever);
#endif
    
    /* 开始处理可变参数并填充缓冲区 */
    va_start(v_args, fmt);
    vsnprintf(rtt_buffer, RTT_PRINTF_BUFFER_SIZE, fmt, v_args);
    va_end(v_args);
    
    /* 将整个缓冲区写入RTT */
    SEGGER_RTT_WriteString(0, rtt_buffer);
    
    /* 释放互斥体 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    tx_mutex_put(&segger_rtt_mutex);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    osMutexRelease(segger_rtt_mutex);
#endif
}

/**
 * 将格式化指令数据打印到标准输出
 *
 * @param[in] para 指令的参数指针
 * @param[in] paralen 指令参数长度
 * @param[in] fmt 定义同标准库printf函数
 * @note 不可在中断服务程序或中断回调函数中使用
 */
void cmd_printf(char* para, uint32_t paralen, const char *fmt, ...)
{
    va_list v_args;
    char *buffer_ptr = rtt_buffer;
    int buffer_space = RTT_PRINTF_BUFFER_SIZE;

    /* 获取互斥体 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    tx_mutex_get(&segger_rtt_mutex, TX_WAIT_FOREVER);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    osMutexAcquire(segger_rtt_mutex, osWaitForever);
#endif

    /* 开始处理可变参数并填充缓冲区 */
    va_start(v_args, fmt);
    int print_length = vsnprintf(buffer_ptr, buffer_space, fmt, v_args);
    va_end(v_args);

    /* 更新指针和剩余空间，检查溢出 */
    if (print_length > 0 && print_length < buffer_space)
    {
        buffer_ptr += print_length;
        buffer_space -= print_length;
    } else {
        /* 发生溢出，释放互斥体并返回 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
        tx_mutex_put(&segger_rtt_mutex);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
        osMutexRelease(segger_rtt_mutex);
#endif
        
        return;
    }

    /* 打印参数 */
    if (para != NULL && paralen > 0)
    {
        for (uint32_t i = 0; i < paralen && buffer_space > 0; i++)
        {
            print_length = snprintf(buffer_ptr, buffer_space, " 0x%02X", para[i]);
            if (print_length > 0 && print_length < buffer_space)
            {
                buffer_ptr += print_length;
                buffer_space -= print_length;
            } else {
                /* 发生溢出，释放互斥体并返回 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
                tx_mutex_put(&segger_rtt_mutex);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
                osMutexRelease(segger_rtt_mutex);
#endif
                return;
            }
        }
    }

    /* 添加换行符 */
    if (buffer_space > 2)
    {
        snprintf(buffer_ptr, buffer_space, "\r\n");
    }

    /* 将整个缓冲区写入RTT */
    SEGGER_RTT_WriteString(0, rtt_buffer);

    /* 释放互斥体 */
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    tx_mutex_put(&segger_rtt_mutex);
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    osMutexRelease(segger_rtt_mutex);
#endif
}

/* Private function ----------------------------------------------------------*/

/********************************* END OF FILE ********************************/
