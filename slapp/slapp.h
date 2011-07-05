#ifndef SLAPP_H
#define SLAPP_H

#include <QMainWindow>
#include "ui_slapp.h"
#include "../three_step_phase_shift.h"

namespace Ui {
    class SLApp;
}

class SLApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SLApp(QWidget *parent = 0);
    ~SLApp();

signals:


public slots:

    void setThreshold(int value) {
        float threshold = value * 1e-2f;
        QString label;
        label.setNum(threshold,'f',2);
        ui->thresholdValue->setText(label);
        decoder->setNoiseThreshold(threshold);
    }

    void setZscale(int value) {
        ui->zscaleValue->setNum(value);
        decoder->setZscale(value);
    }

    void setZskew(int value) {
        ui->zskewValue->setNum(value);
        decoder->setZskew(value);
    }
    
    void updateDecoder() {
        if(decoder)
            decoder->compute();
    }

private:

    void setupDecoder();

    Ui::SLApp *ui;
    ThreeStepPhaseShift *decoder;

};

#endif // SLAPP_H
