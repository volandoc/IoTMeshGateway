#ifndef LIFX_BULB_PLUGIN_H
#define LIFX_BULB_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Timestamp.h>
#include <Poco/Timer.h>
#include "pluginsapi.h"
#include "messaging.h"

#define POLLING_INTERVAL           5000
#define POLLING_START_INTERVAL     0
#define LISTENER_INTERVAL          1000
#define LISTENER_START_INTERVAL    0


class LifXBulbPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    Poco::Timer pollingTimer;
    Poco::Timer listenerTimer;
    std::string work_dir;
    LifxMessageFactory messageFactory = LifxMessageFactory("ap0");

    void doPolling(Poco::Timer &timer);
    void listenUDP(Poco::Timer &timer);
    int proccessDeviceSetCommand(std::string content, std::string device_id);

public:
    LifXBulbPlugin();
    virtual ~LifXBulbPlugin();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string source, IBMessage message);
    virtual int executeInternalCommand(std::string source, std::string message);
    virtual int sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference, std::string sender="");
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(LifXBulbPlugin)
POCO_END_MANIFEST

#endif // LIFX_BULB_PLUGIN_H
