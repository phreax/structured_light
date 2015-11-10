#include "capturedialog.h"
#include "ui_capturedialog.h"
#include <QCoreApplication>

CaptureDialog::CaptureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureDialog),
    imgPhase1(0),
    imgPhase2(0),
    imgPhase3(0),
    m_capture(0),
    m_frame(0),
    m_state(Empty),
    m_width(640),
    m_height(480),
    m_aspect(0.75f),
    m_timer(0)

{
    ui->setupUi(this);


    // disable Ok button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    // connect slots
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->grabButton, SIGNAL(clicked()), this, SLOT(grabImage()));

    // initialize capture
    m_capture = cvCreateCameraCapture(0);
    if(m_capture) {
        // grab first frame to 
        m_frame = cvQueryFrame(m_capture);
    }

    if(m_frame) {
        int w = m_frame->width;
        int h = m_frame->height;

        // aspect ratio heigh/width
        m_aspect = h/(float)w;
        m_width  = w <= m_width? w : m_width;
        m_height = (int) m_width*m_aspect; 

        updateLayout();
        cout << "Update Layout" << endl;
    }

    // periodically update frame
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(refreshFrame()));
    m_timer->start(50);

}

void CaptureDialog::updateLayout() {

    // set camImage size
    ui->camImage->resize(m_width,m_height);

    // compute size for thumbs
    int thumbWidth  = m_width/3 -20;
    int thumbHeight = (int) thumbWidth*m_aspect;

    cout << "resize thumbs to " << thumbWidth << "x" << thumbHeight << endl;

    ui->phase1Thumb->resize(thumbWidth,thumbHeight);
    ui->phase2Thumb->resize(thumbWidth,thumbHeight);
    ui->phase3Thumb->resize(thumbWidth,thumbHeight);
    cout << "real size " << ui->phase3Thumb->width() << "x" << ui->phase3Thumb->height() << endl;

}


void CaptureDialog::refreshFrame() {
        
    if(m_capture) {
        // grab first frame to 
        m_frame = cvQueryFrame(m_capture);
    }

    if(m_frame) {
       QCoreApplication::processEvents();
       setPixmap(ui->camImage,m_frame,m_width,m_height);
       QCoreApplication::processEvents();
    }
}


void CaptureDialog::reset() {

    // reset everything
    if(imgPhase1) cvReleaseImage(&imgPhase1);
    if(imgPhase2) cvReleaseImage(&imgPhase2);
    if(imgPhase3) cvReleaseImage(&imgPhase3);

    ui->phase1Thumb->clear(); ui->phase1Thumb->setText("Phase 1");
    ui->phase2Thumb->clear(); ui->phase2Thumb->setText("Phase 2");
    ui->phase3Thumb->clear(); ui->phase3Thumb->setText("Phase 3");

    m_state = Empty;
    
    // disable
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
}

    
void CaptureDialog::grabImage() {

    int thumbWidth  = m_width/3 -20;
    int thumbHeight = (int) thumbWidth*m_aspect;

    switch(m_state) {
        case Empty:
            imgPhase1 = copyFrame();
            setPixmap(ui->phase1Thumb,imgPhase1,thumbWidth,thumbHeight);
            break;

        case Await1:
            imgPhase2 = copyFrame();
            setPixmap(ui->phase2Thumb,imgPhase2,thumbWidth,thumbHeight);
            break;

        case Await2:
            imgPhase3 = copyFrame();
            setPixmap(ui->phase3Thumb,imgPhase3,thumbWidth,thumbHeight);
            break;
    }

    updateState();
}

void CaptureDialog::updateState() {
    switch(m_state) {
        case Empty:
            m_state = Await1;
            break;
        case Await1:
            m_state = Await2;
            break;
        case Await2:
            m_state = Complete;

            // enable Ok button
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            break;
    }
}

CaptureDialog::~CaptureDialog()
{
    delete ui;
    cvReleaseCapture(&m_capture);
}
