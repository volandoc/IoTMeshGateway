#include "gateway.h"
#include "mqttclient.h"
#include <string.h>

gateway::gateway(void){
    this->mqttHost = "localhost";
    this->gatewayId = "embgw123";
    this->mqttPort = 1883;
    this->CONFIGURATION_FOLDER = "conf";
    this->initFileName = "";
}

gateway::gateway(const char *iFN, const char *confFolder){
    this->CONFIGURATION_FOLDER = confFolder;
    this->initFileName = iFN;
}

gateway::~gateway(void){
}

void gateway::loadConfig(){

}

int gateway::start(void){
    loadConfig();
    const char *topics[]={"Test", "Test2"};
    int topics_count=2;

    this->mqttClient = new mqttclient(this->gatewayId, this->mqttHost);

    mqttClient->do_connect_async();
    mqttClient->do_subscribe(topics_count, topics, 1);
    mqttClient->do_publish("Test", "{'test':'message'}", 0);

    return 0;
}

int gateway::stop(void){

    mqttClient->do_disconnect();
    delete mqttClient;

    return 0;
}
