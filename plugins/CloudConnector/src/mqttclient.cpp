#include <mosquittopp.h>
#include <iostream>
#include <string.h>
#include "mqttclient.h"
#include "JSON_messages.h"

#define BUFFER_SIZE     1024

mqttclient::mqttclient(const char *id, const char *host, int port, int keepalive, bool clean_session, int max_inflight, bool eol, int protocol_version)
    : mosquittopp(id, clean_session)
    , is_onboarded(true)
{
    config.id = id;
    config.host = host;
    config.port = port;
    config.max_inflight = max_inflight;
    config.keepalive = keepalive;
    config.clean_session = clean_session;
    config.eol = eol;
    config.protocol_version = protocol_version;
    config.pub_msg_count=0;
    disconnected_by_user = false;
    mosqpp::lib_init();
}

mqttclient::~mqttclient(){
    mosqpp::lib_cleanup();    // Mosquitto library cleanup
}

void mqttclient::config_init(struct mosquittoConfig *cfg){
    config.id = cfg->id;
    config.port = cfg->port;
    config.max_inflight = cfg->max_inflight;
    config.keepalive = cfg->keepalive;
    config.clean_session = cfg->clean_session;
    config.eol = cfg->eol;
    config.protocol_version = cfg->protocol_version;
}

int mqttclient::config_load(){
    return MOSQ_ERR_SUCCESS;
}

void mqttclient::config_cleanup(){

}

void mqttclient::setCallback(UCLPluginIf *callbackObj){
    this->callbackObj = callbackObj;
}

void mqttclient::topics_init(int gwId, int homeId){
    char buffer[BUFFER_SIZE];

    // init subscribe topics
    snprintf(buffer, BUFFER_SIZE, MQTT_TOPIC_SUB_SENSOR_ACTUATOR_CMD, homeId, gwId);
    this->subTopicSensorActuartorCmd = buffer;
    memset(buffer, 0, BUFFER_SIZE);

    this->subTopicScenarioExecCmnd = MQTT_TOPIC_SUB_SCENARIO_EXEC_CMD;

    snprintf(buffer, BUFFER_SIZE, MQTT_TOPIC_SUB_GW_CMD, gwId);
    this->subTopicGwCmd = buffer;
    memset(buffer, 0, BUFFER_SIZE);

    // init publish topics
    snprintf(buffer, BUFFER_SIZE, MQTT_TOPIC_PUB_SENSOR_ACTUATOR_OCCUR, homeId, gwId);
    this->pubTopicSensorActuartorOccur = buffer;
    memset(buffer, 0, BUFFER_SIZE);

    snprintf(buffer, BUFFER_SIZE, MQTT_TOPIC_PUB_SENSOR_ACTUATOR_ERROR, homeId, gwId);
    this->pubTopicSensorActuartorError = buffer;
    memset(buffer, 0, BUFFER_SIZE);
}

int mqttclient::do_connect_async(){
    int rc = 0;
    rc = connect_async(config.host, config.port, config.keepalive);
    this->loop_start();
    return rc;
}

int mqttclient::do_disconnect(){
    int rc;
    disconnected_by_user = true;
    rc = disconnect();
    rc = this->loop_stop();
    return rc;
}

int mqttclient::do_subscribe(const int count, const char *topics[], int qos){
    int sub_qos = (qos >= 0 && qos <= 3)? qos : config.qos;
    int rc = MOSQ_ERR_SUCCESS;
    for(int i=0; i<count; i++){
        config.sub_msg_count++;
        std::cout << ">> myMosq - try subscribe to Topic: \"" << topics[i] << "\"\n";
        rc = subscribe(&(config.sub_msg_count), topics[i], sub_qos);
        if(rc != MOSQ_ERR_SUCCESS){
            break;
        }
    }
    return rc;
}

int mqttclient::do_unsubscribe(const int count, const char *topics[]){
    int rc = MOSQ_ERR_SUCCESS;
    for(int i=0; i<count; i++){
        config.sub_msg_count++;
         rc = unsubscribe(&(config.sub_msg_count), topics[count]);
        if(rc!=MOSQ_ERR_SUCCESS){
            break;
        }
     }
    return rc;
}

int mqttclient::do_publish(const char *topic, const char *message, int qos){
    if(strlen(message)==0) return MOSQ_ERR_PAYLOAD_SIZE;
    int pub_qos = (qos >= 0 && qos <= 3)? qos : config.qos;
    config.pub_msg_count++;
    int rc = publish(&(config.pub_msg_count), topic, strlen(message), message, pub_qos, false);
    return rc;
}

void mqttclient::on_connect(int rc){
    if( rc == 0 ){
        std::cout << ">> myMosq - connected with server\n";

        const char *topics[]={
            this->subTopicSensorActuartorCmd.c_str(),
            this->subTopicScenarioExecCmnd.c_str(),
            this->subTopicGwCmd.c_str()};
        int topics_count = 3;

        do_subscribe(topics_count, topics, 1);
    } else {
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")";
    }
}

void mqttclient::on_disconnect(int rc){
    std::cout << ">> myMosq - disconnection(" << rc << ")";
    if(!disconnected_by_user){
        this->reconnect_async();
    }
}

void mqttclient::on_subscribe(int mid, int qos_count, const int *granted_qos){
    std::cout << ">> myMosq - Topic(" << mid << ")(" << qos_count << ")(" << granted_qos << ") subscribed\n";

    int i;

    int rc;
    char *msg = NULL;
    for(i = 0; i < qos_count; i++){
        std::cout << ", granted_qos " << granted_qos[i];
    }
    std::cout << "\n";
}

void mqttclient::on_unsubscribe(int mid){
    std::cout << ">> myMosq - Topic(" << mid << ") unsubscribed";
}

void mqttclient::on_message(const struct mosquitto_message *message){

    int msize=message->payloadlen/sizeof(char);
    char messagearr[msize];
    strcpy(messagearr , (char*)message->payload);

    std::string messagestr(messagearr);

    std::cout << ">> myMosq - Message(" << message->mid << ") on Topic(" << message->topic << ") with payload <" << messagestr << ">\n";

    if (!strcmp(message->topic, subTopicGwCmd.c_str())){
        this->callbackObj->executeCommand(messagestr);
    }
}

void mqttclient::on_publish(int mid){
    std::cout << ">> myMosq - Message(" << mid << ") succeed to be published";
}

void mqttclient::on_error(){

}
