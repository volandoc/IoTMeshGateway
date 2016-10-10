#include "plugincontainer.h"

int PluginContainer::LoadPlugins() {
    DIR* mngrsdir;
    struct dirent *direntry;
    std::string name = "";
    std::string pluginpath;
    int result = 0;
    if( (mngrsdir=opendir(this->pluginDirPath.c_str())) != NULL ) {
        while( (direntry=readdir(mngrsdir)) != NULL ) {
            name = direntry->d_name;
            if( (direntry->d_type == DT_DIR) && (name!=".") && (name!="..") ) {
                pluginpath = generatePluginUri(name);
                result = this->LoadPlugin(pluginpath, name);
            }
        }
    } else {
        result=-1;
    }
    return result;
}

std::string PluginContainer::generatePluginUri(std::string pname){
    return this->pluginDirPath + "/" + pname + "/lib" + pname + ".so";
}

int PluginContainer::LoadPlugin(std::string pathToPlugin, std::string pname) {
    void *dmhndl = dlopen(pathToPlugin.c_str(), RTLD_NOW);
    if( dmhndl == NULL ) {
        LOG(ERROR) << dlerror();
        this->failedPlugins[pname] = -1;
        return -1;
    } else {
        this->pluginHandlers[pname] = dmhndl;
    }

    ucl::plugins::PluginDetails *pluginDetails;
    pluginDetails = reinterpret_cast<ucl::plugins::PluginDetails*> (dlsym(dmhndl, "exportDetails"));

    LOG(INFO) << "Plugin Info: "
              << "\n\tAPI Version: " << pluginDetails->apiVersion
              << "\n\tClass Name: " << pluginDetails->className
              << "\n\tPlugin Name: " << pluginDetails->pluginName
              << "\n\tPlugin Version: " << pluginDetails->pluginVersion
              << std::endl;
    // API Version checking
    if( pluginDetails->apiVersion != UCL_PLUGINS_API_VERSION ) {
        this->failedPlugins[pname] = 1;
        return 1;
    }

    // Instantiate the plugin
    auto pluginInstance = reinterpret_cast<ucl::plugins::UCLPluginIf*>(pluginDetails->initializeFunc());
    if( pluginInstance == NULL ) {
        this->failedPlugins[pname] = 2;
        return 2;
    }

    this->loadedPlugins[pname] = pluginInstance;

    return 0;
}

std::string PluginContainer::GetPluginsPath() {
    return this->pluginDirPath;
}

std::list<std::string> PluginContainer::GetPluginsList() {
    std::list<std::string> result;
    return result;
}

std::map<std::string, ucl::plugins::UCLPluginIf*> PluginContainer::GetLoadedPlugins() {
    return this->loadedPlugins;
}

std::map<std::string, int> PluginContainer::GetFailedPlugins() {
    return this->failedPlugins;
}

bool PluginContainer::HasLoadErrors() {
    return this->lerror;
}

int PluginContainer::unloadPlugins(){
    std::map<std::string, void *>::iterator iter;
    for(iter=this->pluginHandlers.begin(); iter != this->pluginHandlers.end(); iter++){
        dlclose(iter->second);
    }
    this->loadedPlugins.clear();
    return 0;
}

PluginContainer::~PluginContainer(){

}
