RTT打印小工具（by：睿智的嵌入式（WiseMCU））

**使用说明：该工具是基于RTX5操作系统使用的，如果不用RTX5操作系统需要修改或删除互斥量和系统时钟相关内容**

使用前需要先配置代码

```c
/**
 * 调试信息输出总开关
 */
#define DEBUG_ENABLE                    (1)

/**
 * 调试信息输出等级设置
 *
 * @note 高于此等级的信息不会输出，开发后期可将等级设置为DEBUG_LEVEL_WARNING
 */
#define DEBUG_LEVEL_ERROR               0
#define DEBUG_LEVEL_WARNING             1
#define DEBUG_LEVEL_INFO                2
#define DEBUG_LEVEL_DEBUG               3

#define DEBUG_LEVEL                     DEBUG_LEVEL_DEBUG

/**
 * 调试信息时间格式化设置
 *
 * @note 时间格式化将消耗较多算力，若资源紧张可直接注释此宏定义
 */
#define DEBUG_TIME_FORMAT_ENABLE        (1)
```

```c
/**
 * 使用示例：
 * ① 初始化：segger_rtt_init();
 * ② 调用宏输出不同等级的信息：
 * DEBUG   ("This is a debug information...");
 * INFO    ("This is a information...");
 * WARNING ("This is a warning...");
 * ERROR   ("This is a error...");
 * char test[2] = {1, 2};
 * CMD_LOG(test, sizeof(test), "This is a cmd log");
 * ③ 终端显示：
 * [00:00:00.00] This is a debug information...  (白色字体)
 * [00:00:00.00] This is a information...        (绿色字体)
 * [00:00:00.00] This is a warning...            (黄色字体)
 * [00:00:00.00] This is a error...              (红色字体)
 * [00:00:00.00] This is a cmd log 0x01 0x02...  (白色字体)
 */
```

