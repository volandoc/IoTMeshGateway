###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      =  NestDevicesPlugin1.0.0
DEFINES       = -DUCL_PLUGINS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,NestDevicesPlugin.so.1
INCPATH       = -I./include -I../include -I../../include -I../../core/include
LIBS          = $(SUBLIBS) -lpthread -lPocoFoundation  -lPocoJSON
DESTDIR       = 
SOURCES       = src/rest.cpp src/NestDevicesPlugin.cpp src/NestTypeFactory.cpp src/NestType.cpp
OBJECTS       = rest.o NestType.o NestTypeFactory.o NestDevicesPlugin.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
TARGET        = libNestDevicesPlugin.so.1.0.0
TARGETA       = libNestDevicesPlugin.a
TARGET0       = libNestDevicesPlugin.so
TARGETD       = libNestDevicesPlugin.so.1.0.0
TARGET1       = libNestDevicesPlugin.so.1
TARGET2       = libNestDevicesPlugin.so.1.0


###
# BuildRules
###
all: $(TARGET) staticlib

$(TARGET):  $(OBJECTS)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	$(SYMLINK) $(TARGET) $(TARGET0)
	$(SYMLINK) $(TARGET) $(TARGET1)
	$(SYMLINK) $(TARGET) $(TARGET2)

staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

rest.o: src/rest.cpp include/rest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rest.o src/rest.cpp

NestType.o: src/NestType.cpp include/NestType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NestType.o src/NestType.cpp

NestTypeFactory.o: src/NestTypeFactory.cpp include/NestTypeFactory.h include/NestType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NestTypeFactory.o src/NestTypeFactory.cpp

NestDevicesPlugin.o: src/NestDevicesPlugin.cpp include/NestDevicesPlugin.h \
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NestDevicesPlugin.o src/NestDevicesPlugin.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)
