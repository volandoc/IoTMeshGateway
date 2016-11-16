#ifndef NEST_TYPE_FACTORY_H
#define NEST_TYPE_FACTORY_H

#include <memory>
#include <array>
#include <Poco/Logger.h>
#include "NestType.h"

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
};


class NestTypeThermostat: public NestType {
public:
    NestTypeThermostat(std::string token, std::string work_dir):
        NestType(token, work_dir) {
    }
    virtual ~NestTypeThermostat(){}

    void init();
    void initCapabilities();
};


class NestTypeFactory {
public:
    static NestType* buildNestType(int typecnt, std::string work_dir){
        Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
        logger.debug("Creating (%d) nest devices type list", typecnt);
        NestType* tmpType;
        switch(typecnt){
            case NEST_TYPE_CAMERA:
                tmpType = new NestTypeCamera("c.J2QMRqxtk5SHfE7oU5gu9LkDXfQdzWJz0tAxaU18aflh7w59vc5DllMXNlg0Rrq3viFcDReErCzNWys7kGotEK6hkFPqRlAA2r9q7aMbcOpWLVkiUEzwdiLOh8ZFZdKMtY9g53xKaBoASyrq", work_dir);
                break;
            case NEST_TYPE_THERMOSTAT:
                tmpType = new NestTypeThermostat("c.J2QMRqxtk5SHfE7oU5gu9LkDXfQdzWJz0tAxaU18aflh7w59vc5DllMXNlg0Rrq3viFcDReErCzNWys7kGotEK6hkFPqRlAA2r9q7aMbcOpWLVkiUEzwdiLOh8ZFZdKMtY9g53xKaBoASyrq", work_dir);
                break;

            default: tmpType = NULL;
        }
        return tmpType;
    }
};

#endif // NEST_TYPE_FACTORY_H

