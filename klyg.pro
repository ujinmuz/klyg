#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T14:40:52
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = klyg
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    glwi.cpp \
    contour.cpp \
    contourcollection.cpp \
    l19maxwell.cpp

HEADERS  += mainwindow.h \
    glwi.h \
    contour.h \
    contourcollection.h \
    l19maxwell.h

RESOURCES += \
    res.qrc
