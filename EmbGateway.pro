TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/include \
    $$PWD/plugins/include \
    $$PWD/core/include

SOURCES += src/main.cpp \
    src/plugincontainer.cpp

HEADERS += \
    include/easylogging++.h \
    include/templates.h \
    plugins/include/pluginsapi.h \
    include/plugincontainer.h \
    include/plugincontainerif.h \
    core/include/innerbusapi.h \
    core/include/innerbusclientapi.h

LIBS += -lmosquittopp -ldl -lPocoFoundation

unix {
    target.path = $$PWD/bin
    INSTALLS += target
}s
