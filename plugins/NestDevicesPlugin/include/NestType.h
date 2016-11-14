#ifndef NEST_DEVICES_TYPE_H
#define NEST_DEVICES_TYPE_H

#include <Poco/Logger.h>
#include "device.h"

#define REST_NEST_HOST      "https://developer-api.nest.com/"
#define URL_DEVICES_PATH    "devices/"

typedef std::map<std::string, Properties> Devices;
typedef std::map<std::string, Properties>::iterator DevicesIterator;
typedef std::vector<std::string> StringVector;

class NestType{
private:
    std::string token;
    std::string devicesJson;

    StringVector serials;
    Devices devices;
    StringVector propertyNames;

    void requestSerialsList(std::string nestType);

    std::string requestStrDeviceProperty(std::string nestType, std::string serial, std::string propertyName);

    std::string parseStrDeviceProperty(std::string serial, std::string propertyName);

    void setStrDeviceProperty(std::string nestType, std::string serial, std::string propertyName, std::string propertyValue);

public:
    NestType(std::string token);
    virtual ~NestType();
    
    virtual void init() = 0;
    void init(std::string nestType);


    Devices getDevices();
    void setDevices(Devices devices);
    void addDevice(std::string serial, Properties properties);

    StringVector getPropertyNames();
    void setPropertyNames(StringVector propertyNames);

    std::string getToken();
    void setToken(std::string token);
};

#endif // NEST_DEVICES_TYPE_H
