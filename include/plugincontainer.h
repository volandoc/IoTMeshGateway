#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include <Poco/ClassLoader.h>
#include <Poco/Manifest.h>
#include <Poco/Logger.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Glob.h>

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

    std::map<std::string, std::string> loadedPlugins;
    std::list<UCLPluginIf*> runningPlugins;
    std::map<std::string, int> failedPlugins;

    void generatePluginList();
    int LoadPlugin(std::string pname);
    void startPlugins();
    void stopPlugins();

public:
    PluginContainer(std::string path="../plugins");
    virtual ~PluginContainer();

    virtual int LoadPlugins();
    virtual std::list<std::string> GetPluginsList();
    virtual std::string GetPluginsPath();
    virtual std::map<std::string, std::string> GetLoadedPlugins();
    virtual std::map<std::string, int> GetFailedPlugins();
    virtual int unloadPlugins();
    virtual bool HasLoadErrors();
};

#endif // PLUGINCONTAINER_H
