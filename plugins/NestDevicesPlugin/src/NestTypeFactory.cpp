#include "NestTypeFactory.h"
#include <iostream>

std::string nestTypeNames[] = {
    "cameras",
    "thermostats",
};


void NestTypeCamera::init() {
    initCapabilities();
    NestType::init(nestTypeNames[NEST_TYPE_CAMERA]);
}

void NestTypeCamera:: initCapabilities() {
    NestType::initCapabilities(nestTypeNames[NEST_TYPE_CAMERA]);
}


void NestTypeThermostat::init() {
    initCapabilities();
    NestType::init(nestTypeNames[NEST_TYPE_THERMOSTAT]);
}

void NestTypeThermostat:: initCapabilities() {
    NestType::initCapabilities(nestTypeNames[NEST_TYPE_THERMOSTAT]);
}

