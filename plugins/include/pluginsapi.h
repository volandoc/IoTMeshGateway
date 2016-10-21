#ifndef PLUGINSAPI_H
#define PLUGINSAPI_H

#include <string>
#include "innerbusclientapi.h"

#define UCL_PLUGINS_API_VERSION 1

struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
};

class UCLPluginIf {
public:
    virtual ~UCLPluginIf(){};
    virtual int startPlugin()=0;
    virtual int setIBusClient(InnerBusClientIF* client) = 0;
    virtual int executeCommand()=0;
    virtual int sendOccurrence(std::string message)=0;
    virtual int getCommandSet()=0;
    virtual int getCapabilitiesSet()=0;
    virtual int stopPlugin()=0;
    virtual PluginDetails* getPluginDetails()=0;

};

#endif
