#ifndef INNERBUS_H
#define INNERBUS_H

#include <iostream>
#include <string>
#include <mosquitto.h>
#include <Poco/String.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include "innerbusapi.h"
#include "pluginsapi.h"

struct mosquitto_config {
    std::string id;
    std::string host;
    std::string will_topic;
    std::string will_payload;
    std::string subtopic;
    std::string pubtopic;
    int   keepalive;
    int   port;
    int   qos;
    int   will_qos;
    bool  clean_session;
    int msg_count;
    std::string cafile;
    std::string capath;
    std::string certfile;
    std::string keyfile;
    std::string ciphers;
    bool  insecure;
    std::string tls_version;
    std::string psk;
    std::string psk_identity;
};

class InnerBusClient: public InnerBusClientIF {
private:
    UCLPluginIf* callbackObj=NULL;
    mosquitto *m_mosq;
    mosquitto_config cfg;
    bool disconnected_by_user = false;
    bool connected = false;

    int reinitialise();
    int username_pw_set(const char *username, const char *password);
    int reconnect();
    int reconnect_async();
    void reconnect_delay_set(unsigned int reconnect_delay, unsigned int reconnect_delay_max, bool reconnect_exponential_backoff);
    int max_inflight_messages_set(unsigned int max_inflight_messages);
    void message_retry_set(unsigned int message_retry);
    int loop_start();
    int loop_stop(bool force=true);
    int publish(std::string message);
    int tls_set(const char *cafile, const char *capath, const char *certfile, const char *keyfile, int (*pw_callback)(char *buf, int size, int rwflag, void *userdata));
    int tls_opts_set(int cert_reqs, const char *tls_version, const char *ciphers);
    int tls_insecure_set(bool value);
    int tls_psk_set(const char *psk, const char *identity, const char *ciphers);
    int will_set();
    int will_clear();
    int subscribe();
    int unsubscribe();

public:
    InnerBusClient();
    virtual~InnerBusClient();

    virtual void init();
    virtual int connect();
    virtual int connect_async();
    virtual int disconnect();
    virtual void setListener(void *listener);
    virtual int sendMessage(std::string message);
    virtual void setConfig(void *config);
    virtual void getInfo();

    mosquitto * getMQTTInst();

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_unsubscribe(int mid);
    void on_log(int level, const char *str);
    void on_error();
};

class InnerBus: public InnerBusIF {
    std::string coreLibPath;
    mosquitto_config defaultConf;
public:
    InnerBus();
    InnerBus(std::string path);
    virtual ~InnerBus();
    virtual int loadConfig();
    virtual bool isAvailable();
    virtual InnerBusClientIF* createIBusClient();
    virtual int getConnectionsCount();
    virtual int getClientsCount();
    virtual void getInfo();
};

POCO_BEGIN_MANIFEST(InnerBusIF)
    POCO_EXPORT_SINGLETON(InnerBus)
POCO_END_MANIFEST

#endif // INNERBUS_H
