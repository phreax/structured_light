#include "three_step_phase_shift.h"
#include <queue>

ThreeStepPhaseShift::ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3)
    : imgPhase1(imgPhase1),imgPhase2(imgPhase2),imgPhase3(imgPhase3) 
{

    int width = imgPhase1->width;
    int height = imgPhase1->height;
    if( width  != imgPhase2->width  ||
        width  != imgPhase3->width  ||
        height != imgPhase2->height ||  
        height != imgPhase3->height ) {
        throw "invalid arguments: input images must have same dimension!";
    }

    // initilize matrices
    imgColor        = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_8U,3);
    imgWrappedPhase = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_32F,1);
    int widthstep = imgWrappedPhase->widthstep;
    mask            = new bool[width*height];
    process         = new bool[width*height];
    distance        = new float[width*height];
    depth           = new float[width*height];
    noiseThreshold = 0.1;
    zscale = 130;
    zskew = 24;
}

// dtor
ThreeStepPhaseShift::~ThreeStepPhaseShift() {

    cvReleaseImage(&imgPhase1);
    cvReleaseImage(&imgPhase2);
    cvReleaseImage(&imgPhase3);
    cvReleaseImage(&imgColor);
    cvReleaseImage(&imgWrappedPhase);
    delete mask;
    delete process;
    delete distance;
    delete depth;
}

void ThreeStepPhaseShift::makeDepth () {
    uchar* ptrWrappedPhase = (uchar *)imgWrappedPhase->imageData;
    for(int i = 0; i<height; i++) {
        float planephase = 0.5 - (y - (height/2))/zskew;
        for(int j=0; j<width; j++) {
            if(mask[i*widthstep+j] {
                depth[i*widthstep+j] = (ptrWrappedPhase[i*widthstep+j] - planephase) * zscale;
            }
        }
    }
}


void ThreeStepPhaseShift::phaseUnwrap(int x, int y, float d, float r, int *toProcess, int * ptrWrappedPhase, widthstep) {
    if(process[x*width+y]) {
        float diff = ptrWrappedPhase[x*widthstep+y] - (r - (int) r);
        if ( diff > .5 ) {
            diff--;
        }
        if ( diff < .5) {
            diff++;
        }
        toProcess.push(new WrappedPixel(x, y, d+distance[x*widthstep+y], r+diff);
    }
}

void ThreeStepPhaseShift::phaseUnwrap() 
{
    int width = imgPhase1->width;
    int height = imgPhase1->height;
    int step = imgPhase1->widthStep;
    
    uchar* ptrPhase1 = (uchar *)imgPhase1->imageData;
    uchar* ptrPhase2 = (uchar *)imgPhase2->imageData;
    uchar* ptrPhase3 = (uchar *)imgPhase3->imageData;
    uchar* ptrWrappedPhase = (uchar *)imgWrappedPhase->imageData;

    step = imgPhaseWrappedPhase->widthstep;

    queue toProcess<WrappedPixel>;
    WrappedPixel p = new WrappedPixel(width/2, height/2, 0, ptrWrappedPhase[(width/2)*widthstep+(height/2)]);
    toProcess.push(p);

    while(!toProcess.empt()) {
        WrappedPixel current = toProcess.pop();
        int x = current.x;
        int y = current.y;
        if(process[x*widthstep+y]) {
            ptrWrappedPhase[x*widthstep+y] = current.phi; 
            process[x*width+y] = false;
            float d = current.r;
            float r = ptrWrappedPhase[x*widthstep+y];
            if (y > 0) {
                phaseUnwrap(x, y-1, d, r, &toProcess, &ptrWrappedPhase, widthstep);
            }
            if (y < height-1) {
                phaseUnwrap(x, y+1, d, r, &toProcess, &ptrWrappedPhase, widthstep);
            }
            if (x > 0) {
                phaseUnwrap(x-1, y, d, r, &toProcess, &ptrWrappedPhase, widthstep);
            }
            if (x < width - 1) {
                phaseUnwrap(x+1, y, d, r, &toProcess, &ptrWrappedPhase, widthstep);
            }
        }
    }
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
    
    float phi1;
    float phi2;
    float phi3;

    float phiSum;
    float phiRange;
    float phiMax;
    float phiMin;

    float gamma;
    float twoPi = M_PI * 2;

    for (int i = 0; i<height; i++) {
        for (int j = 0; j<width; j++) {

            idx = i*step+j;

            // get intensity of each (rgb) phi image
            phi1 = luminance(ptrPhase1+idx);         
            phi2 = luminance(ptrPhase2+idx);         
            phi3 = luminance(ptrPhase3+idx);         
    
            phiSum = phi1 + phi2 + phi3;
            
            phiMax = max_phase(phi1,phi2,phi3);
            phiMin = min_phase(phi1,phi2,phi3);
            
            phiRange = phiMax - phiMin;

            // noise ignoration
            gamma = phiRange / phiSum;
            mask[idx] = gamma < noiseThreshold;
            process[idx] != mask[idx];
            distance[idx] = phiRange;

            // phi calculation
            ptrWrappedPhase[idx] = atan2(sqrt3 * (phi1 - phi3), 2*phi2 - phi1 - phi3) / twoPi;

            // put lightest color in colors-array
            if(phiMax==phi1) copy_channels(ptrColor+idx,ptrPhase1+idx);
            else if(phiMax==phi2) copy_channels(ptrColor+idx,ptrPhase2+idx);
            else if(phiMax==phi3) copy_channels(ptrColor+idx,ptrPhase3+idx);
        }
    }
}

