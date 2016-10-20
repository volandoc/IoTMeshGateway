#include "NetworkingPlugin.h"
#include <iostream>

NetworkingPlugin::NetworkingPlugin(){
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "NetworkingPlugin";
    this->pluginDetails.pluginName ="Networking Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";
    std::cout << "Network Manager: Created" << std::endl;
}

NetworkingPlugin::~NetworkingPlugin(){
    std::cout << "Network Manager: Removed" << std::endl;
}

int NetworkingPlugin::startPlugin(){
    std::cout << "Network Manager: Started" << std::endl;
    return 0;
}

int NetworkingPlugin::setIBusClient(InnerBusClientIF* client){
    this->busClient = client;
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

PluginDetails* NetworkingPlugin::getPluginDetails(){
     return &pluginDetails;
}

int NetworkingPlugin::stopPlugin(){
    std::cout << "Network Manager: Stopped" << std::endl;
    return 0;
}
