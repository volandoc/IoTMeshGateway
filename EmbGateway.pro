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
    plugins/include/pluginsapi.h \
    include/plugincontainer.h \
    include/plugincontainerif.h \
    core/include/innerbusapi.h \
    core/include/innerbusclientapi.h \
    include/gwtemplates.h \
    plugins/include/deviceapi.h \
    include/sysdefs.h

LIBS += -lPocoFoundation -lPocoUtil

unix {
    target.path = $$PWD/bin
    INSTALLS += target
}s
