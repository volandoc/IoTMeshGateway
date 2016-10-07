###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      = NetworkingPlugin1.0.0
DEFINES       = -DUCL_PLUGINS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,NetworkingPlugin.so.1
INCPATH       = -I../include -I./include
LIBS          = $(SUBLIBS) -lpthread 
DESTDIR       = 
SOURCES       = src/NetworkingPlugin.cpp 
OBJECTS       = NetworkingPlugin.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
TARGET        = NetworkingPlugin.so.1.0.0
TARGETA       = NetworkingPlugin.a
TARGET0       = NetworkingPlugin.so
TARGETD       = NetworkingPlugin.so.1.0.0
TARGET1       = NetworkingPlugin.so.1
TARGET2       = NetworkingPlugin.so.1.0


###
# BuildRules
###
all: $(TARGET) staticlib

$(TARGET):  $(OBJECTS)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	-$(SYMLINK) $(TARGET) $(TARGET0)
	-$(SYMLINK) $(TARGET) $(TARGET1)
	-$(SYMLINK) $(TARGET) $(TARGET2)

staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

NetworkingPlugin.o: src/NetworkingPlugin.cpp include/NetworkingPlugin.h \
		../include/pluginsapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NetworkingPlugin.o src/NetworkingPlugin.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)