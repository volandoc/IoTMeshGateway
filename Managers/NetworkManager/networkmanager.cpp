#include "networkmanager.h"
#include <iostream>

GW_MANAGER(NetworkManager, "Network Manager", "0.0.1")

NetworkManager::NetworkManager(){
    std::cout << "Network Manager: Created" << std::endl;
}

NetworkManager::~NetworkManager(){
    std::cout << "Network Manager: Removed" << std::endl;
}

int NetworkManager::startManager(){
    std::cout << "Network Manager: Started" << std::endl;
    return 0;
}

int NetworkManager::executeCommand(){
    std::cout << "Network Manager: executeCommand" << std::endl;
    return 0;
}

int NetworkManager::getCommandSet(){
    std::cout << "Network Manager: getCommandSet" << std::endl;
    return 0;
}

int NetworkManager::getCapabilitiesSet(){
    std::cout << "Network Manager: getCapabilitiesSet" << std::endl;
    return 0;
}

int NetworkManager::stopManager(){
    std::cout << "Network Manager: Stopped" << std::endl;
    return 0;
}
