#ifndef SLAPP_H
#define SLAPP_H

#include <QMainWindow>
#include <QList>
#include <iostream> 
#include "ui_slapp.h"
#include "capturedialog.h"
#include "../three_step_phase_shift.h"

using namespace std;
namespace Ui {
    class SLApp;
}
class SLApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SLApp(QWidget *parent = 0, const char* img1=0, const char* img2=0, const char*img3=0);
    ~SLApp();

public slots:

    void setThreshold(int value);
    void setZscale(int value);
    void setZskew(int value);
    void updateDecoder();
    void updateZMatrix();// only recompute zmatrix
    void updatePointCloud();

    void newCapture();

private:

    void setupDecoder(const char* img1, const char* img2, const char*img3);
    void setupDecoder(QList<IplImage *> *images);

    Ui::SLApp *ui;
    ThreeStepPhaseShift *decoder;
    QString *m_parameters;
    QString *m_timestamp;
};

#endif // SLAPP_H
