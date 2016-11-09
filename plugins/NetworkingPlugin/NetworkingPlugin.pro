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
    $$PWD/../include \
    $$PWD/../../core/include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/NetworkingPlugin.cpp

HEADERS += include/NetworkingPlugin.h \
    ../include/pluginsapi.h \
    ../../core/include/innerbusclientapi.h

LIBS += -lPocoFoundation

unix {
    target.path = $$PWD
    INSTALLS += target
}
