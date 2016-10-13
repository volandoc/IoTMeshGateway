#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mosquittopp.h>
#include <string>
#include "mqttclientconfig.h"

struct mosquitto_config {
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

//int client_config_load(struct mosq_config *config, int pub_or_sub, int argc, char *argv[]);
//void client_config_cleanup(struct mosq_config *cfg);
//int client_opts_set(struct mosquitto *mosq, struct mosq_config *cfg);
//int client_id_generate(struct mosq_config *cfg, const char *id_base);
//int client_connect(struct mosquitto *mosq, struct mosq_config *cfg);


class mqttclient : public mosqpp::mosquittopp
{
public:
    mqttclient(const char * id, const char * host, int port=1883, int keepalive=60, bool clean_session=true, int max_inflight=20, bool eol=true, int protocol_version=MQTT_PROTOCOL_V31);
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

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_publish(int mid);
    void on_unsubscribe(int mid);
    void on_error();
    void on_disconnect(int rc);

private:
    bool is_onboarded;

    void config_init(struct mosquitto_config *cfg);
    void config_cleanup();

    bool disconnected_by_user;
    struct mosquitto_config config;

};

#endif // MQTTCLIENT_H
