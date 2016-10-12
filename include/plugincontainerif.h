#ifndef PLUGINCONTAINERIF_H
#define PLUGINCONTAINERIF_H

#include <list>
#include <string>
#include <map>
#include "pluginsapi.h"

class PluginContainerIF {
public:
    virtual int LoadPlugins()=0;
    virtual std::list<std::string> GetPluginsList()=0;
    virtual std::string GetPluginsPath()=0;
    virtual std::map<std::string, ucl::plugins::UCLPluginIf*> GetLoadedPlugins()=0;
    virtual std::map<std::string, int> GetFailedPlugins()=0;
    virtual int registerPluginsListener(ucl::plugins::NotificationListenerIF*) = 0;
    virtual int unloadPlugins()=0;
    virtual bool HasLoadErrors()=0;
};

#endif // PLUGINCONTAINERIF_H
