#-------------------------------------------------
#
# Project created by QtCreator 2023-10-19T15:29:25
#
#-------------------------------------------------

QT       += core gui sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test10_checkface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    historydialog.cpp \
    managerfactory.cpp \
    mytime.cpp \
    userdao.cpp

HEADERS  += mainwindow.h \
    historydialog.h \
    managerfactory.h \
    mytime.h \
    userdao.h

FORMS    += mainwindow.ui \
    historydialog.ui

INCLUDEPATH += E:/opencv/path/opencv3.4-install/install/include
INCLUDEPATH += E:/opencv/path/opencv3.4-install/install/include/opencv
INCLUDEPATH += E:/opencv/path/opencv3.4-install/install/include/opencv2
LIBS += E:/opencv/path/opencv3.4-install/install/x86/mingw/lib/libopencv_*.a
