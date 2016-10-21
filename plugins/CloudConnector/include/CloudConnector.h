#ifndef CLOUD_CONNECTOR_PLUGIN_H
#define CLOUD_CONNECTOR_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include "pluginsapi.h"

class CloudConnector: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
public:
    CloudConnector();
    virtual ~CloudConnector();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(CloudConnector)
POCO_END_MANIFEST

#endif // CLOUD_CONNECTOR_PLUGIN_H