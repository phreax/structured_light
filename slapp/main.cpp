#include <QtGui/QApplication>
#include "slapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SLApp w;
    w.show();

    return a.exec();
}
