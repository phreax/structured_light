#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#ifndef THREESTEPPHASESHIFT_H
#define THREESTEPPHASESHIFT_H

class ThreeStepPhaseShift {

public:
    ThreeStepPhaseShift(IplImage *imgPhase1, IplImage *imgPhase2, IplImage *imgPhase3) :
                     imgPhase1(imgPhase1),imgPhase2(imgPhase2),imgPhase3(imgPhase3) 
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
        mask            = cvCreateMat(height,width,CV_8U);
        process         = cvCreateMat(height,width,CV_8U);

        noiseThreshold = 0.1;
    }

    void phaseWrap();
    void phaseUnWrap();

private:

    inline uchar max(uchar v1, uchar v2, uchar v3) {
        uchar max = v1>v2 ? v1: v2;
        max = max>v3 ? max : v3;
        return max;
    }

    inline uchar min(uchar v1, uchar v2, uchar v3) {
        uchar max = v1<v2 ? v1 : v2;
        max = max<v2 ? max : v3;
        return max;
    }

    /* use mean as luminance of an rgb triple */
    inline uchar luminance(uchar *color) {
        return (color[0]+color[1]+color[2])/3;
    }

    inline void copy_channels(uchar *dest, uchar *src) {
        for(int i=0;i<3;i++)
            *(dest++) = *(src++);
    } 

    IplImage* imgPhase1;
    IplImage* imgPhase2;
    IplImage* imgPhase3;
    IplImage* imgColor;  // reconstructed color image 
    IplImage* imgWrappedPhase;

    // some helper matrices to track phase quality and
    // processing state
    CvMat *mask;
    CvMat *process;

    float noiseThreshold;
}

#endif
