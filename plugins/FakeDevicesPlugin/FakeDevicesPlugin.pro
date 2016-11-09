#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T15:56:08
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++11

TARGET = FakeDevicesPlugin
TEMPLATE = lib

INCLUDEPATH += $$PWD/include \
    $$PWD/../include \
    $$PWD/../../core/include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/FakeDevicesPlugin.cpp \
    src/FakeDevice.cpp \
    src/FakeDeviceFactory.cpp

HEADERS += include/FakeDevicesPlugin.h \
    ../include/pluginsapi.h \
    ../../core/include/innerbusclientapi.h \
    ../../core/include/ibmessage.h \
    include/device.h \
    include/FakeDevice.h \
    include/FakeDeviceFactory.h

LIBS += -lPocoFoundation

unix {
    target.path = $$PWD
    INSTALLS += target
}
