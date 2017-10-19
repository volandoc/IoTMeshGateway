#include <iostream>
#include <ctime>
#include <cerrno>
#include <csignal>

#include <Poco/Timestamp.h>
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
#include "sysdefs.h"

typedef Poco::ClassLoader<InnerBusIF> BusLoader;
typedef Poco::Manifest<InnerBusIF> BusManifest;

class Gateway: public Poco::Util::ServerApplication {
private:
    bool _helpRequested;
    std::string conf_dir = "/conf/";
    std::string plugins_dir;
    std::string core_dir;
    std::string work_dir;

protected:
    void initialize(Application& self) {

        std::string appdir = Application::config().getString("application.dir");
        work_dir = appdir.substr(0, appdir.find_last_of("bin")-2);

        conf_dir = work_dir + conf_dir;
        logger().information(work_dir);

        Poco::Util::Application::loadConfiguration(conf_dir + "gw.properties"); // load default configuration files, if present

        plugins_dir = work_dir +  Application::config().getString("application.pluginsdir");
        core_dir = work_dir + Application::config().getString("application.coredir");

        ServerApplication::initialize(self);
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
                .callback(Poco::Util::OptionCallback<Gateway>(this, &Gateway::handleHelp)));
        options.addOption(
            Poco::Util::Option("conf", "c", "with configuration file")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<Gateway>(this, &Gateway::handleConfig)));
        options.addOption(
            Poco::Util::Option("plugins", "p", "with plugins folder")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<Gateway>(this, &Gateway::handleHelp)));
        options.addOption(
            Poco::Util::Option("verbose", "v", "log everything")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<Gateway>(this, &Gateway::handleVerbose)));
    }

    void handleHelp(const std::string& name, const std::string& value) {
        _helpRequested = true;
        displayHelp();
        stopOptionsProcessing();
    }

    void handleConfig(const std::string& name, const std::string& value) {
        loadConfiguration(value);
    }

    void handleVerbose(const std::string& name, const std::string& value) {
        logger().setLevel("debug");
        logger().information("%s:%s", name, value);
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
            logger().information("Start IoT Mesh Gateway version 0.0.1");
            BusLoader busLoader;
            PluginContainer pluginContainer(plugins_dir);

            std::string coreBusPath(core_dir + "/InnerBus/InnerBus");
            coreBusPath.append(Poco::SharedLibrary::suffix());
            try {
                busLoader.loadLibrary(coreBusPath);
                logger().information("InnerBus library %s", std::string((busLoader.isLibraryLoaded(coreBusPath)? "Loaded" : "Failed")));
            } catch(Poco::Exception excp) {
                logger().log(excp, __FILE__ , 83);
            }

            InnerBusIF& innerBus = busLoader.instance("InnerBus");

            innerBus.loadConfig(core_dir + "/InnerBus");
            innerBus.setRoot(Application::config().getString("application.gateway.serial"));

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

            pluginContainer.unloadPlugins();

            busLoader.unloadLibrary(coreBusPath);
        }
        return Application::EXIT_OK;
    }

public:
    Gateway(): _helpRequested(false) {
    }

    ~Gateway()	{
    }

};


POCO_SERVER_MAIN(Gateway)
