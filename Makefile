###
# Define Compilers
###
CC            = gcc
CXX           = g++
LINK          = g++

###
# Define compile flags and defines
###
DISTNAME      = EmbedGateway1.0.0
DEFINES       =
CFLAGS        = -c -pipe -O2 -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -c -pipe -O2 -std=gnu++11 -Wall -W -fPIC $(DEFINES)
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS) -lmosquittopp -ldl -lpthread
INCPATH       = -I../EmbGateway -I.
DESTDIR       = bin
SOURCES       = main.cpp gateway.cpp mqttclient.cpp
OBJECTS       = main.o gateway.o mqttclient.o

BIN_DIR = bin
MANAGERS_DIR  = Managers

###
# Define system commands
###
DEL_FILE      = rm -f
COPY_FILE     = cp -f
MOVE_FILE     = mv -f
MKDIR         = mkdir -p
COPY_DIR      = cp -f -R
DEL_DIR       = rm -rf

###
# Define target file names
###
TARGET        = embgateway

###
# BuildRules
###
all: managers $(TARGET)
	-$(DEL_DIR) $(BIN_DIR)
	-$(MKDIR) $(BIN_DIR)
	-$(MOVE_FILE) $(TARGET) $(DESTDIR)

managers:
	$(MAKE) -C $(MANAGERS_DIR)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

ain.o: main.cpp gateway.h mqttclient.h Managers/managersapi.h easylogging++.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mqttclient.o: mqttclient.cpp mqttclient.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o mqttclient.o mqttclient.cpp

gateway.o: gateway.cpp gateway.h mqttclient.h Managers/managersapi.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o gateway.o gateway.cpp

clean:
	-$(DEL_DIR) $(BIN_DIR)
	-$(DEL_FILE) $(OBJECTS)
	$(MAKE) clean -C $(MANAGERS_DIR)
