#ifndef __CONV_H
#define __CONV_H

#define PI 3.14159265358979323846
/* ����������� */
#define N 1000
/* ����·������ */
#define M 32
/* �˲������� */
#define L 16
/* �����źų��� */
#define DLEN (N + M -1)
/* �����ź�Ƶ�� */
#define F 300
/* ����Ƶ�� */
#define FS 2400
/* ���� */
#define STEPSIZE 0.01
#define MUS     0.0075
#define MUS_MAX 0.025
#define MUS_MIN 0.0075
#define VARIANCE 0.05
#define LUMA    0.99
void convolution(double* h, double* y);

#endif