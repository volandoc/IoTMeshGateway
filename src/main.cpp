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
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include "pluginsapi.h"
#include "plugincontainer.h"
#include "innerbusapi.h"

typedef Poco::ClassLoader<InnerBusIF> BusLoader;
typedef Poco::Manifest<InnerBusIF> BusManifest;

class SampleServer: public Poco::Util::ServerApplication {
private:
    bool _helpRequested;
    std::string plugins_dir;
    std::string core_dir;
    std::string path;

protected:
    void initialize(Application& self) {
        path = Application::config().getString("application.dir");
        logger().information(path + "../conf/embgateway.properties");
        Poco::Util::Application::loadConfiguration(path +"../conf/embgateway.properties"); // load default configuration files, if present
        ServerApplication::initialize(self);
        plugins_dir = Application::config().getString("application.pluginsdir");
        core_dir = Application::config().getString("application.coredir");
    }

    void uninitialize()	{
        ServerApplication::uninitialize();
    }

    void defineOptions(Poco::Util::OptionSet& options)	{
        ServerApplication::defineOptions(options);

        options.addOption(
            Poco::Util::Option("help", "h", "display help information on command line arguments")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<SampleServer>(this, &SampleServer::handleHelp)));
        options.addOption(
            Poco::Util::Option("conf", "c", "with configuration file")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<SampleServer>(this, &SampleServer::handleConfig)));
        options.addOption(
            Poco::Util::Option("plugins", "p", "with plugins folder")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<SampleServer>(this, &SampleServer::handleHelp)));
        options.addOption(
            Poco::Util::Option("verbose", "v", "log everything")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<SampleServer>(this, &SampleServer::handleVerbose)));
    }

    void handleHelp(const std::string& name, const std::string& value)	{
        _helpRequested = true;
        displayHelp();
        stopOptionsProcessing();
    }

    void handleConfig(const std::string& name, const std::string& value) {
        loadConfiguration(value);
    }

    void handleVerbose(const std::string& name, const std::string& value)	{
        logger().setLevel("debug");
    }

    void displayHelp()
    {
        Poco::Util::HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A sample server application that demonstrates some of the features.");
        helpFormatter.format(std::cout);
    }

    int main(const ArgVec& args) {
        if (!_helpRequested) {
            logger().information("Start Embedded IoT Gateway version emb-0.0.1");
            BusLoader busLoader;
            PluginContainer pluginContainer(path + "../" + plugins_dir);

            std::string coreBusPath(path + "../" + core_dir + "/InnerBus/InnerBus");
            coreBusPath.append(Poco::SharedLibrary::suffix());
            try {
                busLoader.loadLibrary(coreBusPath);
                logger().information((busLoader.isLibraryLoaded(coreBusPath)? "Loaded" : "Failed"));
            } catch(Poco::Exception excp) {
                logger().log(excp, __FILE__ , 83);
            }

            InnerBusIF& innerBus = busLoader.instance("InnerBus");

            innerBus.loadConfig(path + "../" + core_dir + "/InnerBus");

            InnerBusClientIF* busClient = innerBus.createIBusClient();
            busClient->getInfo();
            busClient->init();
            busClient->connect_async();

            pluginContainer.LoadPlugins();
            pluginContainer.addIBusClients(innerBus);
            pluginContainer.startPlugins();


            waitForTerminationRequest();

            logger().information("shutting down");
            busClient->disconnect();
            delete busClient;
            busClient=NULL;

            pluginContainer.stopPlugins();

            busLoader.unloadLibrary(coreBusPath);
        }
        return Application::EXIT_OK;
    }

public:
    SampleServer(): _helpRequested(false) {
    }

    ~SampleServer()	{
    }

};


POCO_SERVER_MAIN(SampleServer)