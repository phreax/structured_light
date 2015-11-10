#include "three_step_phase_shift.h"
#include <cstdio>


float printMinMax(IplImage *img) {
    
    int w = img->width;
    int h = img->height;
    int step = w; 

    float  *ptr = (float *)img->imageData;
    int i;
    float min,max;
    min = 1e6;
    max = 1e-6;

    for (i = 0; i < h ; i++) {
    for (int j = 0; j < w; j++) {
        float val = (float)ptr[i*step+j*4];
        if(val<min) min = val;
        if(val>max) max = val;
    }

    }

    cout << "min: " << min << "\nmax: "<<max << endl;

    return max;
    
}

IplImage *boolarr2img(bool* barr, CvSize s) {

    IplImage *img = cvCreateImageHeader(s,IPL_DEPTH_8U,1);
    img->imageData = (char *)barr;
    cvConvertScale(img,img,255);
    return img;
}

void scale(IplImage *img) {
    
    int w = img->width;
    int h = img->height;
    int step = w; 

    float  *ptr = (float *)img->imageData;
    int i;
    float min,max;
    min = 1e6;
    max = 1e-6;

    for (i = 0; i < h ; i++) {
    for (int j = 0; j < w; j++) {
        float val = (float)ptr[i*step+j*4];
        if(val<min) min = val;
        if(val>max) max = val;
    }

    }

    cout << "min: " << min << "\nmax: "<<max << endl;
    cvScale(img,img, 1/(max-min), -min/(max-min)); 

}


void makeimg() {
    IplImage *img = cvCreateImage(cvSize(480,640),IPL_DEPTH_32F,1);
    
    int w = img->width;
    int h = img->height;
    int step = img->widthStep;
    uchar *ptr = (uchar *)img->imageData;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j< w; j++) {
            ptr[i*step+j*4] = 0.4;
        }
    }
    cvScale(img, img, 2, 0.5); 
    cvShowImage("white",img);
    cvWaitKey(0);
}



int main(int argc, const char *argv[])
{
    if(argc<4) {
        printf("too fewer arguments!");
        return -1;
    }
    
//    makeimg();
    IplImage *phase1 = cvLoadImage(argv[1]);
    IplImage *phase2 = cvLoadImage(argv[2]);
    IplImage *phase3 = cvLoadImage(argv[3]);

    ThreeStepPhaseShift decoder(phase1,phase2,phase3);

    decoder.phaseDecode();
    IplImage* wrappedPhase = decoder.getWrappedPhase();
    //IplImage* imgColor = decoder.getColorImage();
    printMinMax(wrappedPhase);
    cvScale(wrappedPhase, wrappedPhase, 1, 0.5); 
    
    decoder.compute();
    
    IplImage* unwrappedPhase = decoder.getUnwrappedPhase();
    scale(unwrappedPhase);
    //printMinMax(unwrappedPhase);
    
    IplImage *imgDepth = cvCreateImageHeader(cvGetSize(unwrappedPhase),IPL_DEPTH_32F,1);
    imgDepth->imageData = (char *)decoder.getDepth();
    scale(imgDepth);
    printMinMax(imgDepth);
    //printMinMax(unwrappedPhase);

    cvShowImage("depth",imgDepth);
    cvShowImage("wrapped phase",wrappedPhase);
    cvShowImage("unwrapped phase",unwrappedPhase);
    cvShowImage("mask",boolarr2img(decoder.getMask(),cvGetSize(wrappedPhase)));
    
    cvWaitKey(0);

    return 0;
}
