#ifndef LIFX_BULB_PLUGIN_H
#define LIFX_BULB_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Timestamp.h>
#include <Poco/Timer.h>
#include "pluginsapi.h"

#define POLLING_INTERVAL           5000
#define POLLING_START_INTERVAL     0

class LifXBulbPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    Poco::Timer pollingTimer;
    std::string work_dir;

    void doPolling(Poco::Timer &timer);

public:
    LifXBulbPlugin();
    virtual ~LifXBulbPlugin();

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
    POCO_EXPORT_SINGLETON(LifXBulbPlugin)
POCO_END_MANIFEST

#endif // LIFX_BULB_PLUGIN_H
