#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QObject>
#include <QWidget>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <cstdio> 


#ifndef GLPOINTCLOUD_H 
#define GLPOINTCLOUD_H 

class GLPointCloud : public QGLWidget {    

    Q_OBJECT

public:

    GLPointCloud(QWidget *parent)
        : QGLWidget(parent), ready(false), m_mouseDown(false)
    {
        render_detail = 1;
        alpha = 0.8;
        anglex = 0.0f;
        angley = 0.0f;
        phi=0.0f;
        pad=0.0f;
        nwaves = 5;

        setMouseTracking(true);

        /*m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onIdle()));
        m_timer->start(30);*/

    }

    void setTexture(IplImage *_texture) {
        texture = _texture;

        if(!texture) {
            printf("corrupted texture, doing nothing..\n");
        }
        twidth = texture->width;
        theight = texture->height;
        step =  texture->widthStep;

        ready = true;
    }
    
    void setZMatrix(float *_zmatrix) {zmatrix = _zmatrix;}

public slots:

    void onIdle() {updateGL();}

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // capture mouse events
    void mouseMoveEvent(QMouseEvent *ev) {
        if(m_mouseDown) {
            int dx = ev->pos().x()- m_pos.x();
            int dy = ev->pos().y()- m_pos.y();

            m_pos = ev->pos();

            anglex += 360.f*dx/width();
            angley += 360.f*dy/height();

            /*if(anglex>360.f) anglex=360;
            if(anglex<0.f) anglex +=360;

            if(angley>360.f) angley=360;
            if(angley<0.f) angley +=360;*/

            updateGL();
        }
    }

    void mousePressEvent(QMouseEvent *event) {
        m_mouseDown = true;
        m_pos = event->pos();
    }

    void mouseReleaseEvent(QMouseEvent* event) {
        m_mouseDown = false;
    }

            
private:

    bool ready;

    float    *zmatrix;   // depth matrix
    IplImage *texture;

    int render_detail;
    int theight,twidth,step;
    
    float alpha;
    float anglex;
    float angley;
    float phi;
    float pad;
    int nwaves;

    bool   m_mouseDown;
    QPoint m_pos;

    QTimer *m_timer;
};

#endif
