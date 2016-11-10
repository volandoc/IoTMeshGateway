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
    $$PWD/../include \
    $$PWD/../../core/include

DEFINES += UCL_PLUGINS_EXPORTS

SOURCES += src/CloudConnector.cpp \
    src/JSON_messages.cpp \
    src/mqttclient.cpp \
    src/gwCommand.cpp \
    src/parseCommand.cpp \
    src/rest.cpp

HEADERS += include/CloudConnector.h \
    include/JSON_messages.h \
    include/mqttclient.h \
    include/mqttclientconfig.h \
    include/rest.h \
    include/restconfig.h \
    include/gwCommand.h \
    include/parseCommand.h \
    ../include/pluginsapi.h \
    ../../core/include/innerbusclientapi.h

LIBS += -lmosquittopp -lPocoFoundation -lPocoJSON

unix {
    target.path = $$PWD
    INSTALLS += target
}
