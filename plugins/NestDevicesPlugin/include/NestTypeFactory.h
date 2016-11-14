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

class NestTypeThermostat: public NestType {
public:
    NestTypeThermostat(std::string token):
        NestType(token) {
        StringVector propertyNames = {
            "humidity",
            "locale",
            "temperature_scale",
            "is_using_emergency_heat",
            "has_fan",
            "software_version",
            "has_leaf",
            "where_id",
            "device_id",
            "name",
            "can_heat",
            "can_cool",
            "target_temperature_c",
            "target_temperature_f",
            "target_temperature_high_c",
            "target_temperature_high_f",
            "target_temperature_low_c",
            "target_temperature_low_f",
            "ambient_temperature_c",
            "ambient_temperature_f",
            "away_temperature_high_c",
            "away_temperature_high_f",
            "away_temperature_low_c",
            "away_temperature_low_f",
            "eco_temperature_high_c",
            "eco_temperature_high_f",
            "eco_temperature_low_c",
            "eco_temperature_low_f",
            "is_locked",
            "locked_temp_min_c",
            "locked_temp_min_f",
            "locked_temp_max_c",
            "locked_temp_max_f",
            "sunlight_correction_active",
            "sunlight_correction_enabled",
            "structure_id",
            "fan_timer_active",
            "fan_timer_timeout",
            "fan_timer_duration",
            "previous_hvac_mode",
            "hvac_mode",
            "time_to_target",
            "time_to_target_training",
            "where_name",
            "label",
            "name_long",
            "is_online",
            "hvac_state",
        };
        setPropertyNames(propertyNames);
    }
    virtual ~NestTypeThermostat(){}

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
            case NEST_TYPE_THERMOSTAT:
                tmpType = new NestTypeThermostat("c.J2QMRqxtk5SHfE7oU5gu9LkDXfQdzWJz0tAxaU18aflh7w59vc5DllMXNlg0Rrq3viFcDReErCzNWys7kGotEK6hkFPqRlAA2r9q7aMbcOpWLVkiUEzwdiLOh8ZFZdKMtY9g53xKaBoASyrq");
                break;

            default: tmpType = NULL;
        }
        return tmpType;
    }
};

#endif // NEST_TYPE_FACTORY_H

