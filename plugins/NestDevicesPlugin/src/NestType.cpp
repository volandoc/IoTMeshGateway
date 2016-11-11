#include "NestType.h"
#include "rest.h"
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>


#define HOST_REST_TEMP_FILENAME     "temp.txt"
#define HOST_REST_GET_FILENAME      "get.txt"


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;


NestType::NestType(std::string token) {
    this->token = token;
}

NestType::~NestType() {
}

void NestType::init(std::string nestType) {
    requestSerialsList(nestType);

    Properties properties;
    std::string propertyValue;

    for (unsigned int i = 0; i < serials.size(); ++i) {
        properties.clear();
        for (unsigned int j = 0; j < this->propertyNames.size(); ++j) {
            propertyValue = parseStrDeviceProperty(serials[i], this->propertyNames[j]);
            properties[j] = propertyValue;
        }
        addDevice(serials[i], properties);
    }
}

void NestType::requestSerialsList(std::string nestType) {
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
    buffer += nestType;
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

std::string NestType::requestStrDeviceProperty(std::string nestType, std::string serial, std::string propertyName) {
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
    buffer += nestType;
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

void NestType::setStrDeviceProperty(std::string nestType, std::string serial, std::string propertyName, std::string propertyValue) {
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
    buffer += nestType;
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

StringVector NestType::getPropertyNames()
{
    return propertyNames;
}

void NestType::setPropertyNames(StringVector propertyNames)
{
    this->propertyNames = propertyNames;
}


std::string NestType::getToken() {
    return this->token;
}

void NestType::setToken(std::string token) {
    this->token = token;
}

