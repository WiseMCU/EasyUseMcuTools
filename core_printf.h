/**
 * @file      core_printf.h
 * @brief     信息输出相关函数
 * @version   1.0
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
 *            [00:00:00.00] This is a cmd log 0x01 0x02...  (白色字体)
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CORE_PRINTF_H
#define CORE_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "SEGGER_RTT.h"
#include "cmsis_os2.h"

/* Exported define -----------------------------------------------------------*/
#if(DEBUG_ENABLE)
#if(DEBUG_TIME_FORMAT_ENABLE)
#define GET_TIME()      (osKernelGetTickCount() / 1000 / 3600),      \
                        (osKernelGetTickCount() / 1000 % 3600 / 60), \
                        (osKernelGetTickCount() / 1000 % 60),        \
                        (osKernelGetTickCount() % 1000)
#define TIME_FMT        "[%02d:%02d:%02d.%03d] "
#else
#define GET_TIME()      osKernelGetTickCount()
#define TIME_FMT        "[%8d] "
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
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_LOG */

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
#endif /* DEBUG_LEVEL >= DEBUG_LEVEL_LOG */

#else

#define DEBUG(fmt, args...)
#define INFO(fmt, args...)
#define WARNING(fmt, args...)
#define ERROR(fmt, args...)
#define CMD_LOG(para, paralen, fmt, args...)

#endif /* DEBUG_ENABLE */

/* Exported typedef ----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/
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
extern void cmd_printf(char* para, uint32_t paralen, char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* CORE_PRINTF_H */

/********************************* END OF FILE ********************************/
