#include <iostream>
#include <ctime>
#include <cerrno>
#include <csignal>

#include <Poco/ClassLoader.h>
#include <Poco/Manifest.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>

#include "pluginsapi.h"
#include "plugincontainer.h"
#include "innerbusapi.h"

typedef Poco::ClassLoader<InnerBusIF> BusLoader;
typedef Poco::Manifest<InnerBusIF> BusManifest;

PluginContainer pluginContainer;
BusLoader busLoader;
InnerBusClientIF* busClient = NULL;
Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig = new Poco::Util::PropertyFileConfiguration("../conf/loggerconf.properties");
Poco::Util::LoggingConfigurator configurator;

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
    Poco::Logger& logger = Poco::Logger::get("Main");


    logger.information("Interrupt signal (%d) received.\n", signum);
    logger.information("Stop Gateway-Picaso version emb-0.0.1");
    // cleanup and close up stuff here
    // terminate program
    pluginContainer.unloadPlugins();

    busClient->disconnect();
    delete busClient;
    busClient=NULL;

    busLoader.unloadLibrary("../core/InnerBus/InnerBus.so");

    exit(signum);
}

void registerSignalHandler( void ) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}


int main(int argv, char* argc[]) {
    configurator.configure(pConfig);

    Poco::Logger& logger = Poco::Logger::get("Main");

    registerSignalHandler();

    std::string path("../core/InnerBus/InnerBus");
    path.append(Poco::SharedLibrary::suffix());
    try {
        busLoader.loadLibrary(path);
        logger.information((busLoader.isLibraryLoaded(path)? "Loaded" : "Failed"));
    } catch(Poco::Exception excp) {
        logger.log(excp, __FILE__ , 84);
    }

    InnerBusIF& innerBus = busLoader.instance("InnerBus");

    innerBus.loadConfig();

    busClient = innerBus.createIBusClient();

    busClient->getInfo();

    busClient->init();

    busClient->connect_async();

    logger.information("Start Embedded IoT Gateway version emb-0.0.1");

    logger.information("Hello World!");
    pluginContainer.LoadPlugins();

    while(true){
        sleep(1000);
    }
    return 0;
}
