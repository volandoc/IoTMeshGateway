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

SOURCES += src/CloudConnector.cpp \
    src/JSON_messages.cpp \
    src/mqttclient.cpp \
    src/rest.cpp

HEADERS += include/CloudConnector.h \
    include/JSON_messages.h \
    include/mqttclient.h \
    include/mqttclientconfig.h \
    include/rest.h \
    include/restconfig.h \
    ../include/pluginsapi.h \
    ../../core/include/innerbusclientapi.h

LIBS += -lPocoFoundation

unix {
    target.path = $$PWD
    INSTALLS += target
}
