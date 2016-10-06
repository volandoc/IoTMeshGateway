#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T15:56:08
#
#-------------------------------------------------

QT       -= core gui
CONFIG += c++11

TARGET = DeviceManager
TEMPLATE = lib

DEFINES += GW_MANAGERS_EXPORTS

SOURCES += devicemanager.cpp

HEADERS += devicemanager.h \
    ../managersapi.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
