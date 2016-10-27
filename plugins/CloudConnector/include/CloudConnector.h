#ifndef CLOUD_CONNECTOR_PLUGIN_H
#define CLOUD_CONNECTOR_PLUGIN_H

#include <string>
#include "mqttclient.h"
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include "pluginsapi.h"

using namespace std;

class CloudConnector: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
public:
    CloudConnector();
    virtual ~CloudConnector();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int executeCommand();
    virtual int sendOccurrence(string message);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();


    mqttclient *mqttClient;
    int gatewayId;
    int homeId;

    bool initMqttClient();
    bool provision();
    bool sendProvision(string serial, string version, string mdn, string cloudResFile);
    bool readFileContent(string fileName, string& fileContent);
    bool getGwId(string provisionJson, int& gwId);
    bool getIsOnboarded(string provisionJson, bool& isOnboarded);
    bool getHomeId(string datasyncJson, int& homeId);
    int sendGetDataSync(int gwId, string gwDataSyncFile);
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(CloudConnector)
POCO_END_MANIFEST

#endif // CLOUD_CONNECTOR_PLUGIN_H