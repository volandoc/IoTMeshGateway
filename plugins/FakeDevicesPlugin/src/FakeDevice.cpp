#include "FakeDevice.h"

FakeDevice::FakeDevice(std::string type, std::string name, std::string serial, std::string description){
    Poco::Logger& logger = Poco::Logger::get("FakeDevices");
    this->type = type;
    this->name = name;
    this->serial = serial;
    this->description = description;

    logger.debug("Fake device(%s) created", this->type);
}

FakeDevice::~FakeDevice(){
    Poco::Logger& logger = Poco::Logger::get("FakeDevices");
    logger.debug("Fake device(%s) destroyed", this->type);
}

std::string FakeDevice::getName(){
    return this->name;
}

std::string FakeDevice::getType(){
    return this->type;
}

std::string FakeDevice::getSerial(){
    return this->serial;
}

std::string FakeDevice::getDescription(){
    return this->description;
}

std::string FakeDevice::getConnectionInfo(){
    return this->connectionInfo;
}

void FakeDevice::processDeviceCommand(std::string command){
    Poco::Logger& logger = Poco::Logger::get("FakeDevices");
    logger.debug("Fake device(%s) command received: %s", this->type, command);
}

Properties FakeDevice::getProperties(){
    return this->properties;
}

void FakeDevice::connect(){

}

void FakeDevice::setProperties(Properties properties){
    Poco::Logger& logger = Poco::Logger::get("FakeDevices");
    this->properties = properties;
    logger.debug("Fake device(%s) properties setted, size %d", this->type, (int)properties.size());
}
