#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include <dlfcn.h>
#include <map>
#include <dirent.h>
#include "plugincontainerif.h"
#include "pluginsapi.h"
#include "easylogging++.h"

class PluginContainer: public PluginContainerIF {
private:
    const std::string pluginDirPath;
    bool lerror;
    std::map<std::string, void *> pluginHandlers;
    std::map<std::string, ucl::plugins::UCLPluginIf*> loadedPlugins;
    std::map<std::string, int> failedPlugins;

    std::string generatePluginUri(std::string pname);
    int LoadPlugin(std::string pathToPlugin, std::string pname);

public:
    PluginContainer(std::string path="../plugins"): pluginDirPath(path), lerror(false){

    }
    virtual ~PluginContainer();

    virtual int LoadPlugins();
    virtual std::list<std::string> GetPluginsList();
    virtual std::string GetPluginsPath();
    virtual std::map<std::string, ucl::plugins::UCLPluginIf*> GetLoadedPlugins();
    virtual std::map<std::string, int> GetFailedPlugins();
    virtual int unloadPlugins();
    virtual bool HasLoadErrors();
};

#endif // PLUGINCONTAINER_H
