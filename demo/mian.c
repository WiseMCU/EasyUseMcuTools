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