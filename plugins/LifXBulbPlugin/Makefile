###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      =  LifXBulbPlugin1.0.0
DEFINES       = -DUCL_PLUGINS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,LifXBulbPlugin.so.1
INCPATH       = -I./include -I../include -I../../core/include
LIBS          = -lPocoFoundation -lPocoNet
DESTDIR       = 
SOURCES       = src/LifXBulbPlugin.cpp src/messaging.cpp
OBJECTS       = LifXBulbPlugin.o messaging.o

APPNAME       =  LifxBulbApp
APPDEFINES    =
APPCFLAGS     = -c -pipe -O2 -Wall -W -fPIC $(APPDEFINES)
APPCXXFLAGS   = -c -pipe -O2 -std=gnu++11 -Wall -W -fPIC $(APPDEFINES)
APPLFLAGS     = -Wl,-O1
APPLIBS       = $(LIBS) -lPocoUtil -lPocoJSON
APPOBJECTS    = LifXBulbApp.o $(OBJECTS)

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
APPTARGET     = lifxbulbapp
TARGET        = libLifXBulbPlugin.so.1.0.0
TARGETA       = libLifXBulbPlugin.a
TARGET0       = libLifXBulbPlugin.so
TARGETD       = libLifXBulbPlugin.so.1.0.0
TARGET1       = libLifXBulbPlugin.so.1
TARGET2       = libLifXBulbPlugin.so.1.0


###
# BuildRules
###
all: $(TARGET) staticlib $(APPTARGET)

$(TARGET): $(OBJECTS)
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	$(SYMLINK) $(TARGET) $(TARGET0)
	$(SYMLINK) $(TARGET) $(TARGET1)
	$(SYMLINK) $(TARGET) $(TARGET2)

staticlib: $(TARGETA)

$(TARGETA): $(OBJECTS)
	-$(DEL_FILE) $(TARGETA)
	$(AR) $(TARGETA) $(OBJECTS)

$(APPTARGET): $(APPOBJECTS)
	-$(DEL_FILE) $(APPTARGET)
	$(LINK) $(APPLFLAGS) -o $(APPTARGET) $(APPOBJECTS) $(APPLIBS)

LifXBulbPlugin.o: src/LifXBulbPlugin.cpp include/LifXBulbPlugin.h \
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o LifXBulbPlugin.o src/LifXBulbPlugin.cpp

messaging.o: src/messaging.cpp include/messaging.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messaging.o src/messaging.cpp

LifXBulbApp.o: src/LifXBulbApp.cpp
	$(CXX) -c $(APPCXXFLAGS) $(INCPATH) -o LifXBulbApp.o src/LifXBulbApp.cpp

clean:
	-$(DEL_FILE) $(APPTARGET) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(APPOBJECTS)