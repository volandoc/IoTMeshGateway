#include "FakeDeviceFactory.h"

void  FakeDefaultDevice::generateProperties(){
    Properties properties;

    properties[STATE_CAPABILITY_ID] = "true";
    properties[CONNECTION_CAPABILITY_ID] = "true";

    FakeDevice::setProperties(properties);
}

void FakeLifxTemperatureBulb::generateProperties(){
    Properties properties;

    properties[STATE_CAPABILITY_ID] = "true";
    properties[BRIGHTNESS_CAPABILITY_ID] = "50";
    properties[COLOR_TEMPERATURE_CAPABILITY_ID] = "3000";
    properties[CONNECTION_CAPABILITY_ID] = "true";

    FakeDevice::setProperties(properties);
}

void FakeHomeSecuritySensor::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[BATTERY_CAPABILITY_ID] = "100";
    properties[TEMPERATURE_CAPABILITY_ID] = "25";
    properties[LUMINANCE_CAPABILITY_ID] = "0";
    properties[SENSITIVITY_CAPABILITY_ID] = "10";

    FakeDevice::setProperties(properties);
}

void FakeRoutingAlarmSensor::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[BATTERY_CAPABILITY_ID] = "100";
    properties[TEMPERATURE_CAPABILITY_ID] = "50";

    FakeDevice::setProperties(properties);
}

void FakeRoutingBinarySensor::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[BATTERY_CAPABILITY_ID] = "100";

    FakeDevice::setProperties(properties);
}

void FakeKeypadDoorLock::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[BATTERY_CAPABILITY_ID] = "100";
    properties[LAST_UNLOCKED_USER_ID] = "25";

    FakeDevice::setProperties(properties);
}

void FakeLightDimmerSwitch::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[COLOR_CAPABILITY_ID] = "#0000000000";

    FakeDevice::setProperties(properties);
}

void FakeZigBeeBrightnessBulb::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";
    properties[BRIGHTNESS_CAPABILITY_ID] = "50";

    FakeDevice::setProperties(properties);
}


void FakeBinarySceneSwitch::generateProperties(){
    Properties properties;

    properties[CONNECTION_CAPABILITY_ID] = "true";
    properties[STATE_CAPABILITY_ID] = "true";

    FakeDevice::setProperties(properties);
}
