Structured Light
================

An implementation of a 3D scanner using the three-step phase shifting method.


How it works
------------

In a calibrated setup, a beamer is used to project special stripe patterns onto
the target object. A camera then captures the scene for each the different patterns.
As the camera sees the stripes from a different angle than the beamer, they appear distorted
proportionally to depth of the object's surface.
From the difference of the distorted to the original pattern, the depth for each pixel can be reconstructed.

The patterns are grayscale images, that consist from an equal number of horizontal stripes with a sine intensity distribution. In each pattern, the sine wave is shifted by 120° or 2/3 PI.
From the different intensity values, a relative phase in the range of \[-PI,PI\] can be computed.
The discontinueties of the wrapped phase map are unwrapped using a flood fill algorithm, by following neigbouring pixels with the best phase quality, starting in the center of the image.
The resulting absolute phase, is proportional to he surface depth of the pixel.

![Setup (Source: https://en.wikipedia.org/wiki/Structured-light_3D_scanner)](https://raw.githubusercontent.com/phreax/structured_light/master/images/setup.png)


Files
-----

In the root dirctory, you will find various code, like implementation of the algorithm, a simple commandline test application, a script for generating the fringe pattern images and code for 
viewing the result as a 3D point cloud.

The **slapp** directory contains graphical application with basic user interface, that supports:

* capturing the images
* performing reconstruction
* viewing point cloud
* simple adjusting of parameters for calibration


Requirements
------------

* C++ compiler
* OpenCV
* OpenGL / glut 
* QT 4 (for the SLapp)
* OS: Linux


Usefull Papers
--------------

**Wikipedia**:
- https://en.wikipedia.org/wiki/Structured-light_3D_scanner

**Calibration**:
- http://mesh.brown.edu/3DPGP-2007/pdfs/Calibration_OE.pdf

**Three Phase Shift**
- https://trac.v2.nl/attachment/wiki/structuredLight/3phase-shift.pdf
- https://trac.v2.nl/wiki/structuredLight 

**Phase Unwrapping**
- http://www.iaeng.org/publication/WCE2010/WCE2010_pp633-636.pdf
