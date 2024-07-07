#ifndef ADAPTIVEFILTER_H_
#define ADAPTIVEFILTER_H_
#include <debug_config.h>
#include <conv.h>
typedef struct {
	const double StepSize; /* adaptive filter step size */
    const unsigned int Length; /* length of filter */
    double Error;
	double *pBuffer; /* pointer to input buffer */
    double *pRefFilterBuf;
	double *pWeights; /* pointer to adaptive filter weights */
    double *pYBuffer;   /* filter output */
} AfData;

typedef struct {
	const double StepSize; /* adaptive filter step size */
    const unsigned int Length; /* length of filter */
    double Errorf;
	double *pvBuffer; /* pointer to input buffer */
	double *pEsti; /* pointer to adaptive filter weights */
    double *pPe;
    double *pPf;
    double Ru;
} OMData;
void AdaptiveFilterRun(double input, double desired,double vs, AfData *pData, OMData *pOMData);


#endif /* ADAPTIVEFILTER_H_ */