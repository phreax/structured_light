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
    QObject::connect(ui->zskewSlider,SIGNAL(sliderReleased()),this,SLOT(updateZMatrix()));
    QObject::connect(ui->zscaleSlider,SIGNAL(sliderReleased()),this,SLOT(updateZMatrix()));

    updateDecoder();

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

void SLApp::setThreshold(int value) {
    float threshold = value * 1e-2f;
    QString label;
    label.setNum(threshold,'f',2);
    ui->thresholdValue->setText(label);
    decoder->setNoiseThreshold(threshold);
}

void SLApp::setZscale(int value) {
    ui->zscaleValue->setNum(value);
    decoder->setZscale(value);
}

void SLApp::setZskew(int value) {
    ui->zskewValue->setNum(value);
    decoder->setZskew(value);
}

void SLApp::updateDecoder() {
    if(decoder)
        decoder->compute();

    updatePointCloud();
    
}

// only recompute zmatrix
void SLApp::updateZMatrix() {

    if(decoder)
        decoder->computeDepth();

    updatePointCloud();
}

void SLApp::updatePointCloud() {

    // update visualization
    ui->pointcloud->setZMatrix(decoder->getDepth());
    IplImage *imgColor = decoder->getColorImage();

    ui->pointcloud->setTexture(imgColor);
    ui->pointcloud->setMask(decoder->getMask());
    ui->pointcloud->updateGL();
}

SLApp::~SLApp()
{
    delete ui;
    delete decoder;
}
