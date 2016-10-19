#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include <Poco/ClassLoader.h>
#include <Poco/Manifest.h>
#include <map>
#include "plugincontainerif.h"
#include "pluginsapi.h"
#include "easylogging++.h"

typedef Poco::ClassLoader<UCLPluginIf> PluginLoader;
typedef Poco::Manifest<UCLPluginIf> PluginManifest;

class PluginContainer: public PluginContainerIF {
private:

    PluginLoader pluginLoader;

    const std::string pluginDirPath;
    bool lerror;

    std::map<std::string, UCLPluginIf*> loadedPlugins;
    std::map<std::string, int> failedPlugins;

    std::string generatePluginUri(std::string pname);
    int LoadPlugin(std::string pname);

public:
    PluginContainer(std::string path="../plugins"): pluginDirPath(path), lerror(false){

    }
    virtual ~PluginContainer();

    virtual int LoadPlugins();
    virtual std::list<std::string> GetPluginsList();
    virtual std::string GetPluginsPath();
    virtual std::map<std::string, UCLPluginIf*> GetLoadedPlugins();
    virtual std::map<std::string, int> GetFailedPlugins();
    virtual int unloadPlugins();
    virtual bool HasLoadErrors();
};

#endif // PLUGINCONTAINER_H
