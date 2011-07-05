#include <QtGui/QApplication>
#include "slapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SLApp *w = 0;
    if(argc>3)
        w = new SLApp(0,argv[1],argv[2],argv[3]);
    else
        w = new SLApp(0,"../img/phase1.jpg","../img/phase2.jpg","../img/phase3.jpg");

    w->show();

    return a.exec();
}
