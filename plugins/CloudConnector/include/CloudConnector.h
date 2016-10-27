#ifndef CLOUD_CONNECTOR_PLUGIN_H
#define CLOUD_CONNECTOR_PLUGIN_H

#include <string>
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Timer.h>
#include "pluginsapi.h"
#include "mqttclient.h"

using namespace std;

class CloudConnector: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    Poco::Timer timer;
    bool isOnboarded;
public:
    CloudConnector();
    virtual ~CloudConnector();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string message);
    virtual int sendOccurrence(string message);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();

    virtual int executeCloudCommand(std::string message);

    virtual void doProvision(Poco::Timer &timer);
    virtual void onProvision();

private:
    mqttclient *mqttClient;
    int gatewayId;
    int homeId;

    bool provision();
    bool initMqttClient();
    bool sendProvision(string serial, string version, string mdn, string cloudResFile);
    bool readFileContent(string fileName, string& fileContent);
    bool getGwId(string provisionJson);
    bool getIsOnboarded(string provisionJson);
    bool getHomeId(string datasyncJson);
    int sendGetDataSync(int gwId, string gwDataSyncFile);
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(CloudConnector)
POCO_END_MANIFEST

#endif // CLOUD_CONNECTOR_PLUGIN_H
