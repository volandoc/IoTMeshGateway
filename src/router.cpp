#include <iostream>
#include <string.h>
#include "router.h"
#include "mqttclient.h"
#include "easylogging++.h"
#include "rest.h"

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

#include <unistd.h>


int router::start(void){
    loadConfig();
    int protocol_version = MQTT_PROTOCOL_V311;

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
