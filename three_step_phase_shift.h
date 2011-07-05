#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <deque>
#include <queue>

#ifndef THREESTEPPHASESHIFT_H
#define THREESTEPPHASESHIFT_H

using namespace std;

struct UnwrapPath {
    int x;
    int y;
    float phi;    // last phase 
    float q;      // phase quality 

    UnwrapPath(int x, int y, float phi):
        x(x),y(y),phi(phi)
    {}

    UnwrapPath(int x, int y, float phi, float q):
        x(x),y(y),phi(phi), q(q)
    {}
    
    bool operator<(const UnwrapPath & path) const {return q<path.q;}

};

class ThreeStepPhaseShift {

public:

    ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3
        );

    ~ThreeStepPhaseShift();
 
    void phaseDecode();
    void phaseUnwrap();
    void computeDepth ();
    
    void compute() {
        phaseDecode();
        phaseUnwrap();
        computeDepth();
    }

    void  setZscale(float _zscale) { zscale = _zscale; }
    void  setZskew(float _zskew) { zskew = _zskew; }
    void  setNoiseThreshold(float _threshold) { noiseThreshold = _threshold;}
    float getZscale() { return zscale; }
    float getZskew() { return zskew; }
    float getNoiseThreshold() { return noiseThreshold; }
    float* getDepth() { return depth; }
    bool* getMask() { return mask; }

    IplImage *getWrappedPhase()  { return imgWrappedPhase; };
    IplImage *getUnwrappedPhase()  { return imgUnwrappedPhase; };
    IplImage *getColorImage()  { return imgColor; };

    IplImage* imgPhase1Gray;

protected:

    // unwrap at x,y
    void phaseUnwrap(int x, int y, float phi );
    void phaseUnwrap(int x, int y, float phi, float dist );
    
    void computeQuality();

    // inline helper functions
    float max_phase(float v1, float v2, float v3) {
        float max;
        max = v1>v2 ? v1 : v2;
        max = max>v3 ? max : v3;
        return max;
    }

    float min_phase(float v1, float v2, float v3) {
        float min = v1<v2 ? v1 : v2;
        min = min<v3 ? min : v3;
        return min;
    }

    /* use mean as luminance of an rgb triple */
    float luminance(uchar *color) {return (color[0]+color[1]+color[2])/(3.f*255);}

    void copy_channels(uchar *dest, uchar *src) {
        for(int i=0;i<3;i++)
            *(dest+i) = *(src+i);
    } 

    float sqdist(float v1, float v2) { 
        float d = v1-v2;
        return 1-d*d;
    }

private:

    IplImage* imgPhase1;
    IplImage* imgPhase2;
    IplImage* imgPhase3;
    IplImage* imgColor;  // reconstructed color image 
    IplImage* imgWrappedPhase;
    IplImage* imgUnwrappedPhase;

    // some helper matrices to track phase quality and
    // processing state (each from the same dimension as the input image)
    bool  *mask;
    bool  *process;
    float *quality;
    float *range;
    float *depth;
    float noiseThreshold;
    float zscale;
    float zskew;

    int width;
    int height;
    int step;   // for single channel images

    //deque<UnwrapPath> procQueue;
    priority_queue<UnwrapPath> processHeap;
};

#endif
