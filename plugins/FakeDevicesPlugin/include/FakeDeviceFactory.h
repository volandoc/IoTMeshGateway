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

static const std::string FakeDeviceTypes[DEVICE_TYPES_SIZE] = { "Default_Fake_Device",
                                           "LIFX_bulb_temperature",
                                           "Home Security Sensor",
                                           "Routing Alarm Sensor",
                                           "Routing Binary Sensor",
                                           "Secure Keypad Door Lock",
                                           "Light Dimmer Switch",
                                           "ZigBee_brightness_bulb",
                                           "Binary Scene Switch" };


class FakeDefaultDevice: public FakeDevice {
private:
    static const long CONNECTION_CAPABILITY_ID;
    static const long STATE_CAPABILITY_ID;
    static const std::string NAME;
    static const std::string DESCRIPTION;
public:
    FakeDefaultDevice(std::string serial): FakeDevice("Default_Fake_Device",
                       FakeDefaultDevice::NAME, serial, FakeDefaultDevice::DESCRIPTION) {

    }
    virtual ~FakeDefaultDevice(){}

    void generateProperties();
};

class FakeLifxTemperatureBulb: public FakeDevice {
private:
    long STATE_CAPABILITY_ID             = 10701;
    long BRIGHTNESS_CAPABILITY_ID        = 10702;
    long COLOR_TEMPERATURE_CAPABILITY_ID = 10703;
    long CONNECTION_CAPABILITY_ID        = 10704;
    std::string NAME        = "LIFX temperature bulb";
    std::string DESCRIPTION = "Fake LIFX temperature bulb";
public:
    FakeLifxTemperatureBulb(std::string serial): FakeDevice("LIFX_bulb_temperature",
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeLifxTemperatureBulb(){}

    void generateProperties();
};

class FakeHomeSecuritySensor: public FakeDevice {
private:
    long CONNECTION_CAPABILITY_ID        = 11301;
    long STATE_CAPABILITY_ID             = 11302;
    long BATTERY_CAPABILITY_ID           = 11303;
    long TEMPERATURE_CAPABILITY_ID       = 11304;
    long LUMINANCE_CAPABILITY_ID         = 11305;
    long SENSITIVITY_CAPABILITY_ID       = 11307;
    std::string NAME        = "Home Security Sensor";
    std::string DESCRIPTION = "Fake Home Security Sensor";
public:
    FakeHomeSecuritySensor(std::string serial): FakeDevice(FakeDeviceTypes[HOME_SECURITY_SENSOR],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeHomeSecuritySensor(){}

    void generateProperties();
};

class FakeRoutingAlarmSensor: public FakeDevice {
private:
    long CONNECTION_CAPABILITY_ID        = 11101;
    long STATE_CAPABILITY_ID             = 11102;
    long BATTERY_CAPABILITY_ID           = 11103;
    long TEMPERATURE_CAPABILITY_ID       = 11104;
    std::string NAME        = "Routing Alarm Sensor";
    std::string DESCRIPTION = "Fake Routing Alarm Sensor";
public:
    FakeRoutingAlarmSensor(std::string serial): FakeDevice(FakeDeviceTypes[ROUTING_ALARM_SENSOR],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeRoutingAlarmSensor(){}

    void generateProperties();

};

class FakeRoutingBinarySensor: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11201;
    const long STATE_CAPABILITY_ID             = 11202;
    const long BATTERY_CAPABILITY_ID           = 11203;
    const std::string NAME        = "Routing Binary Sensor";
    const std::string DESCRIPTION = "Fake Routing Binary Sensor";
public:
    FakeRoutingBinarySensor(std::string serial): FakeDevice(FakeDeviceTypes[ROUTING_BINARY_SENSOR],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeRoutingBinarySensor(){}

    void generateProperties();

};

class FakeKeypadDoorLock: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11601L;
    const long STATE_CAPABILITY_ID             = 11602L;
    const long BATTERY_CAPABILITY_ID           = 11603L;
    const long LAST_UNLOCKED_USER_ID           = 11604L;
    const std::string NAME        = "Secure Keypad Door Lock";
    const std::string DESCRIPTION = "Fake Secure Keypad Door Lock";
public:
    FakeKeypadDoorLock(std::string serial): FakeDevice(FakeDeviceTypes[SECURE_KEYPAD_DOOR_LOCK],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeKeypadDoorLock(){}

    void generateProperties();
};

class FakeLightDimmerSwitch: public FakeDevice {
private:
    const long CONNECTION_CAPABILITY_ID        = 11501L;
    const long STATE_CAPABILITY_ID             = 11502L;
    const long COLOR_CAPABILITY_ID             = 11503L;
    const std::string NAME        = "Light Dimmer Switch";
    const std::string DESCRIPTION = "Fake Light Dimmer Switch";
public:
    FakeLightDimmerSwitch(std::string serial): FakeDevice(FakeDeviceTypes[LIGHT_DIMMER_SWITCH],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeLightDimmerSwitch(){}

    void generateProperties();
};

class FakeZigBeeBrightnessBulb: public FakeDevice {
private:
    const long STATE_CAPABILITY_ID             = 10901L;
    const long BRIGHTNESS_CAPABILITY_ID        = 10902L;
    const long CONNECTION_CAPABILITY_ID        = 10903L;
    const std::string NAME        = "ZigBee Brightness Bulb";
    const std::string DESCRIPTION = "Fake ZigBee Brightness Bulb";
public:
    FakeZigBeeBrightnessBulb(std::string serial): FakeDevice(FakeDeviceTypes[ZIGBEE_BRIGHTNESS_BULB],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeZigBeeBrightnessBulb(){}

    void generateProperties();
};

class FakeBinarySceneSwitch: public FakeDevice {
private:
    long CONNECTION_CAPABILITY_ID        = 11001L;
    long STATE_CAPABILITY_ID             = 11002L;
    std::string NAME        = "Binary Scene Switch";
    std::string DESCRIPTION = "Fake Binary Scene Switch";
public:
    FakeBinarySceneSwitch(std::string serial): FakeDevice(FakeDeviceTypes[BINARY_SCENE_SWITCH],
                                                            NAME, serial, DESCRIPTION) {

    }
    virtual ~FakeBinarySceneSwitch(){}

    void generateProperties();
};

class FakeDeviceFactory {
public:
    static FakeDevice* buildFakeDevice(int typecnt, std::string serial){
        //Poco::Logger& logger = Poco::Logger::get("FakeDevicesPlugin");
        //logger.debug("Creating (%d) device with serial (%s)", type, serial);
        FakeDevice* tmpDevice;
        if ( DEFAULT_FAKE_DEVICE == typecnt) {
            tmpDevice = new FakeDefaultDevice(serial);
            tmpDevice->generateProperties();
            delete tmpDevice;

            //case 1: tmpDevice = new FakeLifxTemperatureBulb(serial);
            //case 2: tmpDevice = new FakeHomeSecuritySensor(serial); break;
            //case 3: tmpDevice = new FakeRoutingAlarmSensor(serial); break;
            //case 4: tmpDevice = new FakeRoutingBinarySensor(serial); break;
            //case 5: tmpDevice = new FakeKeypadDoorLock(serial); break;
            //case 6: tmpDevice = new FakeLightDimmerSwitch(serial); break;
            //case 7: tmpDevice = new FakeZigBeeBrightnessBulb(serial); break;
            //case 8: tmpDevice = new FakeBinarySceneSwitch(serial); break;

        } else {
            tmpDevice = NULL;
        }
        return NULL;
    }
};

#endif // FAKE_DEVICE_FACTORY_H
