#include "CloudConnector.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <streambuf>
#include "rest.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include "sysdefs.h"
#include "gwCommand.h"

#define GW_SERIAL                       "987654321"
#define GW_VERSION                      "2"
#define GW_MDN                          "123456789"

#define GW_ID_FILE_PARAMETER_GW_ID      "gatewayId"
#define GW_ID_FILE_PARAMETER_IS_ONBOARD "isOnboarded"

#define GW_PROVISION_INTERVAL           5000
#define GW_PROVISION_START_INTERVAL     0

//#define GW_DISCOVER_INTERVAL            0
//#define GW_DISCOVER_START_INTERVAL      30000


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;


CloudConnector::CloudConnector()
    : mqttClient(nullptr)
    , gatewayId(0)
    , homeId(0)
    , isDiscovering(false)
{
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->pluginDetails.type = _PD_T_COMM;
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "CloudConnector";
    this->pluginDetails.pluginName ="CloudConnector Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";

    this->timer.setStartInterval(GW_PROVISION_START_INTERVAL);
    this->timer.setPeriodicInterval(GW_PROVISION_INTERVAL);

//    this->timerDiscoverSensors.setStartInterval(GW_DISCOVER_START_INTERVAL);
//    this->timerDiscoverSensors.setPeriodicInterval(GW_DISCOVER_INTERVAL);

    initMqttClient();

    logger.debug("Plugin Created");
}

CloudConnector::~CloudConnector() {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }

    if( this->mqttClient != NULL ) {
        delete this->mqttClient;
        this->mqttClient = nullptr;
    }

    logger.debug("Plugin Removed");
}

int CloudConnector::startPlugin() {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
        logger.debug("Started");

        timer.start(Poco::TimerCallback<CloudConnector>(*this, & CloudConnector::doProvision));
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }
    return 0;
}

void CloudConnector::doProvision(Timer& timer){
    if(provision()) {
        this->onProvision();
        timer.restart(0);
    }
    else {
        Poco::Logger& logger = Poco::Logger::get("CloudConnector");
        logger.debug("Gateway provision failed, retrying in 5 s...");
    }

}

void CloudConnector::onProvision() {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Gateway provision SUCCESS");

    string dataSyncFile = this->work_dir + _FILE_SEPARATOR + REST_DATASYNC_FILE;
    sendGetDataSync(this->gatewayId, dataSyncFile);

    string datasyncJson;
    if (true == readFileContent(dataSyncFile, datasyncJson) &&
            true == getHomeId(datasyncJson)) {

        mqttClient->topics_init(this->gatewayId, this->homeId);
        mqttClient->do_connect_async();

        logger.debug("all done");

        return;
    }
    else
    {
        logger.error("Error: Failed to get Home ID.");
        return;
    }
}

int CloudConnector::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    return 0;
}

int CloudConnector::setWorkDir(std::string path){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Running from %s", path);
    this->work_dir = path;
}

int CloudConnector::executeCommand(std::string source, IBMessage message){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    IBPayload payload = message.getPayload();

    if (!message.getReference().compare(pluginDetails.pluginName)) {
        logger.debug("executeCommand for {%s} msg{%s}", source, message.getPayload());
        logger.debug("\"%s : %s : %s : %d\"", message.getId(), message.getPayload(), message.getReference(), (int) message.getTimestamp());

        if(true) {
            logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());

            if ("event" == payload.getType()) {
                if ("SUCCESS" == payload.getValue()) {
                    if ("LIST" == payload.getCvalue()) {
                        // source = "GateWay_Serial/{PluginName/#}"
                        int begin = source.find("/") + 1;
                        int end = source.find("/", begin);
                        std::string pluginSource = source.substr(begin, end - begin);

                        storeDiscoveredDevices(pluginSource, payload.getContent());

                        sendDiscoveredSensors(this->gatewayId, "IN_PROGRESS", payload.getContent(), "log.log");
                    } else if ("PROPERTIES" == payload.getCvalue()) {
                        sendConnectedSensor(this->gatewayId, "serial", payload.getContent(), "log.log");
                    }
                }
            }
        }
    }

    return 0;
}

int CloudConnector::executeInternalCommand(std::string source, std::string message){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    std::string json = message;
    gwCommand gwCmd(json);

    /* TO-DO: after implementation of if-elseif-else logic place one logger.debug call
        before if-elseif-else chain*/
    std::string eventType = gwCmd.getEventType();
    if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_SYNCDATA))
    {
        logger.debug("command %s received", eventType);
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_DISCOVERSENSORS))
    {
        logger.debug("command %s received", eventType);
        discoverSensors();
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_CONNECTSENSORS))
    {
        logger.debug("command %s received", eventType);
        connectSensors(gwCmd.getPayload());
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_UPDATEPLUGINS))
    {
        logger.debug("command %s received", eventType);
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_UPDATEFIRMWARE))
    {
        logger.debug("command %s received", eventType);
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_REBOOT))
    {
        logger.debug("command %s received", eventType);
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_RESET))
    {
        logger.debug("command %s received", eventType);
        this->isOnboarded = false;
        this->mqttClient->do_disconnect();

        this->timer.stop();
        this->timer.setStartInterval(GW_PROVISION_START_INTERVAL);
        this->timer.setPeriodicInterval(GW_PROVISION_INTERVAL);
        this->timer.start(Poco::TimerCallback<CloudConnector>(*this, & CloudConnector::doProvision));
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_BACKUP))
    {
        logger.debug("command %s received", eventType);
    }
    else if (!strcmp(eventType.c_str(), GW_COMMAND_EVENT_RESTORE))
    {
        logger.debug("command %s received", eventType);
    }
    else
    {
        logger.debug("command %s received", eventType);
    }

    return 0;
}

int CloudConnector::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference, std::string sender) {
    return 0;
}

int CloudConnector::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("getCommandSet");
    return 0;
}

int CloudConnector::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* CloudConnector::getPluginDetails(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    return &pluginDetails;
}

int CloudConnector::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");

    timer.stop();

    this->mqttClient->do_disconnect();

    logger.debug("Stopped");
    return 0;
}

bool CloudConnector::initMqttClient()
{
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");

    int protocolVersion = MQTT_PROTOCOL_V311;
    try {
        this->mqttClient = new mqttclient(  MQTT_ID,
                                            MQTT_HOST,
                                            MQTT_PORT,
                                            MQTT_KEEPALIVE,
                                            true,
                                            0,
                                            0,
                                            protocolVersion);

        this->mqttClient->setCallback(this);

        if(this->mqttClient->tls_set(   MQTT_ROOT_CA_FILENAME,
                                        MQTT_CERT_DIR,
                                        MQTT_CERTIFICATE_FILENAME,
                                        MQTT_PRIVATE_KEY_FILENAME,
                                        NULL)) {
            logger.error("Error: Problem setting TLS options.");
            return false;
        }


        if(this->mqttClient->tls_insecure_set(true)) {
            logger.error("Error: Problem setting TLS insecure option.");
            return false;
        }

        if(this->mqttClient->tls_opts_set( 1, MQTT_TLS_VER, NULL)){
            logger.error("Error: Problem setting TLS options");
            return false;
        }

        if(this->mqttClient->max_inflight_messages_set(20)) {
            logger.error("Error: Problem setting max inflight messages option.");
            return false;
        }
        if(this->mqttClient->opts_set(MOSQ_OPT_PROTOCOL_VERSION, (void*)(&protocolVersion)))  {
            logger.error("Error: Problem setting protocol version.");
            return false;
        }
    }
    catch (std::bad_alloc) {
        this->mqttClient = nullptr;
        logger.error("Error: Failed to alloc memory for Mqtt Client");
        return false;
    }

    return true;
}

bool CloudConnector::provision()
{
    bool status = true;
    std::string cloudResponse;
    std::string provisionFile = this->work_dir + _FILE_SEPARATOR + REST_PROVISION_FILE;

    if (false == sendProvision(GW_SERIAL, GW_VERSION, GW_MDN, provisionFile))
    {
        status = false;
    }

    if (true == status)
    {
        status = readFileContent(provisionFile, cloudResponse);
    }

    if (true == status)
    {
        status = getGwId(cloudResponse);
    }

    if (true == status)
    {
        status = getIsOnboarded(cloudResponse);
    }
    if (true == status)
    {
        status = isOnboarded;
    }

    return status;
}

bool CloudConnector::sendProvision(std::string serial, std::string version, std::string mdn, std::string cloudResFile)
{
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_PUT);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);

    buffer = "\"{\\\"serial\\\":\\\"" + serial + "\\\",\\\"version\\\":\\\"" + version + "\\\",\\\"mdn\\\":\\\"+" + mdn + "\\\"}\"";
    rst.setDataBody(buffer.c_str());

    rst.setHeader("\"Content-Type:application/json\"");

    std::string tempOutputFile = this->work_dir + _FILE_SEPARATOR + REST_OUTPUT_FILE;
    rst.setConsoleOutputFile(tempOutputFile);
    rst.setCloudResponseFile(cloudResFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_PROVISION;
    buffer += serial;
    rst.setUrl(buffer.c_str());

    std::string built_rest = rst.buildRest();

    Poco::Logger& logger = Poco::Logger::get("CloudConnector");

    int retVal = system(built_rest.c_str());

    if (0 == retVal)
    {
        return true;
    }
    return false;
}

bool CloudConnector::readFileContent(std::string fileName, std::string&  fileContent)
{
    ifstream streamFile(fileName.c_str());
    if (streamFile.is_open())
    {
        std::string strFileContent;

        streamFile.seekg(0, ios::end);
        strFileContent.reserve(streamFile.tellg());
        streamFile.seekg(0, ios::beg);

        strFileContent.assign((istreambuf_iterator<char>(streamFile)),
                                istreambuf_iterator<char>());

        fileContent = strFileContent;
        return true;
    }
    return false;
}

bool CloudConnector::getGwId(std::string provisionJson)
{
    bool retVal = false;
    if (0 != provisionJson.length())
    {
        int temp = 0;
        Parser parser;
        Var result = parser.parse(provisionJson);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var gatewayId = object->get(GW_ID_FILE_PARAMETER_GW_ID);
        std::string gatewayIdValue = gatewayId.convert<std::string>();

        try
        {
            temp = stoi(gatewayIdValue);
            this->gatewayId = temp;
            retVal = true;
        }
        catch(...)
        {
            retVal = false;
        }
    }
    return retVal;
}

bool CloudConnector::getIsOnboarded(std::string provisionJson)
{
    bool retVal = false;
    if (0 != provisionJson.length())
    {
        Parser parser;
        Var result = parser.parse(provisionJson);
        Object::Ptr object = result.extract<Object::Ptr>();

        Var varIsOnboarded = object->get(GW_ID_FILE_PARAMETER_IS_ONBOARD);
        std::string isOnboardedValue = varIsOnboarded.convert<std::string>();

        if ("true" == isOnboardedValue)
        {
            this->isOnboarded = true;
            retVal = true;
        }
        else if ("false" == isOnboardedValue)
        {
            this->isOnboarded = false;
            retVal = true;
        }
        else
        {
            retVal = false;
        }
    }
    return retVal;
}

bool CloudConnector::getHomeId(std::string datasyncJson)
{
    bool retVal = false;

    if (0 != datasyncJson.length())
    {
        int temp = 0;
        Parser parser;
        Var result = parser.parse(datasyncJson);
        Object::Ptr object = result.extract<Object::Ptr>();

        Var home = object->get("home");
        std::string homeStr = home.convert<std::string>();

        object = home.extract<Object::Ptr>();
        Var id = object->get("id");
        std::string homeIdValue = id.convert<std::string>();

        try
        {
            temp = stoi(homeIdValue);
            this->homeId = temp;
            retVal = true;
        }
        catch(...)
        {
            retVal = false;
        }
    }
    return retVal;
}

int CloudConnector::sendGetDataSync(int gwId, std::string gwDataSyncFile)
{
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_GET);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);
    rst.setHeader("\"Content-Type:application/json\"");

    std::string tempOutputFile = this->work_dir + _FILE_SEPARATOR + REST_OUTPUT_FILE;
    rst.setConsoleOutputFile(tempOutputFile);

    rst.setCloudResponseFile(gwDataSyncFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_DATASYNC;
    buffer += to_string(gwId);
    rst.setUrl(buffer.c_str());

    std::string built_rest = rst.buildRest();

    int ret_val = system(built_rest.c_str());

    return ret_val;
}

int CloudConnector::discoverSensors() {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Starting Sensors Discovering");
    isDiscovering = true;

    IBPayload payload;
    payload.setType("command");
    payload.setValue("GET");
    payload.setCvalue("LIST");
    payload.setContent("");

    IBMessage ibmessage;
    Poco::Timestamp now;
    ibmessage.setId(pluginDetails.pluginName);
    ibmessage.setPayload(payload);
    ibmessage.setReference("");
    ibmessage.setTimestamp(now.epochTime());
    this->busClient->sendMessage(ibmessage);

    //timerDiscoverSensors.start(Poco::TimerCallback<CloudConnector>(*this, & CloudConnector::onDiscoverSensorsEnd));
}

/*
void CloudConnector::onDiscoverSensorsEnd(Timer& timer) {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Discover Sensors ended");
    isDiscovering = false;
}
*/

int CloudConnector::sendDiscoveredSensors(int gwId, std::string status, std::string sensors, std::string cloudResFile)
{
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_PUT);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);

    buffer = "\"{\\\"status\\\":\\\"" + status + "\\\",\\\"sensors\\\":[" + sensors + "]}\"";
    rst.setDataBody(buffer.c_str());

    rst.setHeader("\"Content-Type:application/json\"");

    std::string tempOutputFile = this->work_dir + _FILE_SEPARATOR + REST_OUTPUT_FILE;
    rst.setConsoleOutputFile(tempOutputFile);

    rst.setCloudResponseFile(cloudResFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_DISCOVERED_SENSORS;
    buffer += to_string(gwId);
    rst.setUrl(buffer.c_str());

    std::string built_rest = rst.buildRest();

cout << "built rest = << " << built_rest << ">>\n";

    int ret_val = system(built_rest.c_str());

    return ret_val;
}

int CloudConnector::connectSensors(std::string serialList) {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Starting Connecting Sensors");

    IBPayload payload;
    payload.setType("command");
    payload.setValue("GET");
    payload.setCvalue("PROPERTIES");
    payload.setContent("");

    IBMessage ibmessage;
    Poco::Timestamp now;
    ibmessage.setId(pluginDetails.pluginName);
    ibmessage.setPayload(payload);
    ibmessage.setReference("");
    ibmessage.setTimestamp(now.epochTime());

    std::vector<std::string> serials;
    Poco::JSON::Parser parser;
    try {
        Poco::Dynamic::Var result = parser.parse(payload.getContent());
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        object->getNames(serials);
        std::string target;
        std::map<std::string, std::string>::iterator it;
        for (unsigned int i = 0; i < serials.size(); ++i) {
            logger.debug("Connect sensor request with Request Serial[%u] = %s", i, serials[i]);

            it = discoveredSensors.find(serials[i]);
            if (it != discoveredSensors.end()) {
                target = it->second + "/" + serials[i];
                this->busClient->sendMessage(ibmessage, target);
            } else {
                logger.debug("ERROR - Cloud is trying connect unknown device {%s}", serials[i]);
            }
        }
    } catch(Poco::Exception excp) {
        logger.debug("ERROR - EMPTY SERIALS LIST   <<%s>>", payload.getContent());
    }

    this->busClient->sendMessage(ibmessage);

}


int CloudConnector::sendConnectedSensor(int gwId, std::string serial, std::string properties, std::string cloudResFile) {
    std::string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_PUT);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);

    Poco::JSON::Object obj;
    std::stringstream strFormat;
    
    obj.set("serial", serial);
    obj.set("success", true);
    obj.set("errorMessage", std::string());
    obj.set("properties", properties);
    obj.stringify(strFormat);

    rst.setDataBody(strFormat.str());

    rst.setHeader("\"Content-Type:application/json\"");

    std::string tempOutputFile = this->work_dir + _FILE_SEPARATOR + REST_OUTPUT_FILE;
    rst.setConsoleOutputFile(tempOutputFile);

    rst.setCloudResponseFile(cloudResFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_DISCOVERED_SENSORS;
    buffer += to_string(gwId);
    rst.setUrl(buffer.c_str());

    std::string built_rest = rst.buildRest();

cout << "built rest = << " << built_rest << ">>\n";

    int ret_val = system(built_rest.c_str());

    return ret_val;
}

int CloudConnector::storeDiscoveredDevices(std::string pluginSource, std::string sensorsList) {
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    Poco::JSON::Parser parser;
/*
    try {
        Poco::Dynamic::Var result = parser.parse(sensorsList);
        Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();    
        Poco::JSON::Object::Ptr object;
        unsigned int index = 0;

        object = arr->getObject(index);
        while (!object->isNull()) {
            logger.debug("############# EXTRACTED OBJECT [%u] <<%s>>", index, object.convert<std::string>());

            std::string serial;
            discoveredSensors[serial] = pluginSource;
            ++index;
            object = arr->getObject(index);
        }
    }
    */
}

