#-------------------------------------------------
#
# Project created by QtCreator 2013-11-09T17:11:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gps_test_0618_1405
TEMPLATE = app

#DEPENDPATH += .
#DEPENDPATH += . ./images
#INCLUDEPATH += .


SOURCES += main.cpp\
        mainwindow.cpp \
    gps_nmea.cpp \
    utc_convert.cpp

HEADERS  += mainwindow.h \
    gps_nmea.h

FORMS    += mainwindow.ui
    target.path += /home/root
    INSTALLS += target

RESOURCES +=  \
    gps_resouce.qrc
