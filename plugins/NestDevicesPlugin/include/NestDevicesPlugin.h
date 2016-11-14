#ifndef NEST_DEVICES_PLUGIN_H
#define NEST_DEVICES_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Task.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include "pluginsapi.h"
#include "NestTypeFactory.h"

class NestDevicesPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    std::string work_dir;
    NestType* typeList[NEST_DEVICE_TYPES_SIZE];

public:
    NestDevicesPlugin();
    virtual ~NestDevicesPlugin();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string source, IBMessage message);
    virtual int executeInternalCommand(std::string source, std::string message);
    virtual int sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(NestDevicesPlugin)
POCO_END_MANIFEST

#endif // NEST_DEVICES_PLUGIN_H
