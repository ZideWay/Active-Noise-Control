#ifndef __CONV_H
#define __CONV_H

#define PI 3.14159265358979323846
/* 定义采样点数 */
#define N 1000
/* 初级路径阶数 */
#define M 32
/* 滤波器阶数 */
#define L 16
/* 期望信号长度 */
#define DLEN (N + M -1)
/* 输入信号频率 */
#define F 300
/* 采样频率 */
#define FS 2400
/* 步长 */
#define STEPSIZE 0.01
#define MUS     0.0075
#define MUS_MAX 0.025
#define MUS_MIN 0.0075
#define VARIANCE 0.05
#define LUMA    0.99
void convolution(double* h, double* y);

#endif