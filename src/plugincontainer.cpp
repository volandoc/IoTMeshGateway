#include "plugincontainer.h"
//#include <Poco/AutoPtr.h>
//#include <Poco/Util/LoggingConfigurator.h>
//#include <Poco/Util/PropertyFileConfiguration.h>

//Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig = new Poco::Util::PropertyFileConfiguration("../conf/loggerconf.properties");
//Poco::Util::LoggingConfigurator configurator;

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

    //startPlugins();

    return result;
}

void PluginContainer::startPlugins() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    std::map<std::string,std::string>::iterator itCur = this->loadedPlugins.begin();
    std::map<std::string,std::string>::iterator itEnd = this->loadedPlugins.end();

    for (; itCur != itEnd; ++itCur) {
        std::string plgName = itCur->first;
        try {
            UCLPluginIf& tmpPlugin = pluginLoader.instance(plgName);
            tmpPlugin.startPlugin();
            logger.debug("Plugin <%s> started", itCur->first);
        } catch(Poco::Exception excp) {
            logger.log(excp, __FILE__, 48);
        }
    }
}

void PluginContainer::stopPlugins() {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    std::map<std::string,std::string>::iterator itCur = this->loadedPlugins.begin();
    std::map<std::string,std::string>::iterator itEnd = this->loadedPlugins.end();

    for (; itCur != itEnd; ++itCur) {
        std::string plgName = itCur->first;
        try {
            UCLPluginIf& tmpPlugin = pluginLoader.instance(plgName);
            tmpPlugin.stopPlugin();
            logger.debug("Plugin <%s> stoped", itCur->first);
        } catch(Poco::Exception excp) {
            logger.log(excp, __FILE__, 48);
        }
    }
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
            this->loadedPlugins[itMan->name()] = it->first;
        }
    }
}

int PluginContainer::LoadPlugin(std::string pname) {
    Poco::Logger& logger = Poco::Logger::get("PluginContainer");

    try{
        this->pluginLoader.loadLibrary(pname);
    } catch(Poco::Exception excp){
        logger.log(excp, __FILE__, 88);
        return -1;
    }

    if( this->pluginLoader.isLibraryLoaded(pname) ){
        logger.information("Plugin file <%s> loaded",pname);
    } else {
        logger.information("Plugin file <%s> not loaded",pname);
        return -1;
    }

    return 0;
}

std::string PluginContainer::GetPluginsPath() {
    return this->pluginDirPath;
}

std::list<std::string> PluginContainer::GetPluginsList() {
    std::list<std::string> result;
    return result;
}

std::map<std::string, std::string> PluginContainer::GetLoadedPlugins() {
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

    //stopPlugins();

    std::map<std::string,std::string>::iterator itCur = this->loadedPlugins.begin();
    std::map<std::string,std::string>::iterator itEnd = this->loadedPlugins.begin();

    for (; itCur != itEnd; ++itCur) {
        logger.information("Plugin name: %s", itCur->first);
        if( pluginLoader.isLibraryLoaded(itCur->second)) {
            pluginLoader.unloadLibrary(itCur->second);
        }
    }
    return 0;
}
