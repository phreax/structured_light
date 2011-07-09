#include "slapp.h"
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

SLApp::SLApp(QWidget *parent, const char* img1, const char* img2, const char*img3) :
    QMainWindow(parent),
    ui(new Ui::SLApp), decoder(0), m_timestamp(0)
{
    ui->setupUi(this);
    setupDecoder(img1,img2,img3);

    // connect slider
    QObject::connect(ui->thresholdSlider,SIGNAL(valueChanged(int)),this,SLOT(setThreshold(int)));
    QObject::connect(ui->zskewSlider,SIGNAL(valueChanged(int)),this,SLOT(setZskew(int)));
    QObject::connect(ui->zscaleSlider,SIGNAL(valueChanged(int)),this,SLOT(setZscale(int)));

    QObject::connect(ui->thresholdSlider,SIGNAL(sliderReleased()),this,SLOT(updateDecoder()));
    QObject::connect(ui->zskewSlider,SIGNAL(sliderReleased()),this,SLOT(updateZMatrix()));
    QObject::connect(ui->zscaleSlider,SIGNAL(sliderReleased()),this,SLOT(updateZMatrix()));


    connect(ui->captureAct, SIGNAL(triggered()), this, SLOT(newCapture()));
}

void SLApp::setupDecoder(QList<IplImage *> *images) {
    if(images) {
        // delete old decoder, if any
        if(decoder) delete decoder;
        decoder = new ThreeStepPhaseShift(images->at(0),images->at(1),images->at(2));
        
        // save images stamped with time
        QDateTime current = QDateTime::currentDateTime();
        if(m_timestamp) delete m_timestamp;
        m_timestamp = new QString(current.toString(Qt::ISODate));
        QString f1 = "../img/img"+*m_timestamp+"-1.jpg";
        QString f2 = "../img/img"+*m_timestamp+"-2.jpg";
        QString f3 = "../img/img"+*m_timestamp+"-3.jpg";
        cvSaveImage(f1.toStdString().c_str(), images->at(0));
        cvSaveImage(f2.toStdString().c_str(), images->at(1));
        cvSaveImage(f3.toStdString().c_str(), images->at(2));
        
        updateDecoder();
    }
}


void SLApp::setupDecoder(const char* img1, const char* img2, const char*img3) {

    if(!decoder&&img1&&img2&&img3) {
        IplImage *phase1 = cvLoadImage(img1);
        IplImage *phase2 = cvLoadImage(img2);
        IplImage *phase3 = cvLoadImage(img3);

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
    updateDecoder();
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
    
    if(m_timestamp) {
        // save parameters
        QString f1 = "../img/img"+*m_timestamp+"-1.jpg";
        QString f2 = "../img/img"+*m_timestamp+"-2.jpg";
        QString f3 = "../img/img"+*m_timestamp+"-3.jpg";

        QString parameters = "../img/img"+*m_timestamp+"parameters.txt";
        QFile file(parameters);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << f1 << endl << f2 << endl << f3 << endl;
        out << decoder->getNoiseThreshold() << endl;
        out << decoder->getZscale() << endl;
        out << decoder->getZskew() << endl;
        file.close();
    }

    updatePointCloud();
    
}

// only recompute zmatrix
void SLApp::updateZMatrix() {

    if(decoder) { 
        decoder->computeDepth();
        updatePointCloud();
    }
}

void SLApp::updatePointCloud() {

    // update visualization
    ui->pointcloud->setZMatrix(decoder->getDepth());
    IplImage *imgColor = decoder->getColorImage();

    ui->pointcloud->setTexture(imgColor);
    ui->pointcloud->setMask(decoder->getMask());
    ui->pointcloud->updateGL();
}

void SLApp::newCapture() {

    CaptureDialog cdlg(this);
    QList<IplImage *>* images = cdlg.getImages();

    if(images) {
        setupDecoder(images);
    }
    else
        cout << "No images captured" << endl;

}

SLApp::~SLApp()
{
    delete ui;
    delete decoder;
}
