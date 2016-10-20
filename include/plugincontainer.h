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

typedef Poco::ClassLoader<UCLPluginIf> PluginLoader;
typedef Poco::Manifest<UCLPluginIf> PluginManifest;

class PluginContainer: public PluginContainerIF {
private:

    PluginLoader pluginLoader;
    const std::string pluginDirPath;
    bool lerror;

    std::map<std::string, std::string> pluginsList;
    std::list<std::string> loadedPlugins;
    std::map<std::string, int> failedPlugins;

    void generatePluginList();
    int LoadPlugin(std::string pname);

public:
    PluginContainer(std::string path="../plugins");
    virtual ~PluginContainer();

    virtual int LoadPlugins();
    virtual int addIBusClients(InnerBusIF& ibus);
    virtual void startPlugins();
    virtual void stopPlugins();
    virtual std::map<std::string, std::string> GetPluginsList();
    virtual std::string GetPluginsPath();
    virtual std::list<std::string> GetLoadedPlugins();
    virtual std::map<std::string, int> GetFailedPlugins();
    virtual int unloadPlugins();
    virtual bool HasLoadErrors();
};

#endif // PLUGINCONTAINER_H
