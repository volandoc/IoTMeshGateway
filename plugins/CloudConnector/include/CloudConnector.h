#ifndef CLOUD_CONNECTOR_PLUGIN_H
#define CLOUD_CONNECTOR_PLUGIN_H

#include <string>
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Timer.h>
#include "pluginsapi.h"
#include "mqttclient.h"



class CloudConnector: public UCLPluginIf {
private:
    PluginDetails pluginDetails;
    InnerBusClientIF* busClient = NULL;
    std::string work_dir;
    Poco::Timer timer;
    //Poco::Timer timerDiscoverSensors;
    bool isOnboarded;
    bool isDiscovering;

    std::map<std::string, std::string> discoveredSensors; // <deviceSerial, pluginName>
public:
    CloudConnector();
    virtual ~CloudConnector();

    virtual int startPlugin();
    virtual int setIBusClient(InnerBusClientIF* client);
    virtual int setWorkDir(std::string path);
    virtual int executeCommand(std::string source, IBMessage message);
    virtual int executeInternalCommand(std::string source, std::string message);
    virtual int sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference);
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();

    virtual void doProvision(Poco::Timer &timer);
    virtual void onProvision();

    virtual int discoverSensors();
    //virtual void onDiscoverSensorsEnd(Poco::Timer& timer);

    virtual int connectSensors(std::string serialList);

private:
    mqttclient *mqttClient;
    int gatewayId;
    int homeId;

    bool provision();
    bool initMqttClient();
    bool sendProvision(std::string serial, std::string version, std::string mdn, std::string cloudResFile);
    bool readFileContent(std::string fileName, std::string& fileContent);
    bool getGwId(std::string provisionJson);
    bool getIsOnboarded(std::string provisionJson);
    bool getHomeId(std::string datasyncJson);
    int sendGetDataSync(int gwId, std::string gwDataSyncFile);
    int sendDiscoveredSensors(int gwId, std::string status, std::string sensors, std::string cloudResFile);
    int sendConnectedSensor(int gwId, std::string serial, std::string properties, std::string cloudResFile);

    int storeDiscoveredDevices(std::string pluginSource, std::string sensorsList);
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(CloudConnector)
POCO_END_MANIFEST

#endif // CLOUD_CONNECTOR_PLUGIN_H
