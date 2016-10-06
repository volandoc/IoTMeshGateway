#include <iostream>
#include <dlfcn.h>
#include "gateway.h"
#include "mqttclient.h"
#include "Managers/managersapi.h"

#define ELPP_NO_DEFAULT_LOG_FILE
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {

    START_EASYLOGGINGPP(argv, argc);
    // Load configuration from file
    el::Configurations conf("../conf/logger.conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead

    // Set the plugin shared library location
    std::string path1("../Managers/DeviceManager/libDeviceManager.so.1.0.0");
    std::string path2("../Managers/NetworkManager/libNetworkManager.so.1.0.0");
    void *dmhndl = dlopen(path1.c_str(), RTLD_NOW);
    if(dmhndl==NULL){
        std::cout << dlerror() << std::endl;
    }

    void *nmhndl = dlopen(path2.c_str(), RTLD_NOW);
    if(nmhndl==NULL){
        std::cout << dlerror() << std::endl;
    }

    LOG(INFO) << "Start Gateway-Picaso version emb-0.0.1";
    gateway * embGateway = new gateway();
    embGateway->start();

    cout << "Hello World!" << endl;

    gw::managers::ManagerDetails *managerDetails1;
    gw::managers::ManagerDetails *managerDetails2;

    managerDetails1 = reinterpret_cast<gw::managers::ManagerDetails*> (dlsym(dmhndl, "exportDetails"));

    managerDetails2 = reinterpret_cast<gw::managers::ManagerDetails*> (dlsym(nmhndl, "exportDetails"));

    std::cout << "Plugin Info: "
                << "\n\tAPI Version: " << managerDetails1->apiVersion
                << "\n\tFile Name: " << managerDetails1->fileName
                << "\n\tClass Name: " << managerDetails1->className
                << "\n\tPlugin Name: " << managerDetails1->managerName
                << "\n\tPlugin Version: " << managerDetails1->managerVersion
                << std::endl;

    std::cout << "Plugin Info: "
                << "\n\tAPI Version: " << managerDetails2->apiVersion
                << "\n\tFile Name: " << managerDetails2->fileName
                << "\n\tClass Name: " << managerDetails2->className
                << "\n\tPlugin Name: " << managerDetails2->managerName
                << "\n\tPlugin Version: " << managerDetails2->managerVersion
                << std::endl;

    // API Version checking
    if (managerDetails1->apiVersion != GW_MANAGER_API_VERSION)
        throw std::runtime_error("Plugin ABI version mismatch.");

    if (managerDetails2->apiVersion != GW_MANAGER_API_VERSION)
        throw std::runtime_error("Plugin ABI version mismatch.");

    // Instantiate the plugin
    auto manager1 = reinterpret_cast<gw::managers::GWManagerIf*>(managerDetails1->initializeFunc());
    auto manager2 = reinterpret_cast<gw::managers::GWManagerIf*>(managerDetails2->initializeFunc());

    manager1->setListener(embGateway);
    manager2->setListener(embGateway);
    manager1->startManager();
    manager2->startManager();
    manager1->notifyListener("DeviceManager");
    manager1->notifyListener("NetworkManager");
    manager1->stopManager();
    manager2->stopManager();

    embGateway->stop();
    cout << "What this!" << endl;
    LOG(INFO) << "Stop Gateway-Picaso version emb-0.0.1";

    dlclose(dmhndl);
    dlclose(nmhndl);
    return 0;

}

