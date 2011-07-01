#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

IplImage *frame = 0;
int render_detail = 5;
int height,width,step;
float alpha = 0.8;
float angle = 0.0f;
float phi=0.0f;
float pad=0.0f;
int nwaves = 5;
CvCapture* capture; 

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 1.0f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // get new frame from camera
    frame = cvQueryFrame(capture); 
    glLoadIdentity();
    gluLookAt( 0.0,0.0,-2.0,
               0.0,0.0,0.0,
               0.0,1.0,0.0);

//    glTranslatef( -0.2,-0.5,0.0);
//    glRotatef(angle,1.0f,0.0f,0.0f);
//    glTranslatef( 0.2,0.5,0.0);


    float x,y,z,w,h,dist;
    
    float centerX = width/2;
    float centerY = height/2;
    w = 1.0f;
    h = height/(float)width;


    glRotatef(180.f,0.f,0.f,1.f);
    glRotatef(angle,0.f,1.f,0.f);
    glTranslatef(-w/2,-h/2,0.f);
    uchar * framePtr = (uchar*) frame->imageData;

    uchar cr,cb,cg;

    z = 0.0f;
    // sinwave
    float alpha    = w/20;
    float period   = 1.0f/(nwaves);
    float omega   = 2*M_PI/period;

    glPointSize(1.5);
    glBegin(GL_POINTS);

    for(int i=0;i<height;i+=render_detail)
        for(int j=0;j<width;j+=render_detail) {
            
            x = pad+(float)j/width;
            y = pad+(float)i/width;

            dist = sqrt(pow(x-(w/2),2)+pow(y-(h/2),2));
            z = exp(-2*dist)*cos(omega*sqrt(pow(x-(w/2),2)+pow(y-(h/2),2))+phi)*alpha;
            if(x==centerX && y == centerY) {
                printf("z=%.2f\n",z);
            }

            cb = (uchar)framePtr[i*step+j*3];
            cg = (uchar)framePtr[i*step+j*3+1];
            cr = (uchar)framePtr[i*step+j*3+2];

//            printf("color = (%d,%d,%d)\n",cr,cg,cb);
            glColor4ub(cr,cg,cb,0);

            // draw vertex
            glVertex3f(x,y,z);
            
        }

    glEnd();

    angle+=2.f;
    phi+=0.5f;
    glutSwapBuffers();
}

int main(int argc, char **argv) {

    /*if(argc < 2 ) {
        printf("Usage: %s <image>\n",argv[0]);
    }*/

    //frame = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);

    /*if(!frame) {
        printf("Could not load image %s\n", argv[1]);
        exit(0);
    }*/

    capture = cvCreateCameraCapture(0) ;
    frame = cvQueryFrame( capture );
    height = frame->height;
    width  = frame->width;
    step   = frame->widthStep;

    printf("Loading image:\n\twidth:%d\n\theight:%d\n\tchannels:%d\n",width,height,frame->nChannels);

    // init GLUT
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(800,600);
    glutInitWindowSize(800,600);
    glutCreateWindow("Point Cloud Visualization");

    glEnable(GL_POINT_SMOOTH|GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutMainLoop();

    return 0;
}




    
