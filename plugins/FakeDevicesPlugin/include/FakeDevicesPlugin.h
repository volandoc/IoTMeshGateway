#ifndef FAKE_DEVICES_PLUGIN_H
#define FAKE_DEVICES_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Task.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include "pluginsapi.h"
#include "FakeDeviceFactory.h"

class FakeDevicesPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    std::string work_dir;
    FakeDevice* deviceList[DEVICE_TYPES_SIZE];

public:
    FakeDevicesPlugin();
    virtual ~FakeDevicesPlugin();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string source, IBMessage message);
    virtual int executeInternalCommand(std::string source, std::string message);
    virtual int sendOccurrence(std::string message);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(FakeDevicesPlugin)
POCO_END_MANIFEST

#endif // FAKE_DEVICES_PLUGIN_H
