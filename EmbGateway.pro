TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mqttclient.cpp \
    gateway.cpp \

HEADERS += \
    easylogging++.h \
    gateway.h \
    mqttclient.h \
    gwtemplates.h \
    Managers/managersapi.h \

LIBS += -lmosquittopp -ldl
