#include "three_step_phase_shift.h"

ThreeStepPhaseShift::ThreeStepPhaseShift(IplImage *imgPhase1, IplImage *imgPhase2, IplImage *imgPhase3) :
                     imgPhase1(imgPhase1), 
{}

void ThreeStepPhaseShift::init()
{
    /* not needed, we use rgb
    cvCvtColor(imgPhase1, imgPhase1, CV_RGB2GRAY);
    cvCvtColor(imgPhase2, imgPhase2, CV_RGB2GRAY);
    cvCvtColor(imgPhase3, imgPhase3, CV_RGB2GRAY);
    */
    // init process array with false
    // to indicate that no pixel was processed yet
    
}


void ThreeStepPhaseShift:phaseUnwrap() 
{
    int width = imgPhase1->width;
    int height = imgPhase1->height;
    int step = imgPhase1->widthStep;
    
    uchar* ptrPhase1 = (uchar *)imgPhase1->imageData;
    uchar* ptrPhase2 = (uchar *)imgPhase2->imageData;
    uchar* ptrPhase3 = (uchar *)imgPhase3->imageData;


}

void ThreeStepPhaseShift::phaseWrap() 
{
    // Some initializing and optimization( only the sqrt thing )
    float sqrt3 = sqrt(3);
    int width = imgPhase1->width;
    int height = imgPhase1->height;
    int step = imgPhase1->widthStep;
    
    uchar* ptrPhase1 = (uchar *)imgPhase1->imageData;
    uchar* ptrPhase2 = (uchar *)imgPhase2->imageData;
    uchar* ptrPhase3 = (uchar *)imgPhase3->imageData;
    uchar* ptrColor  = (uchar *)imgColor->imageData;
    uchar* ptrWrappedPhase  = (uchar *)imgWrappedPhase->imageData;

    int idx;
    
    int color1[3];
    int color2[3];
    int color3[3];

    float phase1;
    float phase2;
    float phase3;

    float phaseSum;
    float phaseRange;
    float phaseMax;
    float phaseMin;

    float gamma;
    float twoPi = M_PI * 2;

    for (int i = 0; i<height; i++) 
    {
        for (int j = 0; j<width; j++)
        {
            idx = i*step+j;


            // get rgb intensity by adding b+r+g channels and dividing by 3
            // this returns a value between 0 and 255,
            phase1 = luminance(ptrPhase1+idx);         
            phase2 = luminance(ptrPhase2+idx);         
            phase3 = luminance(ptrPhase3+idx);         
    
            phaseSum = phase1 + phase2 + phase3;
            
            phaseMax = max(phase1,phase2,phase3);
            phaseMin = min(phase1,phase2,phase3);
            
            phaseRange = phaseMax - phaseMin;

            // noise ignoration
            gamma = phaseRange / phaseSum;
            mask[i][j] = gamma < noiseThreshold;
            process[i][j] != mask[i][j];

            // phase calculation
            phase[idx] = atan2(sqrt3 * (phase1 - phase3), 2*phase2 - phase1 - phase3) / twoPi;

            // get color channels from imgPhase1-3

            // put lightest color in colors-array
            switch(phaseMax) {
                case phase1: copy_channels(ptrColor+idx,ptrPhase1+idx);
                case phase2: copy_channels(ptrColor+idx,ptrPhase2+idx);
                case phase3: copy_channels(ptrColor+idx,ptrPhase3+idx);
                default: break;
            }
        }
    }
}
