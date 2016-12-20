#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T15:56:08
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++11

TARGET = NestDevicesPlugin
TEMPLATE = lib

INCLUDEPATH += $$PWD/include \
    $$PWD/../include \
    $$PWD/../../core/include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/NestDevicesPlugin.cpp \
    src/NestType.cpp \
    src/NestTypeFactory.cpp \
    src/rest.cpp

HEADERS += include/NestDevicesPlugin.h \
    ../include/pluginsapi.h \
    ../include/deviceapi.h \
    ../../core/include/innerbusclientapi.h \
    include/NestType.h \
    include/NestTypeFactory.h \
    include/restconfig.h \
    include/rest.h

LIBS += -lPocoFoundation

unix {
    target.path = $$PWD
    INSTALLS += target
}
