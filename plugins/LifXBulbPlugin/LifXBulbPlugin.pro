#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T15:56:08
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++11

TARGET = LifXBulbPlugin
TEMPLATE = lib

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/LifXBulbPlugin.cpp

HEADERS += include/LifXBulbPlugin.h \
    ../include/pluginsapi.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
