#include "glpointcloud.h"

void GLPointCloud::initializeGL() {

    glEnable(GL_LIGHTING|GL_POINT_SMOOTH|GL_COLOR_MATERIAL);
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

    if(!texture||!texture||!mask) {
        printf("no data loaded! doing nothing..\n");
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // set camara position
    gluLookAt( 0.0,0.0,2.0,
               0.0,0.0,0.0,
               0.0,1.0,0.0);
    
    // rotate
    glRotatef(180.f,0.f,0.f,1.f);
    glRotatef(anglex,0,1,0);
    glRotatef(angley,1,0,0);

    float x,y,z,w,h,dist;
    
    z = 0.f;
    w = 1.0f;
    h = theight/(float)twidth;

    // center to image center
    glTranslatef(-w/2,-h/2,0.f);
    uchar * texturePtr = (uchar*) texture->imageData;

    uchar cr,cb,cg;

    glPointSize(1.0);
    glBegin(GL_POINTS);

    for(int i=0;i<theight;i+=render_detail)
        for(int j=0;j<twidth;j+=render_detail) {
            
            if(mask[i*twidth+j]) continue; // ignore masked pixels

            x = (float)j/twidth;
            y = (float)i/twidth;
            z = zmatrix[i*twidth+j]/twidth;

            cb = (uchar)texturePtr[i*step+j*3];
            cg = (uchar)texturePtr[i*step+j*3+1];
            cr = (uchar)texturePtr[i*step+j*3+2];

            //glColor4ub(cr,cg,cb,50);
            glColor3ub(cr,cg,cb);

            // draw vertex
            glVertex3f(x,y,z);
            
        }

    glEnd();
}

void GLPointCloud::mouseMoveEvent(QMouseEvent *ev) {
    if(m_mouseDown) {
        int dx = ev->pos().x()- m_pos.x();
        int dy = ev->pos().y()- m_pos.y();

        m_pos = ev->pos();

        anglex += 360.f*dx/width();
        angley += 360.f*dy/height();


        // mod 360
        if(anglex>360.f) anglex-=360;
        if(anglex<0.f) anglex +=360;

        if(angley>360.f) angley-=360;
        if(angley<0.f) angley +=360;

        updateGL();
    }
}
