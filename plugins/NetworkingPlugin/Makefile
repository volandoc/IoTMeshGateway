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
INCPATH       = -I../include -I./include -I../../core/include
LIBS          = $(SUBLIBS) -lpthread -lPocoFoundation
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
TARGET        = libNetworkingPlugin.so.1.0.0
TARGETA       = libNetworkingPlugin.a
TARGET0       = libNetworkingPlugin.so
TARGETD       = libNetworkingPlugin.so.1.0.0
TARGET1       = libNetworkingPlugin.so.1
TARGET2       = libNetworkingPlugin.so.1.0


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
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NetworkingPlugin.o src/NetworkingPlugin.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)