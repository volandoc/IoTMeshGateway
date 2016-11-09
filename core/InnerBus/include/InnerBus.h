#ifndef INNERBUS_H
#define INNERBUS_H

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <mosquitto.h>
#include <Poco/String.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include "innerbusapi.h"
#include "pluginsapi.h"

class InnerBus;

struct mosquitto_config {
    std::string prefix;
    std::string id;
    std::string host;
    std::string will_topic;
    std::string will_payload;
    std::string command_topic;
    std::string occurrence_topic;
    std::vector<std::string> subTopicList;
    std::vector<std::string> pubTopicList;
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
    InnerBus* ptrIBus=nullptr;
    UCLPluginIf* callbackObj=nullptr;
    mosquitto *m_mosq=nullptr;
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
    int sendStatus(std::string status);
    int subscribe();
    int unsubscribe();
    int generateSubTopics();
    int generatePubTopics();

public:
    InnerBusClient(InnerBus* innerBus);
    virtual~InnerBusClient();

    virtual void init();
    virtual void free();
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
    mosquitto_config defaultConf;
    int connectionsCount = 0;
    std::list<std::string> clientsList;
public:
    InnerBus();
    virtual ~InnerBus();
    virtual int loadConfig(std::string libpath = "../core/InnerBus");
    virtual int setRoot(std::string serial);
    virtual bool isAvailable();
    virtual InnerBusClientIF* createIBusClient();
    virtual int getConnectionsCount();
    virtual int getClientsCount();
    virtual void getInfo();

    void addClientToList(std::string name);
    void delClientFromList(std::string);
};

POCO_BEGIN_MANIFEST(InnerBusIF)
    POCO_EXPORT_SINGLETON(InnerBus)
POCO_END_MANIFEST

#endif // INNERBUS_H
