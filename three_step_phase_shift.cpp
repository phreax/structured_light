#include "three_step_phase_shift.h"
#include <cstdio>

ThreeStepPhaseShift::ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3): 
    imgPhase1(imgPhase1),
    imgPhase2(imgPhase2),
    imgPhase3(imgPhase3),
    mask(0),
    process(0)

{

    width = imgPhase1->width;
    height = imgPhase1->height;
    if( width  != imgPhase2->width  ||
        width  != imgPhase3->width  ||
        height != imgPhase2->height ||  
        height != imgPhase3->height ) {
        throw "invalid arguments: input images must have same dimension!";
    }

    int size = width*height;
    imgColor        = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_8U,3);
    imgPhase1Gray = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_8U,1);
    imgWrappedPhase = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_32F,1);
    imgUnwrappedPhase = cvCreateImage(cvGetSize(imgPhase1),IPL_DEPTH_32F,1);

    mask            = new bool [size];
    process         = new bool [size];
    distance        = new float [size];
    depth           = new float [size];

        
    // initilize matrices
    noiseThreshold = 0.001;
    zscale = 130;
    zskew = 24;

    // init step width for color and single channel images
    step  = width; 

    cout << "step: " << step << "\nstep3: " << step3 << endl;
    cout << "width: " << width << "\nheight: " << height << endl;
    cout << "size " << size << endl;
    
    cvShowImage("phase1", imgPhase1);
    cvWaitKey(1000);

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
    //int step = imgPhase1->widthStep;
    
    uchar* ptrPhase1 = (uchar *)imgPhase1->imageData;
    uchar* ptrPhase2 = (uchar *)imgPhase2->imageData;
    uchar* ptrPhase3 = (uchar *)imgPhase3->imageData;
    uchar* ptrColor  = (uchar *)imgColor->imageData;
    uchar* ptrGray  = (uchar *)imgPhase1Gray->imageData;
    float* ptrWrappedPhase  = (float *)imgWrappedPhase->imageData;

    int ii,iic;
    
    float phi1;
    float phi2;
    float phi3;

    float phiSum;
    float phiRange;
    float phiMax;
    float phiMin;

    float gamma;
    float twoPi = M_PI * 2;
    int stepc = imgPhase1->widthStep;

    cout << "step " << step<< endl;

    for (int i = 0; i<height; i++) {
        for (int j = 0; j<width; j++) {

            ii = i*step+j;
            iic = i*stepc+j*3;

            // get intensity of each (rgb) phi image
            phi1 =luminance(ptrPhase1+iic);         
            phi2 =luminance(ptrPhase2+iic);         
            phi3 =luminance(ptrPhase3+iic);         

            ptrGray[i*imgPhase1Gray->widthStep + j] = phi1*255; 

            phiSum = phi1 + phi2 + phi3;
            
            phiMax = max_phase(phi1,phi2,phi3);
            phiMin = min_phase(phi1,phi2,phi3);
            
            phiRange = phiMax - phiMin;

            // compute phase quality, ignore background pixels,
            // i.e. where the phase range is too low
            gamma = phiRange / phiSum;
            mask[ii] = (gamma < noiseThreshold);
            process[ii] = !mask[ii];
            distance[ii] = phiRange;
            
            //compute phase: phi <- [-1,1]
           
            ptrWrappedPhase[ii] = (float)atan2(sqrt3 * (phi1 - phi3), 2*phi2 - phi1 - phi3) / twoPi;


            // user lightest pixel of all phase images as color
            if(phiMax==phi1) copy_channels(ptrColor+iic,ptrPhase1+iic);
            else if(phiMax==phi2) copy_channels(ptrColor+iic,ptrPhase2+iic);
            else if(phiMax==phi3) copy_channels(ptrColor+iic,ptrPhase3+iic);
        }
    }
}

void ThreeStepPhaseShift::makeDepth () {
    float* ptrWrappedPhase = (float *)imgWrappedPhase->imageData;
    
    for(int i = 0; i<height; i++) {
        float planephase = 0.5 - (i - (height/2))/zskew;
        for(int j=0; j<width; j++) {
            int ii = i*step+j;
            if(mask[ii]) {
                depth[ii] = (ptrWrappedPhase[ii] - planephase) * zscale;
            }
        }
    }
}


void ThreeStepPhaseShift::phaseUnwrap(int x, int y, float phi) {
    float* ptrUnwrappedPhase = (float *)imgUnwrappedPhase->imageData;
    
    if(process[y*step+x]) {

        float frac = phi -(int)phi;     // discontinue unwrapped phase
        float diff = ptrUnwrappedPhase[y*step+x] - frac;  // get phase difference
        if ( diff > 0.5 ) {
            diff--;
        }
        if ( diff < -0.5) {
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

    
    cvCopy(imgWrappedPhase, imgUnwrappedPhase);
    float* ptrUnwrappedPhase = (float *)imgUnwrappedPhase->imageData;


    procQueue.clear();
    UnwrapPath p  = UnwrapPath(startX, startY, ptrUnwrappedPhase[startY*step+startX]);
    procQueue.push_back(p);

    while(!procQueue.empty()) {
        UnwrapPath current = procQueue.front();
        procQueue.pop_front();
        int x = current.x;
        int y = current.y;
            
        if(process[y*step+x]) {
            ptrUnwrappedPhase[y*step+x] = current.phi; 
            process[y*width+x] = false;
        

            // follow path in each direction
            if (y > 0) {
                phaseUnwrap(x, y-1, current.phi );
            }
            if (y < height-1) {
                phaseUnwrap(x, y+1, current.phi);
            }
            if (x > 0) {
                phaseUnwrap(x-1, y, current.phi);
            }
            if (x < width - 1) {
                phaseUnwrap(x+1, y, current.phi);
            }
        }
    }
}



