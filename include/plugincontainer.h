#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)

#   define _FILE_SEPARATOR "\\"
#   define _PLUGIN_PREFIX ""
#   define _PLUGIN_EXTENTION ".dll"

#   include <windows.h>
#else

#   define _FILE_SEPARATOR "/"
#   define _PLUGIN_PREFIX "lib"
#   define _PLUGIN_EXTENTION ".so"

#   include <dlfcn.h>
#   include <dirent.h>
#endif

#include <map>
#include "plugincontainerif.h"
#include "pluginsapi.h"
#include "easylogging++.h"

class PluginContainer: public PluginContainerIF {
private:

    const std::string pluginDirPath;
    bool lerror;

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
    std::map<std::string, HMODULE> pluginHandlers;
#else
    std::map<std::string, void *> pluginHandlers;
#endif

    std::map<std::string, ucl::plugins::UCLPluginIf*> loadedPlugins;
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
    virtual std::map<std::string, ucl::plugins::UCLPluginIf*> GetLoadedPlugins();
    virtual std::map<std::string, int> GetFailedPlugins();
    virtual int registerPluginsListener(ucl::plugins::NotificationListenerIF*);
    virtual int unloadPlugins();
    virtual bool HasLoadErrors();
};

#endif // PLUGINCONTAINER_H
