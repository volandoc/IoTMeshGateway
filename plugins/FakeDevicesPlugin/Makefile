###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      =  FakeDevicesPlugin1.0.0
DEFINES       = -DUCL_PLUGINS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,FakeDevicesPlugin.so.1
INCPATH       = -I./include -I../include -I../../core/include
LIBS          = $(SUBLIBS) -lPocoFoundation
DESTDIR       = 
SOURCES       = src/FakeDevicesPlugin.cpp src/FakeDeviceFactory.cpp src/FakeDevice.cpp
OBJECTS       = FakeDevice.o FakeDeviceFactory.o FakeDevicesPlugin.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
TARGET        = libFakeDevicesPlugin.so.1.0.0
TARGETA       = libFakeDevicesPlugin.a
TARGET0       = libFakeDevicesPlugin.so
TARGETD       = libFakeDevicesPlugin.so.1.0.0
TARGET1       = libFakeDevicesPlugin.so.1
TARGET2       = libFakeDevicesPlugin.so.1.0


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

FakeDevice.o: src/FakeDevice.cpp include/FakeDevice.h include/device.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FakeDevice.o src/FakeDevice.cpp

FakeDeviceFactory.o: src/FakeDeviceFactory.cpp include/FakeDeviceFactory.h include/FakeDevice.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FakeDeviceFactory.o src/FakeDeviceFactory.cpp

FakeDevicesPlugin.o: src/FakeDevicesPlugin.cpp include/FakeDevicesPlugin.h \
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FakeDevicesPlugin.o src/FakeDevicesPlugin.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)