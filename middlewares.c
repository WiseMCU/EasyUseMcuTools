/**
 * @file      middlewares.c
 * @brief     集合库配置文件
 * @version   1.0
 * @date      2024-04-11
 * @author    WiseMCU
 */

/* Includes -----------------------------------------------------------------*/
#include "middlewares.h"

/* Tools --------------------------------------------------------------------*/
/* RTT打印工具 */
#include "middlewares/rtt_log/SEGGER_RTT_printf.c"
#include "middlewares/rtt_log/SEGGER_RTT.c"
#include "middlewares/rtt_log/core_printf.c"

/* SVPWM模块 */
#include "middlewares/svpwm/svpwm.c"

/********************************* END OF FILE ********************************/
