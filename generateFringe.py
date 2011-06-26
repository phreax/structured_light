import cv
from numpy import *
import sys

width  = 800
height = 600

nstripes = 1
alpha = 2*pi/3 # phase shift 120 degree

pattern_dir = "./pattern"

def genFringeVert():

    fringe1 = zeros((height,width))
    fringe2 = zeros((height,width))
    fringe3 = zeros((height,width))

    # periodlength:
    phi = width/nstripes
    
    # scale factor delta = 2pi/phi
    delta = 2*pi/phi

    f = lambda x,a : (sin(x*delta+a) +1)* 0.5
    
    # compute a row of fringe pattern
    sinrow1 = [f(x,-alpha) for x in xrange(width)]
    sinrow2 = [f(x,0) for x in xrange(width)]
    sinrow3 = [f(x,alpha) for x in xrange(width)]
    
    fringe1[:,:] = sinrow1
    fringe2[:,:] = sinrow2
    fringe3[:,:] = sinrow3

    return fringe1,fringe2,fringe3

def arr2ipl(arr,imgtype=cv.IPL_DEPTH_8U):

    img = cv.CreateImage(arr.shape[1],arr.shape[2],imgtype,1)
    img.SetData(arr,cv.CV_AUTOSTEP)

    return img

if __name__ == "__main__":

    nstripes = int(sys.argv[1])

    images = genFringeVert()

    i=1
    for img in images:
        cv.ShowImage("fringe%d"%i,img)
        cv.SaveImage("%s/fringe%d.png"%(pattern_dir,i),img)
        i += 1

    cv.WaitKey(0)







    




