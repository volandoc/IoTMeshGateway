#ifndef NEST_TYPE_FACTORY_H
#define NEST_TYPE_FACTORY_H

#include <memory>
#include <array>
#include <Poco/Logger.h>
#include "NestType.h"

#define DEFAULT_TOKEN       "c.fVTpnlRYq0KNSg4auBywyh9IErpa8UNDrRwKN8MJEQfIAyOw9dCfuswqa45D3YqAbOCsbelorPGfIlfd2i8JLTa1uowuJKgO3wGT1zIXSoLpegm2bky3Gk5JpG3FCnWoqmCpFHcQ7OIJnvEQ"

enum {
    NEST_TYPE_CAMERA,
    NEST_TYPE_THERMOSTAT,

    NEST_DEVICE_TYPES_SIZE
};


class NestTypeCamera: public NestType {
public:
    NestTypeCamera(std::string token, std::string work_dir):
        NestType(token, work_dir) {
    }
    virtual ~NestTypeCamera(){}

    void init();
    void initCapabilities();
    void setStrDeviceProperty(std::string serial, std::string propertyName, std::string propertyValue);
};


class NestTypeThermostat: public NestType {
public:
    NestTypeThermostat(std::string token, std::string work_dir):
        NestType(token, work_dir) {
    }
    virtual ~NestTypeThermostat(){}

    void init();
    void initCapabilities();
    void setStrDeviceProperty(std::string serial, std::string propertyName, std::string propertyValue);
};


class NestTypeFactory {
public:
    static NestType* buildNestType(int typecnt, std::string work_dir){
        Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
        logger.debug("Creating (%d) nest devices type list", typecnt);
        NestType* tmpType;
        switch(typecnt){
            case NEST_TYPE_CAMERA:
                tmpType = new NestTypeCamera(DEFAULT_TOKEN, work_dir);
                break;
            case NEST_TYPE_THERMOSTAT:
                tmpType = new NestTypeThermostat(DEFAULT_TOKEN, work_dir);
                break;

            default: tmpType = NULL;
        }
        return tmpType;
    }
};

#endif // NEST_TYPE_FACTORY_H

