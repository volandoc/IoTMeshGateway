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

int NetworkManager::stopManager(){
    std::cout << "Network Manager: Stopped" << std::endl;
    return 0;
}
