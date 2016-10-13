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
INCPATH       = -I../EmbGateway/include -I./include -I./plugins/include
DESTDIR       = bin
SOURCES       = main.cpp plugincontainer.cpp route.cpp mqttclient.cpp JSON_messages.cpp
OBJECTS       = main.o plugincontainer.o router.o mqttclient.o JSON_messages.o

BIN_DIR = bin
SUBPROJECT_DIR  = plugins

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
	$(MAKE) -C $(SUBPROJECT_DIR)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

main.o: src/main.cpp include/router.h include/mqttclient.h plugins/include/pluginsapi.h include/easylogging++.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

plugincontainer.o: src/plugincontainer.cpp include/plugincontainer.h include/plugincontainerif.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o plugincontainer.o src/plugincontainer.cpp

mqttclient.o: src/mqttclient.cpp include/mqttclient.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o mqttclient.o src/mqttclient.cpp

router.o: src/router.cpp include/router.h include/mqttclient.h plugins/include/pluginsapi.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o router.o src/router.cpp

JSON_messages.o : src/JSON_messages.cpp include/JSON_messages.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o JSON_messages.o src/JSON_messages.cpp

clean:
	-$(DEL_DIR) $(BIN_DIR)
	-$(DEL_FILE) $(OBJECTS)
	$(MAKE) clean -C $(SUBPROJECT_DIR)
