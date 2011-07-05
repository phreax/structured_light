#include "slapp.h"
#include <QObject>

SLApp::SLApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SLApp), decoder(0)
{
    ui->setupUi(this);
    setupDecoder();

    // connect slider
    QObject::connect(ui->thresholdSlider,SIGNAL(valueChanged(int)),this,SLOT(setThreshold(int)));
    QObject::connect(ui->zskewSlider,SIGNAL(valueChanged(int)),this,SLOT(setZskew(int)));
    QObject::connect(ui->zscaleSlider,SIGNAL(valueChanged(int)),this,SLOT(setZscale(int)));

    QObject::connect(ui->thresholdSlider,SIGNAL(sliderReleased()),this,SLOT(updateDecoder()));
    QObject::connect(ui->zskewSlider,SIGNAL(sliderReleased()),this,SLOT(updateDecoder()));
    QObject::connect(ui->zscaleSlider,SIGNAL(sliderReleased()),this,SLOT(updateDecoder()));

}

void SLApp::setupDecoder() {

    if(!decoder) {
        IplImage *phase1 = cvLoadImage("../img/phase1.jpg");
        IplImage *phase2 = cvLoadImage("../img/phase2.jpg");
        IplImage *phase3 = cvLoadImage("../img/phase3.jpg");

        // setup phase decoder
        decoder = new ThreeStepPhaseShift(phase1,phase2,phase3);
    }

    // set up initual values TODO: less dirty
    setThreshold(10);
    setZscale(120);
    setZskew(26);

    ui->thresholdSlider->setValue(10);
    ui->zscaleSlider->setValue(120);
    ui->zskewSlider->setValue(26);
}

SLApp::~SLApp()
{
    delete ui;
    delete decoder;
}
