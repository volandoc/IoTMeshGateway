#include "plugincontainer.h"

PluginContainer::PluginContainer(std::string path): pluginDirPath(path), lerror(false) {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");
    logger.debug("PluginContainer created");
}

PluginContainer::~PluginContainer(){
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");
    logger.debug("PluginContainer destroyed");
}

int PluginContainer::LoadPlugins() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    int result = 0;
    Poco::File pluginDir(this->pluginDirPath);

    if( !pluginDir.exists() && !pluginDir.isDirectory() ) {
        return -2;
    }

    std::set<std::string> files;
    Poco::Glob::glob(this->pluginDirPath + "/*/*.so",files);
    std::set<std::string>::iterator it = files.begin();

    for (; it != files.end(); ++it) {
        logger.information(*it);
        this->LoadPlugin(*it);
    }

    generatePluginList();

    return result;
}

int PluginContainer::addIBusClients(InnerBusIF& ibus){
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");
    logger.debug("IBus instance Rceived");

    std::map<std::string,std::string>::iterator itCur = this->pluginsList.begin();
    std::map<std::string,std::string>::iterator itEnd = this->pluginsList.end();

    for (; itCur != itEnd; ++itCur) {
        std::string plgName = itCur->first;
        try {
            UCLPluginIf& tmpPlugin = pluginLoader.instance(plgName);
            int rc = tmpPlugin.setIBusClient(ibus.createIBusClient());
            if( rc == 0 ){
                logger.debug("Plugin <%s> ", plgName);
            } else {
                logger.debug("Plugin <%s> failed", plgName);
            }
        } catch(Poco::Exception excp) {
            logger.log(excp, __FILE__, 48);
        }
    }

    return 0;
}

void PluginContainer::startPlugins() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    std::map<std::string,std::string>::iterator itCur = this->pluginsList.begin();
    std::map<std::string,std::string>::iterator itEnd = this->pluginsList.end();

    for (; itCur != itEnd; ++itCur) {
        std::string plgName = itCur->first;
        try {
            UCLPluginIf& tmpPlugin = pluginLoader.instance(plgName);
            int rc = tmpPlugin.startPlugin();
            if( rc == 0 ){
                logger.debug("Plugin <%s> started", plgName);
                loadedPlugins.push_back(itCur->first);
            } else {
                logger.debug("Plugin <%s> failed", plgName);
                std::string fpIndex = itCur->first + ":" + itCur->second;
                failedPlugins[fpIndex] = -2;
            }
        } catch(Poco::Exception excp) {
            logger.log(excp, __FILE__, 48);
        }
    }
}

void PluginContainer::stopPlugins() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    std::list<std::string>::iterator itCur = this->loadedPlugins.begin();
    std::list<std::string>::iterator itEnd = this->loadedPlugins.end();

    for (; itCur != itEnd; ++itCur) {
        std::string plgName = *itCur;
        try {
            UCLPluginIf& tmpPlugin = pluginLoader.instance(plgName);
            tmpPlugin.stopPlugin();
            logger.debug("Plugin <%s> stoped", plgName);
        } catch(Poco::Exception excp) {
            logger.log(excp, __FILE__, 48);
        }
    }
    loadedPlugins.clear();
}

void PluginContainer::generatePluginList() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    PluginLoader::Iterator it(pluginLoader.begin());
    PluginLoader::Iterator end(pluginLoader.end());

    for (; it != end; ++it) {
        logger.debug("lib path: %s", it->first);
        PluginManifest::Iterator itMan(it->second->begin());
        PluginManifest::Iterator endMan(it->second->end());
        for (; itMan != endMan; ++itMan) {
            logger.information(itMan->name());
            this->pluginsList[itMan->name()] = it->first;
            try {
                pluginLoader.instance(itMan->name());
            } catch(Poco::Exception excp) {
                logger.log(excp, __FILE__, 48);
            }

        }
    }
}

int PluginContainer::LoadPlugin(std::string pname) {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    try{
        this->pluginLoader.loadLibrary(pname);
    } catch(Poco::Exception excp){
        logger.log(excp, __FILE__, 88);
        failedPlugins[pname] = -1;
        return -1;
    }

    if( this->pluginLoader.isLibraryLoaded(pname) ){
        logger.information("Plugin file <%s> loaded",pname);
    } else {
        logger.information("Plugin file <%s> not loaded",pname);
        failedPlugins[pname] = -1;
        return -1;
    }

    return 0;
}

std::string PluginContainer::GetPluginsPath() {
    return this->pluginDirPath;
}

std::map<std::string, std::string> PluginContainer::GetPluginsList() {
    return this->pluginsList;
}

std::list<std::string> PluginContainer::GetLoadedPlugins() {
    return this->loadedPlugins;
}

std::map<std::string, int> PluginContainer::GetFailedPlugins() {
    return this->failedPlugins;
}

bool PluginContainer::HasLoadErrors() {
    return this->lerror;
}

int PluginContainer::unloadPlugins(){
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    stopPlugins();

    std::map<std::string,std::string>::iterator itCur = this->pluginsList.begin();
    std::map<std::string,std::string>::iterator itEnd = this->pluginsList.end();

    for (; itCur != itEnd; ++itCur) {
        if( pluginLoader.isLibraryLoaded(itCur->second)) {
            pluginLoader.unloadLibrary(itCur->second);
            logger.information("Plugins in <%s> unloaded", itCur->second);
        } else {
            logger.debug("<%s> not loaded", itCur->second);
        }
    }

    pluginsList.clear();

    return 0;
}
