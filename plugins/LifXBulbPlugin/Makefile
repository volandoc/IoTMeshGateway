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
LIBS          = $(SUBLIBS) -lpthread -lPocoFoundation
DESTDIR       = 
SOURCES       = src/LifXBulbPlugin.cpp 
OBJECTS       = LifXBulbPlugin.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs

###
# Define target file names
###
TARGET        = libLifXBulbPlugin.so.1.0.0
TARGETA       = libLifXBulbPlugin.a
TARGET0       = libLifXBulbPlugin.so
TARGETD       = libLifXBulbPlugin.so.1.0.0
TARGET1       = libLifXBulbPlugin.so.1
TARGET2       = libLifXBulbPlugin.so.1.0


###
# BuildRules
###
all: $(TARGET) staticlib

$(TARGET):  $(OBJECTS)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)

staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

LifXBulbPlugin.o: src/LifXBulbPlugin.cpp include/LifXBulbPlugin.h \
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o LifXBulbPlugin.o src/LifXBulbPlugin.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)