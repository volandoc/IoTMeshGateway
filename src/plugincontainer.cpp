#include "plugincontainer.h"

int PluginContainer::LoadPlugins() {

#ifdef _DIRENT_H
    DIR* mngrsdir;
    struct dirent *direntry;
#endif

#ifdef _WINDOWS_H
    WIN32_FIND_DATA direntry;
    HANDLE mngrsdir = INVALID_HANDLE_VALUE;
#endif

    std::string name = "";
    int result = 0;

#ifdef _DIRENT_H
    if( (mngrsdir=opendir(this->pluginDirPath.c_str())) != NULL ) {
        while( (direntry=readdir(mngrsdir)) != NULL ) {
            name = direntry->d_name;
            if( (direntry->d_type == DT_DIR) && (name!=".") && (name!="..") ) {
                result = this->LoadPlugin(name);
            }
        }
    } else {
        result = -1;
    }
#endif

#ifdef _WINDOWS_H
    mngrsdir = FindFirstFile(szDir, &direntry);
    if( mngrsdir == INVALID_HANDLE_VALUE ) {
        result = -1;
    } else {
        do {
            if( direntry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                name = direntry.cFileName;
                if( (name!=".") && (name!="..") ) {
                    result = this->LoadPlugin(name);
                }
            }
        } while( FindNextFile(mngrsdir, &direntry) != 0 );

        if ( GetLastError() != ERROR_NO_MORE_FILES ) {
            result = -1;
        }
        FindClose(hFind);
    }
#endif

    return result;
}

std::string PluginContainer::generatePluginUri(std::string pname){
    return this->pluginDirPath + _FILE_SEPARATOR + pname + _FILE_SEPARATOR + _PLUGIN_PREFIX + pname + _PLUGIN_EXTENTION;
}

int PluginContainer::LoadPlugin(std::string pname) {

    std::string pluginURI = generatePluginUri(pname);

#ifdef _WINDOWS_H
    HMODULE dmhndl = LoadLibrary(pluginURI.c_str());
#endif
#ifdef _DLFCN_H
    void *dmhndl = dlopen(pluginURI.c_str(), RTLD_NOW);
#endif

    if( dmhndl == NULL ) {
        LOG(ERROR) << dlerror();
        this->failedPlugins[pname] = -1;
        return -1;
    } else {
        this->pluginHandlers[pname] = dmhndl;
    }

    ucl::plugins::PluginDetails *pluginDetails;
#ifdef _WINDOWS_H
    pluginDetails = reinterpret_cast<ucl::plugins::PluginDetails*> (GetProcAddress(dmhndl, "exportDetails"));
#endif
#ifdef _DLFCN_H
    pluginDetails = reinterpret_cast<ucl::plugins::PluginDetails*> (dlsym(dmhndl, "exportDetails"));
#endif

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

int PluginContainer::registerPluginsListener(ucl::plugins::NotificationListenerIF *listener) {
    std::map<std::string, ucl::plugins::UCLPluginIf*>::iterator iter;
    for(iter=this->loadedPlugins.begin(); iter != this->loadedPlugins.end(); iter++) {
        iter->second->setListener(listener);
    }
}

bool PluginContainer::HasLoadErrors() {
    return this->lerror;
}

int PluginContainer::unloadPlugins(){
    std::map<std::string, void *>::iterator iter;
    for(iter=this->pluginHandlers.begin(); iter != this->pluginHandlers.end(); iter++) {
#ifdef _WINDOWS_H
        FreeLibrary(iter->second)
#endif
#ifdef _DLFCN_H
        dlclose(iter->second);
#endif
    }
    this->loadedPlugins.clear();
    return 0;
}

PluginContainer::~PluginContainer(){

}
