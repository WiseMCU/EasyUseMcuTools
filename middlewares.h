/**
 * @file      middlewares.h
 * @brief     集合库配置文件
 * @version   1.0
 * @date      2024-04-11
 * @author    WiseMCU
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef MIDDLEWARES_H
#define MIDDLEWARES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define ----------------------------------------------------------*/
/*---------------------------------- COMMON ---------------------------------*/
/**
 * 屏蔽未使用变量或函数编译警告，通用armcc、armclang、gcc
 */
#define __UNUSED                    __attribute__((unused))

/**
 * 定义文件名宏定义，强制去除绝对路径
 */
#if defined (__ARMCC_VERSION)
    #if __ARMCC_VERSION >= 6140000  /* ARM Compiler V6 */
        #ifndef __FILENAME__
            #define __FILENAME__  __FILE_NAME__
        #endif
    #else                           /* ARM Compiler V5 */
        #ifndef __FILENAME__
            #define __FILENAME__  __FILE__
        #endif
    #endif
#elif defined (__GNUC__) && !defined (__CC_ARM) /* GNU Compiler */
    #ifndef __FILENAME__
        #define __FILENAME__  __FILE_NAME__
    #endif
#endif /* __GNUC__ */

/**
 * 设置变量存放的绝对地址
 */
#ifndef __MEMORY_AT
    #if (defined (__CC_ARM))
        #define __MEMORY_AT(x)      __attribute__((at(x)))
    #elif (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
        #define __MEMORY_AT__(x)    __attribute__((section(".bss.ARM.__at_"#x)))
        #define __MEMORY_AT(x)      __MEMORY_AT__(x)
    #else
        #define __MEMORY_AT(x)
        #warning Position memory containing __MEMORY_AT macro at absolute address!
    #endif
#endif

/**
 * 获取数组元素个数
 */
#define ARRAY_SIZE(array)           (sizeof(array) / sizeof((array)[0]))

/**
 * 获取结构体成员占用空间大小
 */
#define FIELD_SIZEOF(type, member)  (sizeof(((type*)0)->member))

/**
 * 操作系统ID,所有工具内兼容操作系统使用ID一致
*/
#define THREAD_NONE         0 // 不使用操作系统
#define THREAD_THREADX      1 // 使用ThreadX操作系统
#define THREAD_RTX5         2 // 使用RTX5操作系统
#define THREAD_RT_THREAD    2 // 使用RT-Thread操作系统

/*---------------------------------- RTT LOG --------------------------------*/
/**
 * 调试信息输出总开关
 */
#ifndef DEBUG_ENABLE
    #define DEBUG_ENABLE            1
#endif

/**
 * 调试信息时间格式化设置
 *
 * @note 时间格式化将消耗较多算力，若资源紧张可直接注释此宏定义
 */
#ifndef DEBUG_TIME_FORMAT_ENABLE
    #define DEBUG_TIME_FORMAT_ENABLE    1
#endif

/**
 * 屏蔽打印等级配置
 */
#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL             DEBUG_LEVEL_LOG
#endif

/**
 * RTT打印操作系统配置定义
 */
#ifndef RTT_TOOL_THREAD
    #define RTT_TOOL_THREAD         RTT_TOOL_NONE
#endif


/*---------------------------------- Filter ---------------------------------*/
/**
 * 滤波器最大阶数10阶
*/
#ifndef FILTER_MAX_ORDER
    #define FILTER_MAX_ORDER        10
#endif

/* Includes -----------------------------------------------------------------*/
/**
 * 标准库头文件
 */
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * 功能模块库头文件
*/
/* RTT打印工具 */
#include "middlewares/rtt_log/core_printf.h"

/* SVPWM模块 */
#include "middlewares/svpwm/svpwm.h"

/* Filter模块 */
#include "middlewares/filter/filter.h"

/* PID模块 */
#include "middlewares/pid/pid.h"

#ifdef __cplusplus
}
#endif

#endif  /* MIDDLEWARES_H */

/*****************************END OF FILE**************************************/
