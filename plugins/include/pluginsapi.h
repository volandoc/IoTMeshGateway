#ifndef PLUGINSAPI_H
#define PLUGINSAPI_H

#include <string>
#include <Poco/Task.h>
#include "innerbusclientapi.h"

#define UCL_PLUGINS_API_VERSION 1

#define _PD_T_DEVICE "device";
#define _PD_T_COMM   "comm";

static const char *PLUGIN_COMMANDS[] = { "GET", "SET", "UPDATE", "REBOOT", "LIST" };

struct Capability {
    const char* name;
    const char* type;
    const char* constrain_type;
    const char* constraints_json;
    const char* default_value;
    const char* description;
    const char* read_write;
};

struct PluginDetails {
    int apiVersion;
    const char* type;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
};

class UCLPluginIf {
public:
    virtual ~UCLPluginIf(){};
    virtual int startPlugin()=0;
    virtual int setIBusClient(InnerBusClientIF* client) = 0;
    virtual int setWorkDir(std::string path)=0;
    virtual int executeCommand(std::string message)=0;
    virtual int sendOccurrence(std::string message)=0;
    virtual int getCommandSet()=0;
    virtual int getCapabilitiesSet()=0;
    virtual int stopPlugin()=0;
    virtual PluginDetails* getPluginDetails()=0;

};

#endif
