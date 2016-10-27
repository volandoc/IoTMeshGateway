#include "CloudConnector.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <streambuf>
#include "rest.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <pthread.h>

#define GW_SERIAL                       "987654321"
#define GW_VERSION                      "2"
#define GW_MDN                          "123456789"

#define GW_ID_FILE_PARAMETER_GW_ID      "gatewayId"
#define GW_ID_FILE_PARAMETER_IS_ONBOARD "isOnboarded"


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;

static void runConnectionThread(void *pArg);
static void* connectionMain(void *pArg);


CloudConnector::CloudConnector()
    : gatewayId(0)
    , homeId(0)
{
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "CloudConnector";
    this->pluginDetails.pluginName ="CloudConnector Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";

    initMqttClient();

    logger.debug("Plugin Created");
}

CloudConnector::~CloudConnector(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    if( NULL != this->busClient ) {
        delete busClient;
        busClient = NULL;
    }

    if( this->mqttClient != NULL ) {
        this->mqttClient->do_disconnect();
        delete this->mqttClient;
        this->mqttClient = nullptr;
    }

    logger.debug("Plugin Removed");
}

int CloudConnector::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
        logger.debug("Started");

        runConnectionThread((void*)this);
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }
}

int CloudConnector::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();

    return 0;
}

int CloudConnector::executeCommand(){
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("executeCommand");
    return 0;
}

int CloudConnector::sendOccurrence (std::string message) {
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
    bool isOnboarded;
    string cloudResponse;

    if (false == sendProvision(GW_SERIAL, GW_VERSION, GW_MDN, REST_PROVISION_FILE))
    {
        status = false;
    }

    if (true == status)
    {
        status = readFileContent(REST_PROVISION_FILE, cloudResponse);
    }

    if (true == status)
    {
        status = getGwId(cloudResponse, this->gatewayId);
    }

    if (true == status)
    {
        status = getIsOnboarded(cloudResponse, isOnboarded);
    }
    if (true == status)
    {
        status = isOnboarded;
    }

    return status;
}

bool CloudConnector::sendProvision(string serial, string version, string mdn, string cloudResFile)
{
    string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_PUT);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);

    buffer = "\"{\\\"serial\\\":\\\"" + serial + "\\\",\\\"version\\\":\\\"" + version + "\\\",\\\"mdn\\\":\\\"+" + mdn + "\\\"}\"";
    rst.setDataBody(buffer.c_str());

    rst.setHeader("\"Content-Type:application/json\"");
    rst.setConsoleOutputFile(REST_OUTPUT_FILE);
    rst.setCloudResponseFile(cloudResFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_PROVISION;
    buffer += serial;
    rst.setUrl(buffer.c_str());

    string built_rest = rst.buildRest();

    int retVal = system(built_rest.c_str());

    if (0 == retVal)
    {
        return true;
    }
    return false;
}

bool CloudConnector::readFileContent(string fileName, string&  fileContent)
{
    ifstream streamFile(fileName.c_str());
    if (streamFile.is_open())
    {
        string strFileContent;

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

bool CloudConnector::getGwId(string provisionJson, int& gwId)
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
            gwId = temp;
            retVal = true;
        }
        catch(...)
        {
            retVal = false;
        }
    }
    return retVal;
}

bool CloudConnector::getIsOnboarded(string provisionJson, bool& isOnboarded)
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
            isOnboarded = true;
            retVal = true;
        }
        else if ("false" == isOnboardedValue)
        {
            isOnboarded = false;
            retVal = true;
        }
        else
        {
            retVal = false;
        }
    }
    return retVal;
}

bool CloudConnector::getHomeId(string datasyncJson, int& homeId)
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
            homeId = temp;
            retVal = true;
        }
        catch(...)
        {
            retVal = false;
        }
    }
    return retVal;
}

int CloudConnector::sendGetDataSync(int gwId, string gwDataSyncFile)
{
    string buffer;
    rest rst;
    rst.setMethod(REST_METHOD_GET);
    rst.setCertificate(REST_CERTIFICATE);
    rst.setPrivatekey(REST_PRIVATEKEY);
    rst.setNocheckCert(true);
    rst.setHeader("\"Content-Type:application/json\"");
    rst.setConsoleOutputFile(REST_OUTPUT_FILE);
    rst.setCloudResponseFile(gwDataSyncFile);

    buffer = REST_HOST;
    buffer += REST_RESOURCE_DATASYNC;
    buffer += to_string(gwId);
    rst.setUrl(buffer.c_str());

    string built_rest = rst.buildRest();

    int ret_val = system(built_rest.c_str());

    return ret_val;
}

static void* connectionMain(void *pArg)
{
    CloudConnector* clonector = (CloudConnector*)pArg;
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    bool status = true;

    if (true == clonector->initMqttClient())
    {
        while (status)
        {
            while (false == clonector->provision()) {
                logger.debug("Gateway provision failed, retrying in 1 s...");
                sleep(1);
            }
            logger.debug("Gateway provision SUCCESS");

            clonector->sendGetDataSync(clonector->gatewayId, REST_DATASYNC_FILE);

            string datasyncJson;
            if (true == clonector->readFileContent(REST_DATASYNC_FILE, datasyncJson) &&
                true == clonector->getHomeId(datasyncJson, clonector->homeId))
            {
                //TODO store dataSync from REST_DATASYNC_FILE flie
                cout << "######## GatewayID = " << clonector->gatewayId << " HomeID = " << clonector->homeId << "\n";

                clonector->mqttClient->topics_init(clonector->gatewayId, clonector->homeId);
                clonector->mqttClient->do_connect_async();
            }
            else
            {
                logger.error("Error: Failed to get Home ID.");
                status = false;
            }

            while (status && true == clonector->mqttClient->is_onboarded)
            {
                sleep(1);
            }
            clonector->mqttClient->is_onboarded = true;
            logger.debug("Gateway detached");
            clonector->mqttClient->do_disconnect();
        }
    }

	return 0;
}

static void runConnectionThread(void *pArg)
{
    pthread_t thConnector = 0;

    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thConnector, &threadAttr, connectionMain, pArg);
}
