#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T00:05:41
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ingestor
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
        dialogcreateproject.cpp \
        dialogdelplugin.cpp \
        dialoginstrumentmanager.cpp \
        dialognetworkset.cpp \
        dialognewinstrument.cpp \
        dialognewplugin.cpp \
        main.cpp \
        mainwindow.cpp \
        networkdata.cpp

HEADERS += \
        dialogcreateproject.h \
        dialogdelplugin.h \
        dialoginstrumentmanager.h \
        dialognetworkset.h \
        dialognewinstrument.h \
        dialognewplugin.h \
        instrumentcommander.h \
        instrumentdescriptor.h \
        instrumentdumper.h \
        instrumentingestor.h \
        instrumentprocessor.h \
        mainwindow.h \
        networkdata.h

FORMS += \
        dialogcreateproject.ui \
        dialogdelplugin.ui \
        dialoginstrumentmanager.ui \
        dialognetworkset.ui \
        dialognewinstrument.ui \
        dialognewplugin.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RC_ICONS = Ingestor.ico


TRANSLATIONS = gLanguage_cn.ts gLanguage_en.ts

RESOURCES += \
    img.qrc
