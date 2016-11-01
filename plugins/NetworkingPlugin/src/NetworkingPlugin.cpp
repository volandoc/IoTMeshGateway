#include "NetworkingPlugin.h"
#include <iostream>

NetworkingPlugin::NetworkingPlugin(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "NetworkingPlugin";
    this->pluginDetails.pluginName ="Networking Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    logger.debug("Plugin Created");
}

NetworkingPlugin::~NetworkingPlugin(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
    logger.debug("Plugin Removed");
}

int NetworkingPlugin::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
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

int NetworkingPlugin::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    return 0;
}

int NetworkingPlugin::setWorkDir(std::string path){
    return 0;
}

int NetworkingPlugin::executeCommand(std::string topic, std::string message){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    logger.debug("executeCommand topic {%s} msg{%s}", topic, message);
    return 0;
}

int NetworkingPlugin::sendOccurrence(std::string message) {
      return 0;
}

int NetworkingPlugin::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    logger.debug("getCommandSet");
    return 0;
}

int NetworkingPlugin::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* NetworkingPlugin::getPluginDetails(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    return &pluginDetails;
}

int NetworkingPlugin::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("NetworkingPlugin");
    logger.debug("Stopped");
    return 0;
}
