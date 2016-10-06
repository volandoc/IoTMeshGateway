#include "devicemanager.h"
#include <iostream>

GW_MANAGER(DeviceManager, "Device Manager", "0.0.1")

DeviceManager::DeviceManager(){
    std::cout << "Device Manager: Created" << std::endl;
}

DeviceManager::~DeviceManager(){
    std::cout << "Device Manager: Removed" << std::endl;
}

int DeviceManager::startManager(){
    std::cout << "Device Manager: Started" << std::endl;
    return 0;
}

int DeviceManager::stopManager(){
    std::cout << "Device Manager: Stopped" << std::endl;
    return 0;
}
