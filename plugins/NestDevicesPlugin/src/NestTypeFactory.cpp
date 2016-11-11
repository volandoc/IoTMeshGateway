#include "NestTypeFactory.h"


std::string nestTypeNames[] = {
    "cameras",
};


void NestTypeCamera::init() {
    NestType::init(nestTypeNames[NEST_TYPE_CAMERA]);
}

