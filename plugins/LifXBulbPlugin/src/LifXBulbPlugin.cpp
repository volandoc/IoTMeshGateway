#include "LifXBulbPlugin.h"
#include <iostream>

LifXBulbPlugin::LifXBulbPlugin() {
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    this->pluginDetails.type = _PD_T_DEVICE;
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "LifXBulbPlugin";
    this->pluginDetails.pluginName ="LifXBulb Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    logger.debug("Plugin Created");
}

LifXBulbPlugin::~LifXBulbPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("Plugin Removed");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
}

int LifXBulbPlugin::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
        logger.debug("Started");
        return 0;
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }
}

int LifXBulbPlugin::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();
    return 0;
}

int LifXBulbPlugin::setWorkDir(std::string path){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("Running from %s", path);
    this->work_dir = path;
    return 0;
}

int LifXBulbPlugin::executeCommand(std::string message){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("executeCommand %s: ", message);
    return 0;
}

int LifXBulbPlugin::sendOccurrence(std::string message) {
      return 0;
}

int LifXBulbPlugin::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("getCommandSet");
    return 0;
}

int LifXBulbPlugin::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* LifXBulbPlugin::getPluginDetails(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    return &pluginDetails;
}

int LifXBulbPlugin::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("Stopped");
    return 0;
}
