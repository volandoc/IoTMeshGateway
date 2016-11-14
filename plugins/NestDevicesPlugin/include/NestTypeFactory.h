#ifndef NEST_TYPE_FACTORY_H
#define NEST_TYPE_FACTORY_H

#include <memory>
#include <array>
#include <Poco/Logger.h>
#include "NestType.h"

enum {
    NEST_TYPE_CAMERA,

    NEST_DEVICE_TYPES_SIZE
};


class NestTypeCamera: public NestType {
public:
    NestTypeCamera(std::string token):
        NestType(token) {
        StringVector propertyNames = {
            "name",
            "software_version",
            "where_id",
            "device_id",
            "structure_id",
            "is_online",
            "is_streaming",
            "is_audio_input_enabled",
            "last_is_online_change",
            "is_video_history_enabled",
            "name_long",
            "web_url",
            "app_url",
        };
        setPropertyNames(propertyNames);
    }
    virtual ~NestTypeCamera(){}

    void init();
};

class NestTypeFactory {
public:
    static NestType* buildNestType(int typecnt){
        Poco::Logger& logger = Poco::Logger::get("NestDevicesPlugin");
        logger.debug("Creating (%d) nest devices type list", typecnt);
        NestType* tmpType;
        switch(typecnt){
            case NEST_TYPE_CAMERA:
                tmpType = new NestTypeCamera("c.J2QMRqxtk5SHfE7oU5gu9LkDXfQdzWJz0tAxaU18aflh7w59vc5DllMXNlg0Rrq3viFcDReErCzNWys7kGotEK6hkFPqRlAA2r9q7aMbcOpWLVkiUEzwdiLOh8ZFZdKMtY9g53xKaBoASyrq");
                break;
            default: tmpType = NULL;
        }
        return tmpType;
    }
};

#endif // NEST_TYPE_FACTORY_H
