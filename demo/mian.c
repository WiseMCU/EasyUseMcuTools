#include <stdio.h>

#include "pid.h"

/* 读取csv文件数据，数据结构为%f %f */
void readData(const char *filename, float *data1, float *data2, int len)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Failed to open file %s\n", filename);
        return;
    }

    for (int i = 0; i < len; i++)
    {
        fscanf(fp, "%f,%f", &data1[i], &data2[i]);
    }

    fclose(fp);
}

#define Kp      1.6817
#define Ki      1.1539
#define Kd      0.18405

int main()
{
    /* 读取数据 */ 
    const int len = 10000;
    float matlab_pid_in[len], matlab_pid_out[len], pid_out[len];
    readData("matlab.csv", matlab_pid_in, matlab_pid_out, len);

    /* 按照Matlab的PID系数创建PID并计算PID */
    pid_t pid_handle;
    pid_init(&pid_handle, Kp, Ki, Kd, 0, 0xFFFF);
    for(int i = 0; i < len; i++)
    {
        pid_out[i] = pid_calc(&pid_handle, matlab_pid_out[i]);
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
        fprintf(fp, "%f,%f\n", pid_out[i], matlab_pid_in[i]);
    }

    fclose(fp);

    return 0;
}