#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T10:34:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tab2abc
TEMPLATE = app


SOURCES += main.cpp\
        tab2abc.cpp \
    convert.cpp \
    elidedlabel.cpp \
    bassstringline.cpp \
    abcfile.cpp

HEADERS  += tab2abc.h \
    convert.h \
    elidedlabel.h \
    bassstringline.h \
    abcfile.h

FORMS    += tab2abc.ui
