#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mosquittopp.h>
#include <iostream>
#include "mqttclientconfig.h"

struct mosquittoConfig {
    const char *id;
    const char *id_prefix;
    const char *host;
    const char *bind_address;
    const char *username;
    const char *password;
    const char *will_topic;
    const char *will_payload;
    int   protocol_version;
    int   keepalive;
    int   port;
    int   qos;
    bool  retain;
    bool  debug;
    bool  quiet;
    unsigned int max_inflight;
    long  will_payloadlen;
    int   will_qos;
    bool  will_retain;
    bool  clean_session; /* sub */
    std::string topics[]; /* sub */
    int   topic_count; /* sub */
    bool  no_retain; /* sub */
    std::string filter_outs; /* sub */
    int   filter_out_count; /* sub */
    bool  verbose; /* sub */
    bool  eol; /* sub */
    int   sub_msg_count; /* sub */
    int   pub_msg_count;
#ifdef WITH_SRV
    bool use_srv;
#endif
#ifdef WITH_TLS
    char *cafile;
    char *capath;
    char *certfile;
    char *keyfile;
    char *ciphers;
    bool  insecure;
    char *tls_version;
#  ifdef WITH_TLS_PSK
    char *psk;
    char *psk_identity;
#  endif
#endif
#ifdef WITH_SOCKS
    char *socks5_host;
    int   socks5_port;
    char *socks5_username;
    char *socks5_password;
#endif
};

class mqttclient : public mosqpp::mosquittopp
{
public:
    mqttclient(const char * id, const char * host, int port=MQTT_PORT, int keepalive=MQTT_KEEPALIVE, bool clean_session=true, int max_inflight=0, bool eol=true, int protocol_version=MQTT_PROTOCOL_V311);
    ~mqttclient();

    int do_connect_async();
    int do_disconnect();
    int do_subscribe(const int count, const char **topics, int qos=-1);
    int do_unsubscribe(const int count, const char *topics[]);
    int do_publish(const char *topic, const char *message=NULL, int qos=-1);
    int set_will(const char *topic, const char *message=NULL, int qos=-1);
    int clean_will();
    int config_load();
    int config_load_from_file();
    void topics_init(int gwId, int homeId);

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_publish(int mid);
    void on_unsubscribe(int mid);
    void on_error();
    void on_disconnect(int rc);

    bool is_onboarded;

private:
    void config_init(struct mosquittoConfig *cfg);
    void config_cleanup();

    bool disconnected_by_user;
    struct mosquittoConfig config;

    std::string subTopicSensorActuartorCmd;
    std::string subTopicScenarioExecCmnd;
    std::string subTopicGwCmd;

    std::string pubTopicSensorActuartorOccur;
    std::string pubTopicSensorActuartorError;
};

#endif // MQTTCLIENT_H
