/**
 * @file      core_printf.h
 * @brief     信息输出相关函数
 * @version   1.1
 *------------------------------------------------------------------------------
 * @attention 使用示例：
 *            ① 初始化：segger_rtt_init();
 *            ② 调用宏输出不同等级的信息：
 *            DEBUG   ("This is a debug information...");
 *            INFO    ("This is a information...");
 *            WARNING ("This is a warning...");
 *            ERROR   ("This is a error...");
 *            char test[2] = {1, 2};
 *            CMD_LOG(test, sizeof(test), "This is a cmd log");
 *            ③ 终端显示：
 *            [00:00:00.00] This is a debug information...  (白色字体)
 *            [00:00:00.00] This is a information...        (绿色字体)
 *            [00:00:00.00] This is a warning...            (黄色字体)
 *            [00:00:00.00] This is a error...              (红色字体)
 *            [00:00:00.00] This is a cmd log 0x01 0x02     (白色字体)
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CORE_PRINTF_H
#define CORE_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "SEGGER_RTT.h"

/* Define   ------------------------------------------------------------------*/
/**
 * 调试信息输出等级设置
 *
 * @note 高于此等级的信息不会输出，开发后期可将等级设置为DEBUG_LEVEL_WARNING
 */
#define DEBUG_LEVEL_ERROR               0
#define DEBUG_LEVEL_WARNING             1
#define DEBUG_LEVEL_INFO                2
#define DEBUG_LEVEL_LOG                 3

/**
 * 已兼容的操作系统
*/
#define RTT_TOOL_NONE                   0 // 不使用操作系统
#define RTT_TOOL_THREADX                1 // 使用ThreadX操作系统
#define RTT_TOOL_RTX5                   2 // 使用RTX5操作系统
#define RTT_TOOL_RTT                    3 // 使用RT-Thread操作系统

/**
 * 调试信息输出总开关
 */
#ifndef DEBUG_ENABLE
    /* 默认开启打印 */
    #define DEBUG_ENABLE
#endif

/**
 * 调试信息时间格式化设置
 *
 * @note 时间格式化将消耗较多算力，若资源紧张可直接注释此宏定义
 */
#ifndef DEBUG_TIME_FORMAT_ENABLE
    /* 默认开启时间格式化 */
    #define DEBUG_TIME_FORMAT_ENABLE
#endif

/**
 * 屏蔽打印等级配置
 */
#ifndef DEBUG_LEVEL
    /* 默认所有等级都打印 */
    #define DEBUG_LEVEL         DEBUG_LEVEL_LOG
#endif

/**
 * 操作系统配置定义
 */
#ifndef RTT_TOOL_THREAD
    /* 默认不使用操作系统 */
    #define RTT_TOOL_THREAD     RTT_TOOL_NONE
#endif

#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    /* 使用ThreadX操作系统 */
    #include "tx_api.h"
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    /* 使用RTX5操作系统 */
    #include "cmsis_os2.h"
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTT)
    /* 使用RT-Thread操作系统 */
    #include "rtthread.h"
#elif (RTT_TOOL_THREAD == RTT_TOOL_NONE)
    /* 无操作系统 */
#endif

#ifdef DEBUG_ENABLE
#ifdef DEBUG_TIME_FORMAT_ENABLE
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    #define GET_TIME()      (tx_time_get() / 1000 / 3600),      \
                            (tx_time_get() / 1000 % 3600 / 60), \
                            (tx_time_get() / 1000 % 60),        \
                            (tx_time_get() / 10 % 100)
    #define TIME_FMT        "[%02d:%02d:%02d.%03d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    #define GET_TIME()      (osKernelGetTickCount() / 1000 / 3600),      \
                            (osKernelGetTickCount() / 1000 % 3600 / 60), \
                            (osKernelGetTickCount() / 1000 % 60),        \
                            (osKernelGetTickCount() % 1000)
    #define TIME_FMT        "[%02d:%02d:%02d.%03d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTT)
    #define GET_TIME()      (rt_tick_get() / 1000 / 3600),      \
                            (rt_tick_get() / 1000 % 3600 / 60), \
                            (rt_tick_get() / 1000 % 60),        \
                            (rt_tick_get() % 1000)
    #define TIME_FMT        "[%02d:%02d:%02d.%03d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_NONE)
    #define GET_TIME()      0, 0, 0, 0
    #define TIME_FMT        "[%02d:%02d:%02d.%03d] "
#endif
#else
#if (RTT_TOOL_THREAD == RTT_TOOL_THREADX)
    /* 使用ThreadX操作系统 */
    #define GET_TIME()      tx_time_get()
    #define TIME_FMT        "[%8d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTX5)
    /* 使用RTX5操作系统 */
    #define GET_TIME()      osKernelGetTickCount()
    #define TIME_FMT        "[%8d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_RTT)
    /* 使用RT-Thread操作系统 */
    #define GET_TIME()      rt_tick_get()
    #define TIME_FMT        "[%8d] "
#elif (RTT_TOOL_THREAD == RTT_TOOL_NONE)
    /* 无操作系统 */
    #define GET_TIME()      0
    #define TIME_FMT        "[%8d] "
#endif
#endif /* DEBUG_TIME_FORMAT_ENABLE */

#if (DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG)
#define DEBUG(fmt, args...)                                 \
do{                                                         \
    core_printf(RTT_CTRL_TEXT_BRIGHT_WHITE TIME_FMT fmt     \
                RTT_CTRL_TEXT_BRIGHT_WHITE "\r\n",          \
                GET_TIME(),                                 \
                ##args);                                    \
}while(0)
#else
#define DEBUG(fmt, args...)
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG */

#if (DEBUG_LEVEL >= DEBUG_LEVEL_INFO)
#define INFO(fmt, args...)                                  \
do {                                                        \
    core_printf(RTT_CTRL_TEXT_BRIGHT_GREEN TIME_FMT fmt     \
                RTT_CTRL_TEXT_BRIGHT_WHITE "\r\n",          \
                GET_TIME(),                                 \
                ##args);                                    \
} while(0)
#else
#define INFO(fmt, args...)
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_INFO */

#if (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define WARNING(fmt, args...)                               \
do {                                                        \
    core_printf(RTT_CTRL_TEXT_BRIGHT_YELLOW TIME_FMT fmt    \
                RTT_CTRL_TEXT_BRIGHT_WHITE "\r\n",          \
                GET_TIME(),                                 \
                ##args);                                    \
} while(0)
#else
#define WARNING(fmt, args...)
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_WARNING */

#if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define ERROR(fmt, args...)                                 \
do {                                                        \
    core_printf(RTT_CTRL_TEXT_BRIGHT_RED TIME_FMT fmt       \
                RTT_CTRL_TEXT_BRIGHT_WHITE "\r\n",          \
                GET_TIME(),                                 \
                ##args);                                    \
} while(0)
#else
#define ERROR(fmt, args...)
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_WARNING */

#if (DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG)
#define CMD_LOG(para, paralen, fmt, args...)                \
do {                                                        \
    cmd_printf( para, paralen,                              \
                RTT_CTRL_TEXT_BRIGHT_WHITE TIME_FMT fmt,    \
                GET_TIME(),                                 \
                ##args);                                    \
} while(0)
#else
#define CMD_LOG(para, paralen, fmt, args...)
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG */

#else

#define DEBUG(fmt, args...)
#define INFO(fmt, args...)
#define WARNING(fmt, args...)
#define ERROR(fmt, args...)
#define CMD_LOG(para, paralen, fmt, args...)

#endif /* DEBUG_ENABLE */

/* Function ------------------------------------------------------------------*/
/**
 * SEGGER RTT组件初始化
 *
 * @return 初始化成功与否状态
 */
extern int32_t segger_rtt_init(void);

/**
 * 将格式化数据打印到标准输出
 *
 * @param[in] fmt 定义同标准库printf函数
 * @note 不可在中断服务程序或中断回调函数中使用
 */
extern void core_printf(const char *fmt, ...);

/**
 * 将格式化指令数据打印到标准输出
 *
 * @param[in] para 指令的参数指针
 * @param[in] paralen 指令参数长度
 * @param[in] fmt 定义同标准库printf函数
 * @note 不可在中断服务程序或中断回调函数中使用
 */
extern void cmd_printf(char* para, uint32_t paralen, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* CORE_PRINTF_H */
