#include "NestDevicesPlugin.h"
#include <iostream>

NestDevicesPlugin::NestDevicesPlugin() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    this->pluginDetails.type = _PD_T_DEVICE;
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "NestDevicesPlugin";
    this->pluginDetails.pluginName ="Nest Devices Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    logger.debug("Plugin Created");
}

NestDevicesPlugin::~NestDevicesPlugin() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("Plugin Removed");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
}

int NestDevicesPlugin::startPlugin() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }

    for(int typecount = 0; typecount < NEST_DEVICE_TYPES_SIZE; typecount++) {
        typeList[typecount] = NestTypeFactory::buildNestType(typecount);
        typeList[typecount]->init();
    }

    logger.debug("Started");

    return 0;
}

int NestDevicesPlugin::setIBusClient(InnerBusClientIF* client) {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    logger.debug("InnerBus client initialized");

    return 0;
}

int NestDevicesPlugin::setWorkDir(std::string path) {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("Running from %s", path);
    this->work_dir = path;
    return 0;
}

int NestDevicesPlugin::executeCommand(std::string source, IBMessage message) {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("executeInternalCommand %s:%s ", source, message.getPayload());

    logger.debug("\"%s : %s : %s : %d\"", message.getId(), message.getPayload(), message.getReference(), (int) message.getTimestamp());

    IBPayload payload;
    if(payload.fromJSON(message.getPayload()))
        logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());

    return 0;
}

int NestDevicesPlugin::executeInternalCommand(std::string source, std::string message) {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    return 0;
}

int NestDevicesPlugin::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference) {
    return 0;
}

int NestDevicesPlugin::getCommandSet() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("getCommandSet");
    return 0;
}

int NestDevicesPlugin::getCapabilitiesSet() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* NestDevicesPlugin::getPluginDetails() {
    return &pluginDetails;
}

int NestDevicesPlugin::stopPlugin() {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    for(int typecount = 0; typecount < NEST_DEVICE_TYPES_SIZE; ++typecount) {
        if( NULL != typeList[typecount] ) {
            delete typeList[typecount];
            typeList[typecount] = NULL;
        }
    }

    if(this->busClient!=NULL) {
        this->busClient->disconnect();
        this->busClient->free();
    } else {
        logger.error("No IBus Client found: something went wrong", __FILE__, 26);
    }
    logger.debug("Stopped");
    return 0;
}
