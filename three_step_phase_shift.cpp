#include "three_step_phase_shift.h"

ThreeStepPhaseShift::ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3): 
    imgPhase1(imgPhase1),
    imgPhase2(imgPhase2),
    imgPhase3(imgPhase3)
{

    width = imgPhase1->width;
    height = imgPhase1->height;
    if( width  != imgPhase2->width  ||
        width  != imgPhase3->width  ||
        height != imgPhase2->height ||  
        height != imgPhase3->height ) {
        throw "invalid arguments: input images must have same dimension!";
    }

    // initilize matrices
    int size = width*height;
    imgColor        = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_8U,3);
    imgWrappedPhase = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_32F,1);
    mask            = new bool[size];
    process         = new bool[size];
    distance        = new float[size];
    depth           = new float[size];
    noiseThreshold = 0.1;
    zscale = 130;
    zskew = 24;

    // init step width for color and single channel images
    step  = width ;
    step3 = imgPhase1->widthStep;
}

// dtor
ThreeStepPhaseShift::~ThreeStepPhaseShift() {

    cvReleaseImage(&imgPhase1);
    cvReleaseImage(&imgPhase2);
    cvReleaseImage(&imgPhase3);
    cvReleaseImage(&imgColor);
    cvReleaseImage(&imgWrappedPhase);
    delete[] mask;
    delete[] process;
    delete[] distance;
    delete[] depth;
}

void ThreeStepPhaseShift::phaseWrap() 
{
    // Some initializing and optimization( only the sqrt thing )
    float sqrt3 = sqrt(3);
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
            phi1 = luminance(ptrPhase1+(i*step3+j));         
            phi2 = luminance(ptrPhase2+(i*step3+j));         
            phi3 = luminance(ptrPhase3+(i*step3+j));         
    
            phiSum = phi1 + phi2 + phi3;
            
            phiMax = max_phase(phi1,phi2,phi3);
            phiMin = min_phase(phi1,phi2,phi3);
            
            phiRange = phiMax - phiMin;

            // compute phase quality, ignore background pixels,
            // i.e. where the phase range is too low
            gamma = phiRange / phiSum;
            mask[idx] = gamma < noiseThreshold;
            process[idx] = !mask[idx];
            distance[idx] = phiRange;

            // compute phase: phi <- [-1,1]
            ptrWrappedPhase[idx] = atan2(sqrt3 * (phi1 - phi3), 2*phi2 - phi1 - phi3) / twoPi;

            // user lightest pixel of all phase images as color
            if(phiMax==phi1) copy_channels(ptrColor+idx,ptrPhase1+idx);
            else if(phiMax==phi2) copy_channels(ptrColor+idx,ptrPhase2+idx);
            else if(phiMax==phi3) copy_channels(ptrColor+idx,ptrPhase3+idx);
        }
    }
}

void ThreeStepPhaseShift::makeDepth () {
    uchar* ptrWrappedPhase = (uchar *)imgWrappedPhase->imageData;
        
    for(int j=0; j<width; j++) {
        float planephase = 0.5 - (j - (height/2))/zskew;
        for(int i = 0; i<height; i++) {
            int idx = i*step+j;
            if(mask[idx]) {
                depth[idx] = (ptrWrappedPhase[idx] - planephase) * zscale;
            }
        }
    }
}


void ThreeStepPhaseShift::phaseUnwrap(int x, int y, float phi) {
    uchar* ptrWrappedPhase = (uchar *)imgWrappedPhase->imageData;
    
    if(process[x*width+y]) {

        float frac = phi -(int)phi;     // discontinue unwrapped phase
        float diff = ptrWrappedPhase[x*step+y] - frac;  // get phase difference
        if ( diff > .5 ) {
            diff--;
        }
        if ( diff < .5) {
            diff++;
        }
        procQueue.push_back(UnwrapPath(x, y, phi+diff));
    }
}

void ThreeStepPhaseShift::phaseUnwrap() 
{
    int startX = width/2;
    int startY = height/2;
    
    uchar* ptrPhase1 = (uchar *)imgPhase1->imageData;
    uchar* ptrPhase2 = (uchar *)imgPhase2->imageData;
    uchar* ptrPhase3 = (uchar *)imgPhase3->imageData;
    uchar* ptrWrappedPhase = (uchar *)imgWrappedPhase->imageData;

    procQueue.clear();
    UnwrapPath p  = UnwrapPath(startX, startY, ptrWrappedPhase[startX*step+startY]);
    procQueue.push_back(p);

    while(!procQueue.empty()) {
        UnwrapPath current = procQueue.front();
        procQueue.pop_front();
        int x = current.x;
        int y = current.y;
        if(process[x*step+y]) {
            ptrWrappedPhase[x*step+y] = current.phi; 
            process[x*width+y] = false;
            float phi = ptrWrappedPhase[x*step+y];

            // follow path in each direction
            if (y > 0) {
                phaseUnwrap(x, y-1, phi );
            }
            if (y < height-1) {
                phaseUnwrap(x, y+1, phi);
            }
            if (x > 0) {
                phaseUnwrap(x-1, y, phi);
            }
            if (x < width - 1) {
                phaseUnwrap(x+1, y, phi);
            }
        }
    }
}

void ThreeStepPhaseShift::setZscale(float newZscale) 
{
    zscale = newZscale;
}

void ThreeStepPhaseShift::setZskew(float newZskew)
{
    zskew = newZskew;
}

void ThreeStepPhaseShift::setNoiseThreshold(float newThreshold) 
{
    noiseThreshold = newThreshold;
}

float ThreeStepPhaseShift::getZscale() {
    return zscale;
}

float ThreeStepPhaseShift::getZskew() {
    return zskew;
}

float ThreeStepPhaseShift::getNoiseThreshold() {
    return noiseThreshold;
}

float* ThreeStepPhaseShift::getDepth() {
    return depth;
}

