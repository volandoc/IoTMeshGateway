#ifndef NETWORKING_PLUGIN_H
#define NETWORKING_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include "pluginsapi.h"

class NetworkingPlugin: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
public:
    NetworkingPlugin();
    virtual ~NetworkingPlugin();

    virtual int startPlugin();
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(NetworkingPlugin)
POCO_END_MANIFEST

#endif // NETWORKING_PLUGIN_H
