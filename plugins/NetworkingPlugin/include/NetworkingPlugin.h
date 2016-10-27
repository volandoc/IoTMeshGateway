#ifndef NETWORKING_PLUGIN_H
#define NETWORKING_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include "pluginsapi.h"

class NetworkingPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
public:
    NetworkingPlugin();
    virtual ~NetworkingPlugin();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string message);
    virtual int sendOccurrence(std::string message);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(NetworkingPlugin)
POCO_END_MANIFEST

#endif // NETWORKING_PLUGIN_H
