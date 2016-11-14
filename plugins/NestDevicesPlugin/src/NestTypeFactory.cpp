#include "NestTypeFactory.h"


std::string nestTypeNames[] = {
    "cameras",
    "thermostats",
};


void NestTypeCamera::init() {
    NestType::init(nestTypeNames[NEST_TYPE_CAMERA]);
}

void NestTypeThermostat::init() {
    NestType::init(nestTypeNames[NEST_TYPE_THERMOSTAT]);
}

