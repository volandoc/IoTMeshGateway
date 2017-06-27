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
LIBS          = $(SUBLIBS) -lPocoFoundation -lPocoUtil -lPocoJSON
INCPATH       = -I../EmbGateway/include -I./include -I./plugins/include -I./core/include
DESTDIR       = bin
SOURCES       = main.cpp plugincontainer.cpp
OBJECTS       = main.o plugincontainer.o

BIN_DIR = bin
SUBPROJECT_DIR  = plugins
CORE_DIR = core

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
all: corelibs pluginlibs $(TARGET) install

corelibs:
	$(MAKE) -C $(CORE_DIR)

pluginlibs:
	$(MAKE) -C $(SUBPROJECT_DIR)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

main.o: src/main.cpp plugins/include/pluginsapi.h core/include/innerbusapi.h include/sysdefs.h	
	$(CXX) $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

plugincontainer.o: src/plugincontainer.cpp include/plugincontainer.h include/plugincontainerif.h
	$(CXX) $(CXXFLAGS) $(INCPATH) -o plugincontainer.o src/plugincontainer.cpp

install:
	-$(DEL_DIR) $(BIN_DIR)
	-$(MKDIR) $(BIN_DIR)
	-$(MOVE_FILE) $(TARGET) $(DESTDIR)
	$(MAKE) install -C $(SUBPROJECT_DIR)

clean:
	-$(DEL_DIR) $(BIN_DIR)
	-$(DEL_FILE) $(OBJECTS)
	$(MAKE) clean -C $(CORE_DIR)
	$(MAKE) clean -C $(SUBPROJECT_DIR)
