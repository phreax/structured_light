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
        : QGLWidget(parent), m_mouseDown(false)
    {
        render_detail = 1;
        anglex = 0.0f;
        angley = 0.0f;

        setMouseTracking(true);

    }

    void setTexture(IplImage *_texture) {
        texture = _texture;

        if(!texture) {
            printf("corrupted texture, doing nothing..\n");
        }
        twidth = texture->width;
        theight = texture->height;
        step =  texture->widthStep;

    }
    
    void setZMatrix(float *_zmatrix) {zmatrix = _zmatrix;}
    void setMask(bool *_mask) {mask = _mask;}

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // capture mouse events
    void mouseMoveEvent(QMouseEvent *ev);

    void mousePressEvent(QMouseEvent *event) {
        m_mouseDown = true;
        m_pos = event->pos();
    }

    void mouseReleaseEvent(QMouseEvent* event) {
        m_mouseDown = false;
    }
            
private:

    float    *zmatrix;   // depth matrix
    bool     *mask;
    IplImage *texture;

    int render_detail;
    int theight,twidth,step;
    
    float anglex;
    float angley;

    bool   m_mouseDown;
    QPoint m_pos;

    QTimer *m_timer;
};

#endif
