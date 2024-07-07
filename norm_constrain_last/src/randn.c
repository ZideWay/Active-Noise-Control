
#include "randn.h"

double generate_gaussian_noise(double variance) {
        static int i = 1;
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;

        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
        if((i % 2) == 0)
        {
            return sqrt(variance) * z0;
            i++; 
        }
        else
        {
            return  sqrt(variance) * z1;
            i++;   
        }

}