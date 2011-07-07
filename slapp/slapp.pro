#-------------------------------------------------
#
# Project created by QtCreator 2011-07-04T23:16:35
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = slapp
TEMPLATE = app

LIBS += -L/usr/lib -lcv -lcvaux -lcxcore -lm -lhighgui -lm -lstdc++  -lGL -lglut -lm -lGLU
INCLUDEPATH += ../ /usr/include/opencv  /usr/include

SOURCES += main.cpp\
        slapp.cpp\
        ../three_step_phase_shift.cpp\
        glpointcloud.cpp \
    capturedialog.cpp

HEADERS  += slapp.h glpointcloud.h \
    capturedialog.h

FORMS    += slapp.ui \
    capturedialog.ui
