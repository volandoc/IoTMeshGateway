###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      = NetworkManager1.0.0
DEFINES       = -DGW_MANAGERS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,libNetworkManager.so.1
INCPATH       = -I../NetworkManager -I.
LIBS          = $(SUBLIBS) -lpthread 
DESTDIR       = 
SOURCES       = networkmanager.cpp 
OBJECTS       = networkmanager.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
TARGET        = libNetworkManager.so.1.0.0
TARGETA       = libNetworkManager.a
TARGET0       = libNetworkManager.so
TARGETD       = libNetworkManager.so.1.0.0
TARGET1       = libNetworkManager.so.1
TARGET2       = libNetworkManager.so.1.0


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

networkmanager.o: networkmanager.cpp networkmanager.h \
		../managersapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o networkmanager.o networkmanager.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)