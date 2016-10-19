#include "LifXBulbPlugin.h"
#include <iostream>

LifXBulbPlugin::LifXBulbPlugin(){
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "LifXBulbPlugin";
    this->pluginDetails.pluginName ="LifX Bulb Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
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

PluginDetails* LifXBulbPlugin::getPluginDetails(){
    return &pluginDetails;
}

int LifXBulbPlugin::stopPlugin(){
    std::cout << "Device Manager: Stopped" << std::endl;
    return 0;
}
