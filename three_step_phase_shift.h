#include <iostream>
#include <cv.h>
#include <highgui.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef THREESTEPPHASESHIFT_H
#define THREESTEPPHASESHIFT_H

class ThreeStepPhaseShift {

public:
    ThreeStepPhaseShift();

private:
    IplImage* phase1Image;
    IplImage* phase2Image;
    IplImage* phase2Image;
    int inputwidth, inputheight;
    float[][] phase;
    bool[][] mask;
    bool[][] process;
    float noiseThreshold;
}

#endif
