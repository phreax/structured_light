#ifndef CAPTUREDIALOG_H
#define CAPTUREDIALOG_H

#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include <QLabel>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>

using namespace std;


#define CAM_MAX_WIDTH 640

namespace Ui {
    class CaptureDialog;
}

class CaptureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CaptureDialog(QWidget *parent = 0);
    ~CaptureDialog();

    QList<IplImage *> *getImages() {
        int acc = exec();
        if(acc==Accepted) {
            QList<IplImage*> *images = new QList<IplImage*>();
            *images << (imgPhase1);
            *images << (imgPhase2);
            *images << (imgPhase3);

            return images;
        }

        return NULL;
    }

public slots:

    void grabImage();

    void reset();
    void refreshFrame();

signals:

    void stateChanged();

protected:

    enum State {Empty, Await1, Await2, Complete};

    QImage * fromIplImage(IplImage *image) {
        int w = image->width;
        int h = image->height;
        QImage *qimage = new QImage((uchar *)image->imageData,w,h,QImage::Format_RGB888);
                    
        return qimage;

    }

    IplImage *copyFrame() {
        if(m_frame) {
            IplImage *current = cvCreateImage(cvGetSize(m_frame),m_frame->depth,m_frame->nChannels);
            cvCopy(m_frame,current);
            return current;
        }
       
        return 0;
    }

    void setPixmap(QLabel *label, IplImage *image, int w, int h) {

        QImage *qimage = fromIplImage(image);
        QPixmap pixmap;
        #if QT_VERSION < 0x040700
        pixmap = QPixmap::fromImage(qimage->rgbSwapped());
        #else
        pixmap.convertFromImage(*qimage);
        #endif
       
        // set label
        label->setPixmap(pixmap.scaled(w,h));
        cout << "pixmap size " << w << "x" << h << endl;
        cout << "label size " << label->width() << "x" << label->height() << endl;
        delete qimage;
    }


    void updateState();
    void updateLayout();

private:
    Ui::CaptureDialog *ui;

    IplImage *imgPhase1;
    IplImage *imgPhase2;
    IplImage *imgPhase3;
    CvCapture *m_capture;
    IplImage  *m_frame;

    State m_state;

    int m_width;
    int m_height;
    float m_aspect;

    QTimer *m_timer;
};

#endif // CAPTUREDIALOG_H
