#include "FakeDevicesPlugin.h"
#include <iostream>

FakeDevicesPlugin::FakeDevicesPlugin() {
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    this->pluginDetails.type = _PD_T_DEVICE;
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "FakeDevicesPlugin";
    this->pluginDetails.pluginName ="Fake Devices Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    logger.debug("Plugin Created");
}

FakeDevicesPlugin::~FakeDevicesPlugin(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("Plugin Removed");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
}

int FakeDevicesPlugin::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }

    Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();

    for(int typecount = 0; typecount < DEVICE_TYPES_SIZE; typecount++){
        Poco::UUID dev_serial = generator.createRandom();
        deviceList[typecount] = FakeDeviceFactory::buildFakeDevice(typecount, dev_serial.toString());
        deviceList[typecount]->generateProperties();
    }

    logger.debug("Started");

    return 0;
}

int FakeDevicesPlugin::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    logger.debug("InnerBus client initialized");

    return 0;
}

int FakeDevicesPlugin::setWorkDir(std::string path){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("Running from %s", path);
    this->work_dir = path;
    return 0;
}

int FakeDevicesPlugin::executeCommand(std::string source, IBMessage message){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("executeInternalCommand %s:%s ", source, message.getPayload());

    logger.debug("\"%s : %s : %s : %d\"", message.getId(), message.getPayload(), message.getReference(), (int) message.getTimestamp());

    IBPayload payload;
    if(payload.fromJSON(message.getPayload()))
        logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());

    return 0;
}

int FakeDevicesPlugin::executeInternalCommand(std::string source, std::string message){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    return 0;
}


int FakeDevicesPlugin::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference) {
      return 0;
}

int FakeDevicesPlugin::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("getCommandSet");
    return 0;
}

int FakeDevicesPlugin::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* FakeDevicesPlugin::getPluginDetails(){
    return &pluginDetails;
}

int FakeDevicesPlugin::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
    for(int typecount = 0; typecount < DEVICE_TYPES_SIZE; typecount++){
        if( NULL != deviceList[typecount] ) {
            delete deviceList[typecount];
            deviceList[typecount] = NULL;
        }
    }

    if(this->busClient!=NULL){
        this->busClient->disconnect();
        this->busClient->free();
    } else {
        logger.error("No IBus Client found: something went wrong", __FILE__, 26);
    }
    logger.debug("Stopped");
    return 0;
}
