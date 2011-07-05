#ifndef SLAPP_H
#define SLAPP_H

#include <QMainWindow>
#include <iostream> 
#include "ui_slapp.h"
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

private:

    void setupDecoder(const char* img1, const char* img2, const char*img3);

    Ui::SLApp *ui;
    ThreeStepPhaseShift *decoder;

};

#endif // SLAPP_H
