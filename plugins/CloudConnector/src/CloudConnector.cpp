#include "CloudConnector.h"
#include <iostream>

CloudConnector::CloudConnector(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "CloudConnector";
    this->pluginDetails.pluginName ="CloudConnector Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    logger.debug("Plugin Created");
}

CloudConnector::~CloudConnector(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
    logger.debug("Plugin Removed");
}

int CloudConnector::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
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

int CloudConnector::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    return 0;
}

int CloudConnector::executeCommand(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("executeCommand");
    return 0;
}

int CloudConnector::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("getCommandSet");
    return 0;
}

int CloudConnector::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* CloudConnector::getPluginDetails(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    return &pluginDetails;
}

int CloudConnector::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Stopped");
    return 0;
}
