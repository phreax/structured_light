#include <QGLWidget>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QObject>
#include <QWidget>
#include <cv.h>
#include <highgui.h>
#include <math.h>


#ifndef GLPOINTCLOUD_H 
#define GLPOINTCLOUD_H 

class GLPointCloud : public QGLWidget {    

    Q_OBJECT

public:

    GLPointCloud(QWidget *parent)
        : QGLWidget(parent) 
    {
        render_detail = 5;
        alpha = 0.8;
        angle = 0.0f;
        phi=0.0f;
        pad=0.0f;
        nwaves = 5;

        capture = cvCreateCameraCapture(0) ;
        texture = cvQueryFrame( capture );
        height = texture->height;
        width  = texture->width;
        step   = texture->widthStep;

        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onIdle()));
        m_timer->start(30);

    }

    void setZMatrix(float *_zmatrix) {zmatrix = _zmatrix;}
    void setTexture(IplImage *_texture) {texture = _texture;}

public slots:

    void onIdle() {updateGL();}

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    float    *zmatrix;   // depth matrix
    IplImage *texture;

    int render_detail;
    int height,width,step;
    float alpha;
    float angle;
    float phi;
    float pad;
    int nwaves;
    CvCapture* capture; 

    QTimer *m_timer;
};

#endif
