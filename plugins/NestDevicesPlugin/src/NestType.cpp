#include "NestType.h"
#include "rest.h"
#include "sysdefs.h"
#include <sstream>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Exception.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>


#define HOST_REST_TEMP_FILENAME     "temp.txt"
#define HOST_REST_GET_FILENAME      "get.txt"


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;
using namespace Poco::Util;


NestType::NestType(std::string type, std::string token, std::string work_dir) {
    this->type = type;
    this->token = token;
    this->work_dir = work_dir;
}

NestType::~NestType() {
}

void NestType::init() {
    requestSerialsList();

    Properties properties;
    std::string propertyValue;

    Poco::Logger& logger = Poco::Logger::get("NestType");

    for (unsigned int i = 0; i < serials.size(); ++i) {
        logger.debug("Serial[%u] = %s", i, serials[i]);
        properties.clear();
        for (unsigned int j = 0; j < this->capabilities.size(); ++j) {
            propertyValue = parseStrDeviceProperty(serials[i], this->capabilities[j].getName());
            properties[j] = propertyValue;
            logger.debug("     %s = %s", this->capabilities[j].getName(), propertyValue);
        }
        addDevice(serials[i], properties);
    }
}

void NestType::requestSerialsList() {
    std::string buffer;
    Poco::Logger& logger = Poco::Logger::get("NestType");

    rest rst;
    rst.setMethod(REST_METHOD_GET);

    buffer = "\"Authorization:Bearer ";
    buffer += this->token;
    buffer += "\"";
    rst.setHeader(buffer);

    rst.setConsoleOutputFile(HOST_REST_TEMP_FILENAME);

    rst.setCloudResponseFile(HOST_REST_GET_FILENAME);

    buffer = REST_NEST_HOST;
    buffer += URL_DEVICES_PATH;
    buffer += this->type;
    buffer += "/";
    rst.setUrl(buffer);

    this->devicesJson = rst.execute();

    if (0 != this->devicesJson.length()) {
        Parser parser;
        Var result = parser.parse(this->devicesJson);
        Object::Ptr object = result.extract<Object::Ptr>();
        object->getNames(this->serials);
    } else {
        logger.debug("Failed to get devices");
    }
}

std::string NestType::requestDeviceProperties(std::string serial) {
    std::string buffer;
    Poco::Logger& logger = Poco::Logger::get("NestType");

    std::string properties;

    rest rst;
    rst.setMethod(REST_METHOD_GET);

    buffer = "\"Authorization:Bearer ";
    buffer += this->token;
    buffer += "\"";
    rst.setHeader(buffer);

    rst.setConsoleOutputFile(HOST_REST_TEMP_FILENAME);

    rst.setCloudResponseFile(HOST_REST_GET_FILENAME);

    buffer = REST_NEST_HOST;
    buffer += URL_DEVICES_PATH;
    buffer += this->type;
    buffer += "/";
    buffer += serial;
    buffer += "/";
    rst.setUrl(buffer);

    properties = rst.execute();

    return properties;
}



std::string NestType::requestStrDeviceProperty(std::string serial, std::string propertyName) {
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_GET);

    buffer = "\"Authorization:Bearer ";
    buffer += this->token;
    buffer += "\"";
    rst.setHeader(buffer);

    rst.setConsoleOutputFile(HOST_REST_TEMP_FILENAME);

    rst.setCloudResponseFile(HOST_REST_GET_FILENAME);

    buffer = REST_NEST_HOST;
    buffer += URL_DEVICES_PATH;
    buffer += this->type;
    buffer += "/";
    buffer += serial;
    buffer += "/";
    buffer += propertyName;
    buffer += "/";
    rst.setUrl(buffer);

    std::string propertyValue = rst.execute();

    return propertyValue;
}

std::string NestType::parseStrDeviceProperty(std::string serial, std::string propertyName) {
    Poco::Logger& logger = Poco::Logger::get("NestType");
    std::string propertyValue;

    if (0 != this->devicesJson.length()) {
        Parser parser;
        Var result = parser.parse(this->devicesJson);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var deviceInfo = object->get(serial);
        object = deviceInfo.extract<Object::Ptr>();
        Var varPropertyValue = object->get(propertyName);
        propertyValue = varPropertyValue.convert<std::string>();
    } else {
        logger.debug("Failed to parse property %s", propertyName);
    }

    return propertyValue;
}

void NestType::setStrDeviceProperty(std::string serial, std::string propertyName, std::string propertyValue) {
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_PUT);

    buffer = "\"Authorization:Bearer ";
    buffer += this->token;
    buffer += "\"";
    rst.setHeader(buffer);

    rst.setConsoleOutputFile(HOST_REST_TEMP_FILENAME);

    rst.setCloudResponseFile(HOST_REST_GET_FILENAME);

    buffer = REST_NEST_HOST;
    buffer += URL_DEVICES_PATH;
    buffer += this->type;
    buffer += "/";
    buffer += serial;
    buffer += "/";
    buffer += propertyName;
    buffer += "/";
    rst.setUrl(buffer);

    rst.setDataBody(propertyValue);

    rst.execute();
}


Devices NestType::getDevices() {
    return this->devices;
}

void NestType::setDevices(Devices devices) {
    this->devices = devices;
}

void NestType::addDevice(std::string serial, Properties properties) {
    this->devices[serial] = properties;
}

Capabilities NestType::getCapabilities() {
    return this->capabilities;
}

void NestType::setCapabilities(Capabilities capabilities) {
    this->capabilities = capabilities;
}

void NestType::addCapability(unsigned int id, CapabilityInfo capability) {
    this->capabilities[id] = capability;
}

std::string NestType::getToken() {
    return this->token;
}

void NestType::setToken(std::string token) {
    this->token = token;
}

void NestType::initCapabilities() {
Poco::Logger& logger = Poco::Logger::get("NestType");

    std::string fileName = getWorkDir();
    fileName += _FILE_SEPARATOR;
    fileName += CONFIG_CAPABILITIES_FLIENAME;

    try
    {
        AutoPtr<XMLConfiguration> pConf(new XMLConfiguration(fileName));

        std::stringstream attribPath;
        unsigned int counter = 0;
        CapabilityInfo tempCapability;
        std::string value;

        try {
            while(1) {
                tempCapability.clear();

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@name]";
                value = pConf->getString(attribPath.str());
                tempCapability.setName(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@type]";
                value = pConf->getString(attribPath.str());
                tempCapability.setType(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@constrain_type]";
                value = pConf->getString(attribPath.str());
                tempCapability.setConstrainType(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@constraints_json]";
                value = pConf->getString(attribPath.str());
                tempCapability.setConstraintsJson(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@default_value]";
                value = pConf->getString(attribPath.str());
                tempCapability.setDefaultValue(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@description]";
                value = pConf->getString(attribPath.str());
                tempCapability.setDescription(value);

                attribPath.str(std::string());
                attribPath << this->type << ".Capability[" << counter<< "]" << "[@read_write]";
                value = pConf->getString(attribPath.str());
                tempCapability.setReadWrite(value);

                addCapability(counter, tempCapability);
                ++counter;
            }
        } catch(NotFoundException& e) {
        }
    }
    catch (...)
    {
        logger.debug("Config File %s not found", fileName);
        return;
    }
}

std::string NestType::getWorkDir() {
    return this->work_dir;
}

