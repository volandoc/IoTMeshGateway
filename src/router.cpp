#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <streambuf>
#include "router.h"
#include "mqttclient.h"
#include "easylogging++.h"
#include "rest.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#define GW_SERIAL                       "987654321"
#define GW_VERSION                      "2"
#define GW_MDN                          "123456789"

#define GW_ID_FILE_PARAMETER_GW_ID      "gatewayId"
#define GW_ID_FILE_PARAMETER_IS_ONBOARD "isOnboarded"


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;


bool provision(int& gwId);
bool sendProvision(string serial, string version, string mdn, string cloudResFile);
bool readFileContent(string fileName, string&  fileContent);
bool getGwId(string strJson, int& gwId);
bool getIsOnboarded(string strJson, bool& isOnboarded);
int sendGetDataSync(int gwId, string gwDataSyncFile);


router::router(void){
    this->mqttHost = MQTT_HOST;
    this->routerId = "embgw123";
    this->mqttPort = MQTT_PORT;
    this->CONFIGURATION_FOLDER = "conf";
    this->initFileName = "";
}

router::router(const char *iFN, const char *confFolder){
    this->CONFIGURATION_FOLDER = confFolder;
    this->initFileName = iFN;
}

router::~router(void){
}

void router::loadConfig(){

}


int router::start(void){
    loadConfig();
    int protocol_version = MQTT_PROTOCOL_V311;

    while (false == provision(this->gwId))
    {
        LOG(INFO) << "Gateway provision failed, retrying in 1 s...";
        sleep(1);
    }
    LOG(INFO) << "Gateway provision SUCCESS";

    this->mqttClient = new mqttclient(  this->routerId,
                                        this->mqttHost,
                                        this->mqttPort,
                                        60,
                                        true,
                                        0,
                                        0,
                                        MQTT_PROTOCOL_V311);

    if(this->mqttClient->tls_set(   MQTT_ROOT_CA_FILENAME,
                                    MQTT_CERT_DIR,
                                    MQTT_CERTIFICATE_FILENAME,
                                    MQTT_PRIVATE_KEY_FILENAME,
                                    NULL))
    {
        // TODO handle Error: Problem setting TLS options.\n";
        return 1;
    }

    if(this->mqttClient->tls_insecure_set(true))
    {
        // TODO handle Error: Problem setting TLS insecure option.\n";
        return 1;
    }

    if(this->mqttClient->tls_opts_set( 1, MQTT_TLS_VER, NULL)){
        // TODO handle Error: Problem setting TLS options
        return 1;
    }

    if(this->mqttClient->max_inflight_messages_set(20))
    {
        // TODO handle Error: Problem setting max inflight messages option.\n";
        return 1;
    }
    if(this->mqttClient->opts_set(MOSQ_OPT_PROTOCOL_VERSION, (void*)(&protocol_version)))
    {
        // TODO handle Error: Problem setting protocol version.\n";
        return 1;
    }

    mqttClient->do_connect_async();

    sendGetDataSync(this->gwId, REST_DATASYNC_FILE);

    return 0;
}

int router::sendToCloud(){
    return 0;
}

int router::notificationArived(string message){
    LOG(INFO) << "router Notified: " << message;
    return 0;
}

int router::errorHandler(){
    return 0;
}

int router::commandResult(){
    return 0;
}

int router::stop(void){

    mqttClient->do_disconnect();
    delete mqttClient;

    return 0;
}

bool provision(int& gwId)
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
        status = getGwId(cloudResponse, gwId);
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



bool sendProvision(string serial, string version, string mdn, string cloudResFile)
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

bool readFileContent(string fileName, string&  fileContent)
{
    ifstream steamFile(fileName.c_str());
    if (steamFile.is_open())
    {
        string strFileContent;

        steamFile.seekg(0, ios::end);
        strFileContent.reserve(steamFile.tellg());
        steamFile.seekg(0, ios::beg);

        strFileContent.assign((istreambuf_iterator<char>(steamFile)),
                                istreambuf_iterator<char>());

        fileContent = strFileContent;
        return true;
    }
    return false;
}

bool getGwId(string strJson, int& gwId)
{
    bool retVal = false;
    if (0 != strJson.length())
    {
        int temp = 0;
        Parser parser;
        Var result = parser.parse(strJson);
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

bool getIsOnboarded(string strJson, bool& isOnboarded)
{
    bool retVal = false;
    if (0 != strJson.length())
    {
        Parser parser;
        Var result = parser.parse(strJson);
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


int sendGetDataSync(int gwId, string gwDataSyncFile)
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

