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
    process = {0};
    noiseThreshold = 0.1;
}


void ThreeStepPhaseShift:phaseUnwrap() 
{
    int width = imgPhase1->width;
    int height = imgPhase1->height;
    int step = imgPhase1->widthStep;
    
    uchar* data1phase = (uchar *)imgPhase1->imageData;
    uchar* data2phase = (uchar *)imgPhase2->imageData;
    uchar* data3phase = (uchar *)imgPhase3->imageData;


}

void ThreeStepPhaseShift::phaseWrap() 
{
    // Some initializing and optimization( only the sqrt thing )
    float sqrt3 = sqrt(3);
    int width = imgPhase1->width;
    int height = imgPhase1->height;
    int step = imgPhase1->widthStep;
    
    uchar* data1phase = (uchar *)imgPhase1->imageData;
    uchar* data2phase = (uchar *)imgPhase2->imageData;
    uchar* data3phase = (uchar *)imgPhase3->imageData;

    int imageIndex;
    
    int color1[3];
    int color2[3];
    int color3[3];
    //int colors[width][height][3]; // defined in ThreeStepPhaseShift.h

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
            imageIndex = i*step+j;


            // get rgb intensity by adding b+r+g channels and dividing by 3
            // this returns a value between 0 and 255,
            // then divide by 255 to get phase
            phase1 = ((data1phase[imageIndex]+data1phase[imageIndex+1]+data1phase[imageIndex+2]) / 3) / 255.0;
            phase2 = ((data2phase[imageIndex]+data2phase[imageIndex+1]+data2phase[imageIndex+2]) / 3) / 255.0;
            phase3 = ((data3phase[imageIndex]+data3phase[imageIndex+1]+data3phase[imageIndex+2]) / 3) / 255.0;
    
            phaseSum = phase1 + phase2 + phase3;
            
            // get phaseMax and lightest color
            if(phase1 > phase2) {
                phaseMax = phase1;
                if (phaseMax < phase3) {
                    phaseMax = phase3;
                }
            }
            else {
                phaseMax = phase2;
                if(phaseMax < phase3) {
                    phaseMax = phase3;
                }
            }

            // get phaseMin
            if(phase1 < phase2) {
                phaseMin = phase1;
                if(phaseMin > phase3) {
                    phaseMin = phase3;
                }
            }
            else {
                phaseMin = phase2;
                if(phaseMin > phase3) {
                    phaseMin = phase3;
                }
            }

            phaseRange = phaseMax - phaseMin;

            // noise ignoration
            gamma = phaseRange / phaseSum;
            mask[i][j] = gamma < noiseThreshold;
            process[i][j] != mask[i][j];

            // phase calculation
            phase[i][j] = atan2(sqrt3 * (phase1 - phase3), 2*phase2 - phase1 - phase3) / twoPi;

            // get color channels from imgPhase1-3
            color1[0] = data1phase[imageIndex];
            color1[1] = data1phase[imageIndex+1];
            color1[2] = data1phase[imageIndex+2];
            color2[0] = data2phase[imageIndex];
            color2[1] = data2phase[imageIndex+1];
            color2[2] = data2phase[imageIndex+2];
            color3[0] = data3phase[imageIndex];
            color3[1] = data3phase[imageIndex+1];
            color3[2] = data3phase[imageIndex+2];
            // put lightest color in colors-array
            switch(phaseMax) {
                case phase1: colors[i][j][0] = color1[0]; colors[i][j][1] = color1[1]; colors[i][j][2] = color1[2]; break;
                case phase2: colors[i][j][0] = color2[0]; colors[i][j][1] = color2[1]; colors[i][j][2] = color2[2]; break;
                case phase3: colors[i][j][0] = color3[0]; colors[i][j][1] = color3[1]; colors[i][j][2] = color3[2]; break;
                default: break;
            }
        }
    }
}
