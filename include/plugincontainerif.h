#ifndef PLUGINCONTAINERIF_H
#define PLUGINCONTAINERIF_H

#include <list>
#include <string>
#include <map>
#include "pluginsapi.h"
#include "innerbusapi.h"

class PluginContainerIF {
public:
    virtual int LoadPlugins()=0;
    virtual int addIBusClients(InnerBusIF& ibus)=0;
    virtual void startPlugins()=0;
    virtual void stopPlugins()=0;
    virtual std::map<std::string, std::string> GetPluginsList()=0;
    virtual std::string GetPluginsPath()=0;
    virtual std::list<std::string> GetLoadedPlugins()=0;
    virtual std::map<std::string, int> GetFailedPlugins()=0;
    virtual int unloadPlugins()=0;
    virtual bool HasLoadErrors()=0;
};

#endif // PLUGINCONTAINERIF_H
