###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      = DeviceManager1.0.0
DEFINES       = -DGW_MANAGERS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,libDeviceManager.so.1
INCPATH       = -I../DeviceManager -I.
LIBS          = -lpthread 
DESTDIR       = 
SOURCES       = devicemanager.cpp 
OBJECTS       = devicemanager.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs

###
# Define target file names
###
TARGET        = libDeviceManager.so.1.0.0
TARGETA       = libDeviceManager.a
TARGET0       = libDeviceManager.so
TARGETD       = libDeviceManager.so.1.0.0
TARGET1       = libDeviceManager.so.1
TARGET2       = libDeviceManager.so.1.0


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

devicemanager.o: devicemanager.cpp devicemanager.h \
		../managersapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o devicemanager.o devicemanager.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)