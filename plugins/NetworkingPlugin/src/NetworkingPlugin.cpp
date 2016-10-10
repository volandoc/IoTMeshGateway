#include "NetworkingPlugin.h"
#include <iostream>

UCL_PLUGIN(NetworkingPlugin, "Networking Plugin", "0.0.1")

NetworkingPlugin::NetworkingPlugin(){
    std::cout << "Network Manager: Created" << std::endl;
}

NetworkingPlugin::~NetworkingPlugin(){
    std::cout << "Network Manager: Removed" << std::endl;
}

int NetworkingPlugin::startPlugin(){
    std::cout << "Network Manager: Started" << std::endl;
    return 0;
}

int NetworkingPlugin::executeCommand(){
    std::cout << "Network Manager: executeCommand" << std::endl;
    return 0;
}

int NetworkingPlugin::getCommandSet(){
    std::cout << "Network Manager: getCommandSet" << std::endl;
    return 0;
}

int NetworkingPlugin::getCapabilitiesSet(){
    std::cout << "Network Manager: getCapabilitiesSet" << std::endl;
    return 0;
}

int NetworkingPlugin::stopPlugin(){
    std::cout << "Network Manager: Stopped" << std::endl;
    return 0;
}
