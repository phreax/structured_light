#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <deque>

#ifndef THREESTEPPHASESHIFT_H
#define THREESTEPPHASESHIFT_H

using namespace std;

struct UnwrapPath {
    int x;
    int y;
    int phi; // last phase 
    int r;   // phase distance

    UnwrapPath(int x, int y, int phi):
        x(x),y(y),phi(phi)
    {}

    //bool operator<(const UnwrapPath & p) {return r<p.r;}

};

class ThreeStepPhaseShift {

public:

    ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3
        );

    ~ThreeStepPhaseShift();
 
    void phaseWrap();
    void phaseUnwrap();
    void setZscale(float newZscale);
    void setZskew(float newZskew);
    void setNoiseThreshold(float newThreshold);
    float getZscale();
    float getZskew();
    float getNoiseThreshold();
    float* getDepth();
protected:

    // unwrap at x,y
    void phaseUnwrap(int x, int y, float phi );
    void makeDepth ();

    // inline helper functions
    uchar max_phase(uchar v1, uchar v2, uchar v3) {
        uchar max;
        max = v1>v2 ? v1 : v2;
        max = max>v3 ? max : v3;
        return max;
    }

    uchar min_phase(uchar v1, uchar v2, uchar v3) {
        uchar max = v1<v2 ? v1 : v2;
        max = max<v2 ? max : v3;
        return max;
    }

    /* use mean as luminance of an rgb triple */
    float luminance(uchar *color) {return (color[0]+color[1]+color[2])/(3.f*255);}

    void copy_channels(uchar *dest, uchar *src) {
        for(int i=0;i<3;i++)
            *(dest++) = *(src++);
    } 

private:

    IplImage* imgPhase1;
    IplImage* imgPhase2;
    IplImage* imgPhase3;
    IplImage* imgColor;  // reconstructed color image 
    IplImage* imgWrappedPhase;

    // some helper matrices to track phase quality and
    // processing state (each from the same dimension as the input image)
    bool *mask;
    bool *process;
    float *distance;
    float *depth;
    float noiseThreshold;
    float zscale;
    float zskew;

    int width;
    int height;
    int step;   // for single channel images
    int step3; // for 3 channel images

    deque<UnwrapPath> procQueue;
};

#endif
