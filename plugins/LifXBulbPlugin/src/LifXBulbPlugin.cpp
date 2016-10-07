#include "LifXBulbPlugin.h"
#include <iostream>

UCL_PLUGIN(LifXBulbPlugin, "LifX Bulb Plugin", "0.0.1")

LifXBulbPlugin::LifXBulbPlugin(){
    std::cout << "Device Manager: Created" << std::endl;
}

LifXBulbPlugin::~LifXBulbPlugin(){
    std::cout << "Device Manager: Removed" << std::endl;
}

int LifXBulbPlugin::startPlugin(){
    std::cout << "Device Manager: Started" << std::endl;
    return 0;
}

int LifXBulbPlugin::executeCommand(){
    std::cout << "Device Manager: executeCommand" << std::endl;
    return 0;
}

int LifXBulbPlugin::getCommandSet(){
    std::cout << "Device Manager: getCommandSet" << std::endl;
    return 0;
}

int LifXBulbPlugin::getCapabilitiesSet(){
    std::cout << "Device Manager: getCapabilitiesSet" << std::endl;
    return 0;
}

int LifXBulbPlugin::stopPlugin(){
    std::cout << "Device Manager: Stopped" << std::endl;
    return 0;
}
