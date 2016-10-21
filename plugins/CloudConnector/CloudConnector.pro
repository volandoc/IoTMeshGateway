#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T09:58:43
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++11

TARGET = CloudConnector
TEMPLATE = lib

INCLUDEPATH += $$PWD/include \
    $$PWD/../include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/CloudConnector.cpp

HEADERS += include/CloudConnector.h \
    ../include/pluginsapi.h \
    ../../core/include/innerbusclientapi.h

LIBS += -lPocoFoundation

unix {
    target.path = $$PWD
    INSTALLS += target
}
