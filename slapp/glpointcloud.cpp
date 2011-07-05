#include "glpointcloud.h"

void GLPointCloud::initializeGL() {

    glEnable(GL_POINT_SMOOTH|GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    setAutoBufferSwap(true);
}

void GLPointCloud::resizeGL(int w, int h) {

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

    
void GLPointCloud::paintGL() {

    if(!ready) return;

    if(!texture) {
        printf("corrupt texture image! doing nothing..\n");
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    printf("drawing points\n");

    // get new texture from camera
    glLoadIdentity();
    gluLookAt( 0.0,0.0,2.0,
               0.0,0.0,0.0,
               0.0,1.0,0.0);

    
    glRotatef(180.f,0.f,0.f,1.f);
    glRotatef(anglex,0,1,0);
    glRotatef(angley,1,0,0);

    float x,y,z,w,h,dist;
    
    
    float centerX = twidth/2;
    float centerY = theight/2;
    
    z = 0.f;
    w = 1.0f;
    h = theight/(float)twidth;

    //glRotatef(180.f,0.f,0.f,1.f);
    //glRotatef(angle,0.f,1.f,0.f);
    glTranslatef(-w/2,-h/2,0.f);
    uchar * texturePtr = (uchar*) texture->imageData;

    uchar cr,cb,cg;

    glPointSize(1.5);
    glBegin(GL_POINTS);

    for(int i=0;i<theight;i+=render_detail)
        for(int j=0;j<twidth;j+=render_detail) {
            
            x = (float)j/twidth;
            y = (float)i/twidth;
            z = zmatrix[i*twidth+j]/twidth;

            cb = (uchar)texturePtr[i*step+j*3];
            cg = (uchar)texturePtr[i*step+j*3+1];
            cr = (uchar)texturePtr[i*step+j*3+2];

            glColor4ub(cr,cg,cb,50);

            // draw vertex
            glVertex3f(x,y,z);
            
        }

    glEnd();
}

