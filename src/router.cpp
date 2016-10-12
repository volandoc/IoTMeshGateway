#include <iostream>
#include <string.h>
#include "router.h"
#include "mqttclient.h"
#include "easylogging++.h"

router::router(void){
    this->mqttHost = "localhost";
    this->routerId = "embgw123";
    this->mqttPort = 1883;
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
    const char *topics[]={"Test", "Test2"};
    int topics_count=2;

    this->mqttClient = new mqttclient(this->routerId, this->mqttHost);

    mqttClient->do_connect_async();
    mqttClient->do_subscribe(topics_count, topics, 1);
    mqttClient->do_publish("Test", "{'test':'message'}", 0);

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
