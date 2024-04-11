RTT打印工具（by：睿智的嵌入式（WiseMCU））

使用前需要先配置代码

```c
/**
 * 调试信息输出总开关
 */
#ifndef DEBUG_ENABLE
    /* 默认开启打印 */
    #define DEBUG_ENABLE                1
#endif

/**
 * 调试信息时间格式化设置
 *
 * @note 时间格式化将消耗较多算力，若资源紧张可直接注释此宏定义
 */
#ifndef DEBUG_TIME_FORMAT_ENABLE
    /* 默认开启时间格式化 */
    #define DEBUG_TIME_FORMAT_ENABLE    1
#endif

/**
 * 屏蔽打印等级配置
 */
#ifndef DEBUG_LEVEL
    /* 默认所有等级都打印 */
    #define DEBUG_LEVEL                 DEBUG_LEVEL_LOG
#endif

/**
 * 操作系统配置定义
 */
#ifndef RTT_TOOL_THREAD
    /* 默认不使用操作系统 */
    #define RTT_TOOL_THREAD             RTT_TOOL_NONE
#endif
```

### 使用示例：

1.  初始化：segger_rtt_init();

2.  调用宏输出不同等级的信息：

    ```c
    DEBUG   ("This is a debug information...");
    INFO    ("This is a information...");
    WARNING ("This is a warning...");
    ERROR   ("This is a error...");
    char test[2] = {1, 2};
    CMD_LOG(test, sizeof(test), "This is a cmd log")
    ```
    
3.  终端显示：

    ```c
    [00:00:00.00] This is a debug information...  (白色字体)
    [00:00:00.00] This is a information...        (绿色字体)
    [00:00:00.00] This is a warning...            (黄色字体)
    [00:00:00.00] This is a error...              (红色字体)
    [00:00:00.00] This is a cmd log 0x01 0x02     (白色字体)
    ```

    
