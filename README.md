# PID模块（by：睿智的嵌入式（WiseMCU））
PID控制模块无须额外配置直接使用

### DEMO使用说明

1.  #### 获取PID控制器参数

    打开Matlab的Simulink创建一个PID系统仿真
    ![](img/image-20240622172542334.png)
    训练PID参数达到合适滤波效果
    ![image-20240622172602919](img/image-20240622172602919.png)
    假设如图已经达到预期效果获取PID参数、滤波器系数、计算周期，同时**将Simulink的PID输入输出导出到Matlab.csv**
    ![image-20240622172628132](img/image-20240622172628132.png)

2.  #### 编写测试demo程序

    1.  准备好matlab.csv数据，编写C语言使用Matlab的PID系数创建PID，并按照Matlab的PID输入计算一遍比较输出结果

        ```c
        #include <stdio.h>
        #include "pid.h"
        
        /* 读取data.csv文件数据*/
        void readData(const char *filename, float *data1, float *data2, float *data3, int len)
        {
            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
            {
                printf("Failed to open file %s\n", filename);
                return;
            }
        
            for (int i = 0; i < len; i++)
            {
                fscanf(fp, "%f,%f,%f", &data1[i], &data2[i], &data3[i]);
            }
        
            fclose(fp);
        }
        
        #define KP          0.199666833293656f
        #define KI          39.9333666587313f
        #define KD          0.0f
        #define FILTER_PARA 100.0f
        #define CYCLE_TIME  0.01f
        
        int main(void)
        {
            /* 读取数据 */ 
            const int len = 10000;
            float pid_out[len], matlab_pid_input[len], matlab_pid_output[len], matlab_sfun_output[len];
            
            readData("matlab.csv", matlab_pid_input, matlab_pid_output, matlab_sfun_output, len);
        
            
            /* 计算PID */
            pid_t pid_handle;
            pid_init(&pid_handle, KP, KI, KD, FILTER_PARA, CYCLE_TIME);
            for(int i = 0; i < len; i++)
            {
                pid_out[i] = pid_calc(&pid_handle, matlab_pid_input[i], matlab_sfun_output[i]);
            }
        
            /* 保存数据到output.csv */
            FILE *fp = fopen("output.csv", "w");
            if (fp == NULL)
            {
                printf("Failed to open file output.csv\n");
                return 0;
            }
        
            /* 将所有数据写入 */
            for (int i = 0; i < len; i++)
            {
                fprintf(fp, "%f,%f\n", pid_out[i], matlab_pid_output[i]);
            }
        
            fclose(fp);
        
            return 0;
        }
        ```
    
3.  #### 运行demo，可视化结果

    [^Matlab的PID计算结果-C语言PID计算结果 ]: 结果完全一致

    ![image-20240622173219607](img/image-20240622173219607.png)