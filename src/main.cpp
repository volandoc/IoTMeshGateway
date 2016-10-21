#include <iostream>
#include <ctime>
#include <cerrno>
#include <csignal>

#define ELPP_NO_DEFAULT_LOG_FILE
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#include "router.h"
#include "mqttclient.h"
#include "pluginsapi.h"
#include "plugincontainer.h"

router embGateway = router();
PluginContainer pluginContainer = PluginContainer();

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
    pluginContainer.unloadPlugins();
    embGateway.stop();
    exit(signum);

}

void registerSignalHandler( void ) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}


int main(int argv, char* argc[]) {

    START_EASYLOGGINGPP(argv, argc);
    // Load configuration from file
    // el::Configurations conf("../conf/logger.conf");
    // Actually reconfigure all loggers instead
    el::Loggers::configureFromGlobal("../conf/logger.conf");

    registerSignalHandler();

    LOG(INFO) << "Start Gateway-Picaso version emb-0.0.1";
    embGateway.start();

    LOG(INFO) << "Hello World!";
    pluginContainer.LoadPlugins();
    pluginContainer.registerPluginsListener(&embGateway);

    while(true){
        sleep(1000);
    }
    return 0;

}
