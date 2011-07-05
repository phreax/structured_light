/********************************************************************************
** Form generated from reading UI file 'slapp.ui'
**
** Created: Tue Jul 5 04:40:40 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAPP_H
#define UI_SLAPP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glpointcloud.h"

QT_BEGIN_NAMESPACE

class Ui_SLApp
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    GLPointCloud *pointcloud;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_2;
    QSlider *thresholdSlider;
    QSlider *zscaleSlider;
    QSlider *zskewSlider;
    QVBoxLayout *verticalLayout_3;
    QLabel *thresholdValue;
    QLabel *zscaleValue;
    QLabel *zskewValue;

    void setupUi(QMainWindow *SLApp)
    {
        if (SLApp->objectName().isEmpty())
            SLApp->setObjectName(QString::fromUtf8("SLApp"));
        SLApp->resize(1194, 692);
        centralwidget = new QWidget(SLApp);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pointcloud = new GLPointCloud(centralwidget);
        pointcloud->setObjectName(QString::fromUtf8("pointcloud"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pointcloud->sizePolicy().hasHeightForWidth());
        pointcloud->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(pointcloud);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        thresholdSlider = new QSlider(centralwidget);
        thresholdSlider->setObjectName(QString::fromUtf8("thresholdSlider"));
        thresholdSlider->setMaximum(100);
        thresholdSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(thresholdSlider);

        zscaleSlider = new QSlider(centralwidget);
        zscaleSlider->setObjectName(QString::fromUtf8("zscaleSlider"));
        zscaleSlider->setMaximum(500);
        zscaleSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(zscaleSlider);

        zskewSlider = new QSlider(centralwidget);
        zskewSlider->setObjectName(QString::fromUtf8("zskewSlider"));
        zskewSlider->setMinimum(-100);
        zskewSlider->setMaximum(100);
        zskewSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(zskewSlider);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(10, -1, 50, -1);
        thresholdValue = new QLabel(centralwidget);
        thresholdValue->setObjectName(QString::fromUtf8("thresholdValue"));
        thresholdValue->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(thresholdValue);

        zscaleValue = new QLabel(centralwidget);
        zscaleValue->setObjectName(QString::fromUtf8("zscaleValue"));
        zscaleValue->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(zscaleValue);

        zskewValue = new QLabel(centralwidget);
        zskewValue->setObjectName(QString::fromUtf8("zskewValue"));
        zskewValue->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(zskewValue);


        gridLayout->addLayout(verticalLayout_3, 0, 2, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        SLApp->setCentralWidget(centralwidget);

        retranslateUi(SLApp);

        QMetaObject::connectSlotsByName(SLApp);
    } // setupUi

    void retranslateUi(QMainWindow *SLApp)
    {
        SLApp->setWindowTitle(QApplication::translate("SLApp", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SLApp", "Noise Threshold", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SLApp", "Z-Scale", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SLApp", "Z-Skew", 0, QApplication::UnicodeUTF8));
        thresholdValue->setText(QApplication::translate("SLApp", "0", 0, QApplication::UnicodeUTF8));
        zscaleValue->setText(QApplication::translate("SLApp", "0", 0, QApplication::UnicodeUTF8));
        zskewValue->setText(QApplication::translate("SLApp", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SLApp: public Ui_SLApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAPP_H
