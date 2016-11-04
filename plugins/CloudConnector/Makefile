###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      = CloudConnector1.0.0
DEFINES       = -DUCL_PLUGINS_EXPORTS
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
LFLAGS        = -shared -Wl,-soname,CloudConnector.so.1
INCPATH       = -I../include -I./include -I../../core/include
LIBS          = $(SUBLIBS) -lmosquittopp -lPocoFoundation -lPocoJSON
DESTDIR       = 
SOURCES       = src/CloudConnector.cpp mqttclient.cpp rest.cpp JSON_messages.cpp
OBJECTS       = CloudConnector.o mqttclient.o rest.o JSON_messages.o parseCommand.o gwCommand.o

###
# Define system commands
###
DEL_FILE      = rm -f
AR            = ar cqs
SYMLINK       = ln -f -s

###
# Define target file names
###
TARGET        = libCloudConnector.so.1.0.0
TARGETA       = libCloudConnector.a
TARGET0       = libCloudConnector.so
TARGETD       = libCloudConnector.so.1.0.0
TARGET1       = libCloudConnector.so.1
TARGET2       = libCloudConnector.so.1.0


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

CloudConnector.o: src/CloudConnector.cpp include/CloudConnector.h \
		../include/pluginsapi.h ../../core/include/innerbusclientapi.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CloudConnector.o src/CloudConnector.cpp

mqttclient.o: src/mqttclient.cpp include/mqttclient.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mqttclient.o src/mqttclient.cpp

rest.o: src/rest.cpp include/rest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rest.o src/rest.cpp

JSON_messages.o: src/JSON_messages.cpp include/JSON_messages.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o JSON_messages.o src/JSON_messages.cpp

parseCommand.o: src/parseCommand.cpp include/parseCommand.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parseCommand.o src/parseCommand.cpp

gwCommand.o: src/gwCommand.cpp include/gwCommand.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gwCommand.o src/gwCommand.cpp

clean:
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA) $(OBJECTS)
