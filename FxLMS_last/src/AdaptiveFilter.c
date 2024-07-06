#include "AdaptiveFilter.h"
#include "AdaptiveFilter_test.h"
#include <math.h>
#include <stdio.h>

static double sw[L] = {
        0.05, -0.4, 0.87, 0.1, -0.38, 0.01, 0.1, 0, 0, 0, 0, 0, 0, 0.04, 0, 0.6
    };


static int cnt = 0;
static void AdaptWeights(AfData *pData, OMData *pOMData)
{
    double StepSize = 0;
    int i = 0;
    StepSize = pData->StepSize;
    
    for(i = 0; i < pData->Length; i++)
    {
        pData->pWeights[i] +=  StepSize*pData->pRefFilterBuf[i]*pData->Error; 
    }    
    
}
static void OnlineModeling(OMData *pOMData)
{
    double StepSize;
    int i;
    double norm_s = 0.0;
    StepSize = pOMData->Ru * MUS_MIN + (1 - pOMData->Ru) * MUS_MAX;
    for(i = 0; i < pOMData->Length; i++)
    {
        pOMData->pEsti[i] += StepSize*pOMData->pvBuffer[i]*pOMData->Errorf;
    }
    

     
}

static void ControlFilter(double intput, double auilNoise,AfData *pData, OMData *pOMData)
{
    int i;
    double output = 0;
    double dTmp = pOMData->Ru;

    for (i = pData->Length- 1; i > 0; i--) {
        pData->pBuffer[i] = pData->pBuffer[i - 1];
    }
    pData->pBuffer[0] = intput; 
    // printf("pData->pBuffer[0] = %.15f\r\n", pData->pBuffer[0]);    
    for(i = 0; i < pData->Length; i++)
    {
        output += pData->pWeights[i] * pData->pBuffer[i];    		
    }

    for (i = pData->Length- 1; i > 0; i--) {
        pData->pYBuffer[i] = pData->pYBuffer[i - 1];
    }
    pData->pYBuffer[0] = output;
    // printf("filter output : %.15f\r\n", pData->pYBuffer[0]);

    // printf("dTmp = %.15f\r\n", dTmp);
    // printf("auil noise : %.15f\r\n", pOMData->pvBuffer[0]);

}

/* 得到滤波参考信号 */
static void GetFilterReferSignal(AfData *pData, OMData *pOMData)
{
    int i;
    for (i = pData->Length- 1; i > 0; i--) {
        pData->pRefFilterBuf[i] = pData->pRefFilterBuf[i - 1];
    }
	pData->pRefFilterBuf[0] = 0.0;
    for(i = 0; i < L; i++) {
        pData->pRefFilterBuf[0] += pData->pBuffer[i] * sw[i]; 
	}
	// printf("refenence signal : %.15f\r\n",  pData->pRefFilterBuf[0]); 
}

void AdaptiveFilterRun(double input, double desired,double vs, AfData *pData, OMData *pOMData)
{
    int i;
    unsigned int index;
    double tmp = 0;
    double vtmp = 0;
    index = cnt % 16;
   

    ControlFilter(input, vs, pData, pOMData);      /* filter the input */
    for(i = 0; i < L; i++)
    {
        tmp += pData->pYBuffer[i]*sw[i];
    }

    pData->Error = desired - tmp ;    /* update the error */
   
    //pOMData->Ru = pOMData->pPf[index] / pOMData->pPe[index];   
    // OnlineModeling(pOMData);
    GetFilterReferSignal(pData, pOMData);		/* 得到滤波参考信号 */
    AdaptWeights(pData, pOMData);                /* update adaptive filter weights */ 
    cnt++; 
    /*
    if(cnt == 10000) 
    {
        for(i = 0; i<L; i++)
        {
            sw[i] = 0.5*sw[i];
        }        
    }
    */
   
   
                
}










































