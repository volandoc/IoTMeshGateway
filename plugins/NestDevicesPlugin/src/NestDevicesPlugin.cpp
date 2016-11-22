#include "NestDevicesPlugin.h"
#include "discoveredDevInfo.h"
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
        typeList[typecount] = NestTypeFactory::buildNestType(typecount, this->work_dir);
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
    if(payload.fromJSON(message.getPayload())) {
        logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());
        if ("command" == payload.getType()) {
            if ("GET" == payload.getValue()) {
                if ("LIST" == payload.getCvalue()) {
                    std::string devicesList;
                    for(int typecount = 0; typecount < NEST_DEVICE_TYPES_SIZE; typecount++) {
                        Devices devices = typeList[typecount]->getDevices();

                        for (DevicesIterator it = devices.begin(); it != devices.end(); ++it) {
                            DiscoveredDevInfo devInfo;
                            static int i = 0;

                            devInfo.setSerial(it->first);

                            if (NEST_TYPE_CAMERA == typecount) {
                                std::string buffer;
                                buffer = "NestCameraName" + std::to_string(i);
                                devInfo.setName(buffer);

                                buffer = "NestCameraDescription" + std::to_string(i);
                                devInfo.setDescription(buffer);

                                buffer = "Nest_Camera";
                                devInfo.setType(buffer);
                            }

                            if (NEST_TYPE_THERMOSTAT == typecount) {
                                std::string buffer;
                                buffer = "NestThermostatName" + std::to_string(i);
                                devInfo.setName(buffer);
                            
                                buffer = "NestThermostatDescription" + std::to_string(i);
                                devInfo.setDescription(buffer);
                            
                                buffer = "Nest_Thermostat";
                                devInfo.setType(buffer);
                            }
                            ++i;
                            if (0 != devicesList.length()) {
                                devicesList += ",\n";
                            }
                            devicesList += devInfo.toJSON();
                        }
                    }

                    devicesList = Poco::replace(devicesList, "\"", "*");
                    devicesList = Poco::replace(devicesList, "*", "\\\"");
                    sendOccurrence(true, "LIST", devicesList, message.getId());
                }
            }
        }
    }

    return 0;
}

int NestDevicesPlugin::executeInternalCommand(std::string source, std::string message) {
    Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    return 0;
}

int NestDevicesPlugin::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference) {
    IBPayload payload;
    payload.setType("occurence");
    payload.setValue((success?"SUCCESS":"FAILED"));
    payload.setCvalue(cvalue);
    payload.setContent(content);

    IBMessage ibmessage;
    Poco::Timestamp now;
    ibmessage.setId(pluginDetails.pluginName);
    ibmessage.setPayload(payload.toJSON());
    ibmessage.setReference(reference);
    ibmessage.setTimestamp(now.epochTime());

    busClient->sendMessage(ibmessage);

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

