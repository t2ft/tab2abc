#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T10:34:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tab2abc
TEMPLATE = app

TRANSLATIONS = \
    lang/tab2abc_en.ts         # english

SOURCES += main.cpp\
        tab2abc.cpp \
    convert.cpp \
    elidedlabel.cpp \
    bassstringline.cpp \
    abcfile.cpp \
    apptranslator.cpp \
    autoupdatelineedit.cpp

HEADERS  += tab2abc.h \
    convert.h \
    elidedlabel.h \
    bassstringline.h \
    abcfile.h \
    apptranslator.h \
    autoupdatelineedit.h

FORMS    += tab2abc.ui

RESOURCES += \
    tab2abc.qrc

win32 {
    RC_FILE = tab2abc.rc
}

OTHER_FILES += \
    tab2abc.rc
