#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T10:34:55
#
#-------------------------------------------------

QT       -= core gui
CONFIG += c++11

TARGET = InnerBus
TEMPLATE = lib

INCLUDEPATH += $$PWD/include \
    $$PWD/../include \
    $$PWD/../../plugins/include

DEFINES += IBUS_EXPORT

SOURCES += src/InnerBus.cpp

HEADERS += include/InnerBus.h \
        ../include/innerbusapi.h \
        ../include/innerbusclientapi.h \
        ../../plugins/include/pluginsapi.h

LIBS += -lmosquittopp -lPocoFoundation -lPocoUtil

unix {
    target.path = $$PWD
    INSTALLS += target
}
