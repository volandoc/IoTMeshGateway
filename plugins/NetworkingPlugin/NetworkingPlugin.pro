#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T09:58:43
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++11

TARGET = NetworkingPlugin
TEMPLATE = lib

INCLUDEPATH += $$PWD/include \
    $$PWD/../include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/NetworkingPlugin.cpp

HEADERS += include/NetworkingPlugin.h\
    ../include/pluginsapi.h

unix {
    target.path = $$PWD
    INSTALLS += target
}
