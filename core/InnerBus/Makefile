###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      =  InnerBus
DEFINES       = -DUCL_CORE_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,InnerBus
INCPATH       = -I./include -I../include -I../../plugins/include
LIBS          = $(SUBLIBS) -lmosquitto -lpthread -lPocoFoundation -lPocoUtil
DESTDIR       = 
SOURCES       = src/InnerBus.cpp 
OBJECTS       = InnerBus.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs

###
# Define target file names
###
TARGET        = InnerBus.so

###
# BuildRules
###
all: $(TARGET)

$(TARGET):  $(OBJECTS)  
	-$(DEL_FILE) $(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

InnerBus.o: src/InnerBus.cpp include/InnerBus.h \
		../include/innerbusapi.h ../../plugins/include/pluginsapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o InnerBus.o src/InnerBus.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(OBJECTS)