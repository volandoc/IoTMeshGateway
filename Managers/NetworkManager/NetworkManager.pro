#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T09:58:43
#
#-------------------------------------------------

QT       -= core gui
CONFIG += c++11

TARGET = NetworkManager
TEMPLATE = lib

DEFINES += GW_MANAGERS_EXPORTS

SOURCES += networkmanager.cpp

HEADERS += networkmanager.h\
    ../managersapi.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
