#include <iostream>
#include <ctime>
#include <cerrno>
#include <csignal>
#include <list>
#include <dlfcn.h>
#include <dirent.h>

#define ELPP_NO_DEFAULT_LOG_FILE
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#include "router.h"
#include "mqttclient.h"
#include "pluginsapi.h"

router * embGateway = new router();
list<void *> mngrslist;

void sleep(unsigned msec) {
    struct timespec req, rem;
    int err;
    req.tv_sec = msec / 1000;
    req.tv_nsec = (msec % 1000) * 1000000;
    while ((req.tv_sec != 0) || (req.tv_nsec != 0)) {
        if (nanosleep(&req, &rem) == 0)
            break;
        err = errno;
        // Interrupted; continue
        if (err == EINTR) {
            req.tv_sec = rem.tv_sec;
            req.tv_nsec = rem.tv_nsec;
        }
        // Unhandleable error (EFAULT (bad pointer), EINVAL (bad timeval in tv_nsec), or ENOSYS (function not supported))
        break;
    }
}

void signalHandler( int signum ) {
    LOG(INFO) << "What this!";
    LOG(INFO) << "Interrupt signal (" << signum << ") received.\n";
    LOG(INFO) << "Stop Gateway-Picaso version emb-0.0.1";
    // cleanup and close up stuff here
    // terminate program
    embGateway->stop();
    exit(signum);

}

void registerSignalHandler( void ) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

list<void*> loadManagers(string path){
    DIR* mngrsdir;
    struct dirent *direntry;
    string name="";
    list<void*>  tmplist;
    if((mngrsdir=opendir(path.c_str())) != NULL){
        while ((direntry=readdir(mngrsdir)) != NULL ) {
            name=direntry->d_name;
            if((direntry->d_type == DT_DIR) && (name!=".") && (name!="..")) LOG(INFO) << direntry->d_name;
        }
    } else {
        tmplist.clear();
    }
    return tmplist;
}

int main(int argv, char* argc[]) {

    START_EASYLOGGINGPP(argv, argc);
    // Load configuration from file
    // el::Configurations conf("../conf/logger.conf");
    // Actually reconfigure all loggers instead
    el::Loggers::configureFromGlobal("../conf/logger.conf");

    registerSignalHandler();

    // Set the plugin shared library location
    mngrslist=loadManagers("../plugins");
    std::string path1("../plugins/LifXBulbPlugin/libLifXBulbPlugin.so.1.0.0");
    std::string path2("../plugins/NetworkingPlugin/libNetworkingPlugin.so.1.0.0");
    void *dmhndl = dlopen(path1.c_str(), RTLD_NOW);
    if(dmhndl==NULL){
        LOG(ERROR) << dlerror();
    }

    void *nmhndl = dlopen(path2.c_str(), RTLD_NOW);
    if(nmhndl==NULL){
        LOG(ERROR) << dlerror();
    }

    LOG(INFO) << "Start Gateway-Picaso version emb-0.0.1";
    embGateway->start();

    LOG(INFO) << "Hello World!";

    ucl::plugins::PluginDetails *managerDetails1;
    ucl::plugins::PluginDetails *managerDetails2;

    managerDetails1 = reinterpret_cast<ucl::plugins::PluginDetails*> (dlsym(dmhndl, "exportDetails"));

    managerDetails2 = reinterpret_cast<ucl::plugins::PluginDetails*> (dlsym(nmhndl, "exportDetails"));

    LOG(INFO) << "Plugin Info: "
                << "\n\tAPI Version: " << managerDetails1->apiVersion
                << "\n\tClass Name: " << managerDetails1->className
                << "\n\tPlugin Name: " << managerDetails1->pluginName
                << "\n\tPlugin Version: " << managerDetails1->pluginVersion
                << std::endl;

    LOG(INFO) << "Plugin Info: "
                << "\n\tAPI Version: " << managerDetails2->apiVersion
                << "\n\tClass Name: " << managerDetails2->className
                << "\n\tPlugin Name: " << managerDetails2->pluginName
                << "\n\tPlugin Version: " << managerDetails2->pluginVersion
                << std::endl;

    // API Version checking
    if (managerDetails1->apiVersion != UCL_PLUGINS_API_VERSION)
        throw std::runtime_error("Plugin ABI version mismatch.");

    if (managerDetails2->apiVersion != UCL_PLUGINS_API_VERSION)
        throw std::runtime_error("Plugin ABI version mismatch.");

    // Instantiate the plugin
    auto manager1 = reinterpret_cast<ucl::plugins::UCLPluginIf*>(managerDetails1->initializeFunc());
    auto manager2 = reinterpret_cast<ucl::plugins::UCLPluginIf*>(managerDetails2->initializeFunc());

    manager1->setListener(embGateway);
    manager2->setListener(embGateway);
    manager1->startPlugin();
    manager2->startPlugin();
    manager1->notifyListener("DeviceManager");
    manager1->notifyListener("NetworkManager");
    manager1->stopPlugin();
    manager2->stopPlugin();

    dlclose(dmhndl);
    dlclose(nmhndl);

    while(true){
        sleep(1000);
    }
    return 0;

}

