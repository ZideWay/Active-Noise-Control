#include <stdio.h>
#include <math.h>
#include <debug_config.h>
#include "conv.h"

// DEBUG
#define ITM_PORT8(n)         (*(volatile unsigned char *)(0xe0000000 + 4*(n)))
#define ITM_PORT16(n)        (*(volatile unsigned short *)(0xe0000000 + 4*(n)))
#define ITM_PORT32(n)        (*(volatile unsigned long *)(0xe0000000 + 4*(n)))
#define DEMCR                (*(volatile unsigned long *)(0xE000EDFC))
#define TRCENA               0X01000000 


void convolution(double* h, double* y) {
   
    // 初始化输出y(n)
    for (int i = 0; i < DLEN; i++) {
        y[i] = 0;
    }

    // 卷积运算
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            y[i + j] += sin(2*PI*F*(i + 1)/FS) * h[j];			
        }
    }
}



 








/* 调试DEBUG */
int fputc(int ch, FILE *f)
{
    if(DEMCR & TRCENA)
    {
        while(ITM_PORT32(0) == 0);                                                                                                                                                                                                                                                                                      
        ITM_PORT8(0) = ch;
    }
    return ch;
}

