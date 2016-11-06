#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T11:26:13
#
#-------------------------------------------------

QT       += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWebCam
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
QTPLUGIN += qjpeg

SOURCES += test_webcam.cpp \
    mainwindow.cpp \
    simpleflow.cpp \
    myglwidget.cpp \
    listmodelrecord.cpp \
    listrecords.cpp \
    record.cpp \
    confetti.cpp

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2412 \
    -lopencv_highgui2412 \
    -lopencv_imgproc2412 \
    -lopencv_features2d2412 \
    -lopencv_calib3d2412 \
    -lopencv_video2412
FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    simpleflow.h \
    myglwidget.h \
    listmodelrecord.h \
    listrecords.h \
    record.h \
    confetti.h

win32:LIBS += -lopengl32
win32:LIBS += -lglu32



