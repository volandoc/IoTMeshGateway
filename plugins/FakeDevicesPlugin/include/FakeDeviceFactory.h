#ifndef FAKE_DEVICE_FACTORY_H
#define FAKE_DEVICE_FACTORY_H

#include <memory>
#include <array>
#include <Poco/Logger.h>
#include "FakeDevice.h"

#define DEFAULT_FAKE_DEVICE     0
#define LIFX_TEMPERATURE_BULB   1
#define HOME_SECURITY_SENSOR    2
#define ROUTING_ALARM_SENSOR    3
#define ROUTING_BINARY_SENSOR   4
#define SECURE_KEYPAD_DOOR_LOCK 5
#define LIGHT_DIMMER_SWITCH     6
#define ZIGBEE_BRIGHTNESS_BULB  7
#define BINARY_SCENE_SWITCH     8

#define DEVICE_TYPES_SIZE       9

//static const std::string FakeDeviceTypes[DEVICE_TYPES_SIZE] = {
//                                           "Default_Fake_Device",
//                                           "LIFX_bulb_temperature",
//                                           "Home Security Sensor",
//                                           "Routing Alarm Sensor",
//                                           "Routing Binary Sensor",
//                                           "Secure Keypad Door Lock",
//                                           "Light Dimmer Switch",
//                                           "ZigBee_brightness_bulb",
//                                           "Binary Scene Switch" };


class FakeDefaultDevice: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11001;
    const long STATE_CAPABILITY_ID             = 11002;
public:
    FakeDefaultDevice(std::string serial):
        FakeDevice("Default_Fake_Device", "Fake Device", serial, "Default Fake Device") {

    }
    virtual ~FakeDefaultDevice(){}

    void generateProperties();
};

class FakeLifxTemperatureBulb: public FakeDevice {
private:
    const long STATE_CAPABILITY_ID             = 10701;
    const long BRIGHTNESS_CAPABILITY_ID        = 10702;
    const long COLOR_TEMPERATURE_CAPABILITY_ID = 10703;
    const long CONNECTION_CAPABILITY_ID        = 10704;
public:
    FakeLifxTemperatureBulb(std::string serial):
        FakeDevice("LIFX_bulb_temperature", "LIFX temperature bulb", serial, "Fake LIFX temperature bulb") {

    }
    virtual ~FakeLifxTemperatureBulb(){}

    void generateProperties();
};

class FakeHomeSecuritySensor: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11301;
    const long STATE_CAPABILITY_ID             = 11302;
    const long BATTERY_CAPABILITY_ID           = 11303;
    const long TEMPERATURE_CAPABILITY_ID       = 11304;
    const long LUMINANCE_CAPABILITY_ID         = 11305;
    const long SENSITIVITY_CAPABILITY_ID       = 11307;
public:
    FakeHomeSecuritySensor(std::string serial):
        FakeDevice("Home Security Sensor", "Home Security Sensor", serial, "Fake Home Security Sensor") {

    }
    virtual ~FakeHomeSecuritySensor(){}

    void generateProperties();
};

class FakeRoutingAlarmSensor: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11101;
    const long STATE_CAPABILITY_ID             = 11102;
    const long BATTERY_CAPABILITY_ID           = 11103;
    const long TEMPERATURE_CAPABILITY_ID       = 11104;
public:
    FakeRoutingAlarmSensor(std::string serial):
        FakeDevice("Routing Alarm Sensor", "Routing Alarm Sensor", serial, "Fake Routing Alarm Sensor") {

    }
    virtual ~FakeRoutingAlarmSensor(){}

    void generateProperties();

};

class FakeRoutingBinarySensor: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11201;
    const long STATE_CAPABILITY_ID             = 11202;
    const long BATTERY_CAPABILITY_ID           = 11203;
public:
    FakeRoutingBinarySensor(std::string serial):
        FakeDevice("Routing Binary Sensor", "Routing Alarm Sensor", serial, "Fake Routing Alarm Sensor") {

    }
    virtual ~FakeRoutingBinarySensor(){}

    void generateProperties();

};

class FakeKeypadDoorLock: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11601;
    const long STATE_CAPABILITY_ID             = 11602;
    const long BATTERY_CAPABILITY_ID           = 11603;
    const long LAST_UNLOCKED_USER_ID           = 11604;
public:
    FakeKeypadDoorLock(std::string serial):
        FakeDevice("Secure Keypad Door Lock", "Secure Keypad Door Lock", serial, "Fake Secure Keypad Door Lock") {

    }
    virtual ~FakeKeypadDoorLock(){}

    void generateProperties();
};

class FakeLightDimmerSwitch: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11501;
    const long STATE_CAPABILITY_ID             = 11502;
    const long COLOR_CAPABILITY_ID             = 11503;
public:
    FakeLightDimmerSwitch(std::string serial):
        FakeDevice("Light Dimmer Switch", "Light Dimmer Switch", serial, "Fake Light Dimmer Switch") {

    }
    virtual ~FakeLightDimmerSwitch(){}

    void generateProperties();
};

class FakeZigBeeBrightnessBulb: public FakeDevice {
private:
    const long STATE_CAPABILITY_ID             = 10901;
    const long BRIGHTNESS_CAPABILITY_ID        = 10902;
    const long CONNECTION_CAPABILITY_ID        = 10903;
public:
    FakeZigBeeBrightnessBulb(std::string serial):
        FakeDevice("ZigBee_brightness_bulb", "ZigBee Brightness Bulb", serial, "Fake ZigBee Brightness Bulb") {

    }
    virtual ~FakeZigBeeBrightnessBulb(){}

    void generateProperties();
};

class FakeBinarySceneSwitch: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11001;
    const long STATE_CAPABILITY_ID             = 11002;
public:
    FakeBinarySceneSwitch(std::string serial):
        FakeDevice("Binary Scene Switch", "Binary Scene Switch", serial, "Fake Binary Scene Switch") {

    }
    virtual ~FakeBinarySceneSwitch(){}

    void generateProperties();
};

class FakeDeviceFactory {
public:
    static FakeDevice* buildFakeDevice(int typecnt, std::string serial){
        Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
        logger.debug("Creating (%d) device with serial (%s)", typecnt, serial);
        FakeDevice* tmpDevice;
        switch(typecnt){
            case DEFAULT_FAKE_DEVICE:     tmpDevice = new FakeDefaultDevice(serial); break;
            case LIFX_TEMPERATURE_BULB:   tmpDevice = new FakeLifxTemperatureBulb(serial); break;
            case HOME_SECURITY_SENSOR:    tmpDevice = new FakeHomeSecuritySensor(serial); break;
            case ROUTING_ALARM_SENSOR:    tmpDevice = new FakeRoutingAlarmSensor(serial); break;
            case ROUTING_BINARY_SENSOR:   tmpDevice = new FakeRoutingBinarySensor(serial); break;
            case SECURE_KEYPAD_DOOR_LOCK: tmpDevice = new FakeKeypadDoorLock(serial); break;
            case LIGHT_DIMMER_SWITCH:     tmpDevice = new FakeLightDimmerSwitch(serial); break;
            case ZIGBEE_BRIGHTNESS_BULB:  tmpDevice = new FakeZigBeeBrightnessBulb(serial); break;
            case BINARY_SCENE_SWITCH:     tmpDevice = new FakeBinarySceneSwitch(serial); break;
            default: tmpDevice = NULL;
        }
        return tmpDevice;
    }
};

#endif // FAKE_DEVICE_FACTORY_H
