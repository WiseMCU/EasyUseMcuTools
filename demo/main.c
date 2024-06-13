#include "filter.h"
#include "stdio.h"

/* 读取data.csv文件数据，数据结构为%f %f */
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

int main(void)
{
    /* 读取数据 */ 
    const int len = 10000;
    float input_data1[len], input_data2[len], matlab_output1[len], matlab_output2[len];
    
    readData("data.csv", input_data1, input_data2, len);
    readData("matlab.csv", matlab_output1, matlab_output2, len);

    /* 创建滤波器对象 */ 
    Filter filter = {
        .order  = 2,
        .a      = {1.0f,-1.955578240315035f,  0.956543676511203f},
        .b      = {0.000241359049041f, 0.000482718098083f, 0.000241359049041f},
        .zi     = {0.999758640950951f, -0.956302317462154f},
    };

    /* 计算滤波 */
    float sout1[len], sout2[len];

    filtfilt_calc(&filter, input_data1, sout1, len);
    filtfilt_calc(&filter, input_data2, sout2, len);

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
        fprintf(fp, "%f,%f,%f,%f,%f,%f\n", input_data1[i], input_data2[i], 
            matlab_output1[i], matlab_output2[i], sout1[i], sout2[i]);
    }

    return 0;
}