#!/usr/bin/python

import cv
from numpy import *
import sys

width  = 1024 
height = 768

nstripes = 30
alpha = 2*pi/3 # phase shift 120 degree

pattern_dir = "./pattern"

def genFringe(h,w):

    fringe1 = zeros((h,w))
    fringe2 = zeros((h,w))
    fringe3 = zeros((h,w))

    # periodlength:
    phi = w/nstripes
    
    # scale factor delta = 2pi/phi
    delta = 2*pi/phi

    f = lambda x,a : (sin(x*delta+a) +1)* 120
    
    # compute a row of fringe pattern
    sinrow1 = [f(x,-alpha) for x in xrange(w)]
    sinrow2 = [f(x,0) for x in xrange(w)]
    sinrow3 = [f(x,alpha) for x in xrange(w)]
    
    fringe1[:,:] = sinrow1
    fringe2[:,:] = sinrow2
    fringe3[:,:] = sinrow3

    return fringe1,fringe2,fringe3

def genFringeVert():
    return map(arr2ipl,genFringe(height,width))

def genFringeHor():
    imgarr = genFringe(width,height)
    return map(rotate,map(arr2ipl,imgarr))

def arr2ipl(arr,imgtype=cv.IPL_DEPTH_8U):

    img = cv.CreateImageHeader((arr.shape[1],arr.shape[0]),imgtype,1)
    a = array(arr,uint8)
    cv.SetData(img,a.tostring(),a.dtype.itemsize*a.shape[1])

    return img

def rotate(img):
    timg = cv.CreateImage((img.height,img.width), img.depth, img.channels)
    cv.Transpose(img,timg)
    cv.Flip(timg,timg,flipMode=1)
    return timg

if __name__ == "__main__":

    nstripes = int(sys.argv[1])

    images = genFringeHor()

    i=1
    for img in images:
        cv.ShowImage("fringe%d"%i,img)
        cv.SaveImage("%s/fringe%d.png"%(pattern_dir,i),img)
        i += 1

    while True:
        k = cv.WaitKey(0)
        if k=='q':
            break


