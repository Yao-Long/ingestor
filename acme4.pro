#-------------------------------------------------
#
# Project created by QtCreator 2019-07-03T19:52:57
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = acme4
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        acmeinstrument.cpp \
        acmeplugin.cpp \
        dialogcreateproject.cpp \
        dialogdelplugin.cpp \
        dialognewinstrument.cpp \
        dialognewplugin.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        acmeinstrument.h \
        acmeplugin.h \
        dialogcreateproject.h \
        dialogdelplugin.h \
        dialognewinstrument.h \
        dialognewplugin.h \
        instrumentcommander.h \
        instrumentdescriptor.h \
        instrumentdumper.h \
        instrumentingestor.h \
        instrumentprocessor.h \
        mainwindow.h

FORMS += \
        dialogcreateproject.ui \
        dialogdelplugin.ui \
        dialognewinstrument.ui \
        dialognewplugin.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

DISTFILES +=


RC_ICONS = th.ico

