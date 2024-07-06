#ifndef __RANDN_H_
#define __RANDN_H_
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define VARIANCE 0.05

double generate_gaussian_noise(double variance);

#endif