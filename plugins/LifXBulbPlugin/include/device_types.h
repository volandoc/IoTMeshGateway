#ifndef DEVICE_TYPES_H
#define DEVICE_TYPES_H

#include <map>
#include <string>
#include "deviceapi.h"

static const std::string LIFX_BULB_800("LIFX_bulb_temperature");
static const std::string LIFX_BULB_ORIGINAL_1000("LIFX_hue_bulb");

static const DeviceType deviceTypes = {
    { LIFX_BULB_800, "Lifx White 800" },
    { LIFX_BULB_ORIGINAL_1000, "Lifx 1000" }
};

#endif // DEVICE_TYPES_H
