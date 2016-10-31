#include "InnerBus.h"

static void on_connect_wrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_connect(rc);
    }
}

static void on_disconnect_wrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_disconnect(rc);
    }
}

static void on_publish_wrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_publish(mid);
    }
}

static void on_message_wrapper(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_message(message);
    }
}

static void on_subscribe_wrapper(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_subscribe(mid, qos_count, granted_qos);
    }
}

static void on_unsubscribe_wrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_unsubscribe(mid);
    }
}

static void on_log_wrapper(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    if( mosq == m->getMQTTInst()) {
        m->on_log(level, str);
    }
}

int lib_version(int *major, int *minor, int *revision)
{
    if(major) *major = LIBMOSQUITTO_MAJOR;
    if(minor) *minor = LIBMOSQUITTO_MINOR;
    if(revision) *revision = LIBMOSQUITTO_REVISION;
    return LIBMOSQUITTO_VERSION_NUMBER;
}

int lib_init(){
    return mosquitto_lib_init();
}

int lib_cleanup(){
    return mosquitto_lib_cleanup();
}

const char* connack_string(int connack_code) {
    return mosquitto_connack_string(connack_code);
}

int sub_topic_tokenise(const char *subtopic, char ***topics, int *count){
    return mosquitto_sub_topic_tokenise(subtopic, topics, count);
}

int sub_topic_tokens_free(char ***topics, int count){
    return mosquitto_sub_topic_tokens_free(topics, count);
}

int topic_matches_sub(const char *sub, const char *topic, bool *result){
    return mosquitto_topic_matches_sub(sub, topic, result);
}

InnerBusClient::InnerBusClient(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.debug("InnerBus client created");
}

InnerBusClient::~InnerBusClient(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    if(connected) disconnect();
    this->loop_stop();
    lib_cleanup();
    //mosquitto_destroy(m_mosq);
    logger.debug("InnerBus client destroyed");
    callbackObj = NULL;
}

//
//Private section of InnerBusClient
//

int InnerBusClient::reinitialise() {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    int rc;
    rc = mosquitto_reinitialise(m_mosq, cfg.id.c_str(), cfg.clean_session, this);
    if(rc == MOSQ_ERR_SUCCESS){
        mosquitto_connect_callback_set(m_mosq, on_connect_wrapper);
        mosquitto_disconnect_callback_set(m_mosq, on_disconnect_wrapper);
        mosquitto_publish_callback_set(m_mosq, on_publish_wrapper);
        mosquitto_message_callback_set(m_mosq, on_message_wrapper);
        mosquitto_subscribe_callback_set(m_mosq, on_subscribe_wrapper);
        mosquitto_unsubscribe_callback_set(m_mosq, on_unsubscribe_wrapper);
        mosquitto_log_callback_set(m_mosq, on_log_wrapper);
    }
    logger.debug("InnerBus client <%s> reinitialized:", this->cfg.id);
    return rc;
}

int InnerBusClient::username_pw_set(const char *username, const char *password) {
    return mosquitto_username_pw_set(m_mosq, username, password);
}

int InnerBusClient::reconnect() {
    return mosquitto_reconnect(m_mosq);
}

int InnerBusClient::reconnect_async() {
    return mosquitto_reconnect_async(m_mosq);
}

void InnerBusClient::reconnect_delay_set(unsigned int reconnect_delay, unsigned int reconnect_delay_max, bool reconnect_exponential_backoff) {
    mosquitto_reconnect_delay_set(m_mosq, reconnect_delay, reconnect_delay_max, reconnect_exponential_backoff);
}

int InnerBusClient::max_inflight_messages_set(unsigned int max_inflight_messages) {
    return mosquitto_max_inflight_messages_set(m_mosq, max_inflight_messages);
}

void InnerBusClient::message_retry_set(unsigned int message_retry) {
    mosquitto_message_retry_set(m_mosq, message_retry);
}

int InnerBusClient::loop_start() {
    return mosquitto_loop_start(m_mosq);
}

int InnerBusClient::loop_stop(bool force) {
    return mosquitto_loop_stop(m_mosq, force);
}

int InnerBusClient::publish(std::string message) {
    cfg.msg_count++;
    return mosquitto_publish(m_mosq, &(cfg.msg_count), cfg.pubtopic.c_str(), message.length(), message.c_str(), cfg.qos, false);
}

int InnerBusClient::tls_set(const char *cafile, const char *capath, const char *certfile, const char *keyfile, int (*pw_callback)(char *buf, int size, int rwflag, void *userdata)) {
    return mosquitto_tls_set(m_mosq, cafile, capath, certfile, keyfile, pw_callback);
}

int InnerBusClient::tls_opts_set(int cert_reqs, const char *tls_version, const char *ciphers) {
    return mosquitto_tls_opts_set(m_mosq, cert_reqs, tls_version, ciphers);
}

int InnerBusClient::tls_insecure_set(bool value) {
    return mosquitto_tls_insecure_set(m_mosq, value);
}

int InnerBusClient::tls_psk_set(const char *psk, const char *identity, const char *ciphers) {
    return mosquitto_tls_psk_set(m_mosq, psk, identity, ciphers);
}

int InnerBusClient::will_set() {
    return mosquitto_will_set(m_mosq, cfg.will_topic.c_str(), cfg.will_payload.length(), cfg.will_payload.c_str(), cfg.will_qos, false);
}

int InnerBusClient::will_clear() {
    return mosquitto_will_clear(m_mosq);
}

int InnerBusClient::subscribe() {
    cfg.msg_count++;
    return mosquitto_subscribe(m_mosq, &(cfg.msg_count), cfg.subtopic.c_str(), cfg.qos);
}

int InnerBusClient::unsubscribe() {
    cfg.msg_count++;
    return mosquitto_unsubscribe(m_mosq, &(cfg.msg_count), cfg.subtopic.c_str());
}

//
//Public section of InnerBusClient
//

void InnerBusClient::init(){
    m_mosq = mosquitto_new(cfg.id.c_str(), cfg.clean_session, this);
    mosquitto_connect_callback_set(m_mosq, on_connect_wrapper);
    mosquitto_disconnect_callback_set(m_mosq, on_disconnect_wrapper);
    mosquitto_publish_callback_set(m_mosq, on_publish_wrapper);
    mosquitto_message_callback_set(m_mosq, on_message_wrapper);
    mosquitto_subscribe_callback_set(m_mosq, on_subscribe_wrapper);
    mosquitto_unsubscribe_callback_set(m_mosq, on_unsubscribe_wrapper);
    mosquitto_log_callback_set(m_mosq, on_log_wrapper);
    will_set();
    lib_init();
}

void InnerBusClient::setConfig(void *config){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->cfg = *(struct mosquitto_config*)config;

    std::string plgName="Default";
    cfg.id = cfg.id + plgName;
    cfg.pubtopic = plgName + "/" + cfg.pubtopic;
    cfg.subtopic = plgName + "/" + cfg.subtopic;
    cfg.will_payload = plgName + ": " + cfg.will_payload;
    cfg.msg_count = 0;
    logger.debug("InnerBus client config setted");
}

void InnerBusClient::setListener(void *listener){
    if(listener!=NULL){
        callbackObj = (class UCLPluginIf *)listener;
        cfg.id = Poco::replace(cfg.id, "Default", callbackObj->getPluginDetails()->className);
        cfg.subtopic = Poco::replace(cfg.subtopic, "Default", callbackObj->getPluginDetails()->className);
        cfg.pubtopic = Poco::replace(cfg.pubtopic, "Default", callbackObj->getPluginDetails()->className);
        cfg.will_payload = Poco::replace(cfg.will_payload, "Default", callbackObj->getPluginDetails()->className);
    }
}

int InnerBusClient::connect() {
    int rc = mosquitto_connect(m_mosq, cfg.host.c_str(), cfg.port, cfg.keepalive);
    rc = this->loop_start();
    return rc;
}

int InnerBusClient::connect_async() {
    int rc = mosquitto_connect_async(m_mosq, cfg.host.c_str(), cfg.port, cfg.keepalive);
    rc = this->loop_start();
    return rc;
}

int InnerBusClient::disconnect() {
    int rc;
    disconnected_by_user = true;
    connected = false;
    rc = mosquitto_disconnect(m_mosq);
    return rc;
}

mosquitto * InnerBusClient::getMQTTInst(){
    return this->m_mosq;
}

int InnerBusClient::sendMessage(std::string message){
    return publish(message);
}

void InnerBusClient::getInfo() {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.information(":\n\
    %s InnerBus client info:\n\
        mqtt.host: %s\n\
        mqtt.port: %d\n\
        mqtt.subtopic: %s\n\
        mqtt.pubtopic: %s\n", \
    cfg.id, cfg.host, cfg.port, cfg.subtopic, cfg.pubtopic);
}

void InnerBusClient::on_connect(int rc) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    if( rc == 0 ){
        logger.debug("%s- connected with server", cfg.id);
        connected = true;
        this->subscribe();
        this->publish("available");
    } else {
        logger.debug("%s - Impossible to connect with server (%d)", cfg.id, rc);
        connected = false;
    }
}

void InnerBusClient::on_disconnect(int rc) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    connected=false;

    if(!disconnected_by_user){
        logger.debug("%s- disconnection (%d), trying to reconnect", cfg.id, rc);
        this->reconnect_async();
    } else {
        logger.debug("%s- Manualy disconnected  no reconnection needed", cfg.id, rc);
    }
}

void InnerBusClient::on_publish(int mid) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    logger.debug("%s- Message(%d) succeed to be published", cfg.id, mid);;
}

void InnerBusClient::on_message(const struct mosquitto_message *message) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    int msize=message->payloadlen/sizeof(char);
    char messagearr[msize];
    strcpy(messagearr , (char*)message->payload);

    std::string messagestr(messagearr);

    logger.debug("%s- Message(%d) on Topic(%s) with payload: %s", cfg.id, message->mid, message->topic, messagestr);

    this->callbackObj->executeInternalCommand(messagestr);
}

void InnerBusClient::on_subscribe(int mid, int qos_count, const int *granted_qos) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.debug("%s- Topic(%d)(%d)(%d) subscribed", cfg.id, mid, qos_count, granted_qos);
}

void InnerBusClient::on_unsubscribe(int mid) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    logger.debug("%s- Topic(%d) unsubscribed", cfg.id, mid);
}

void InnerBusClient::on_log(int level, const char *str) {
    //Poco::Logger& logger = Poco::Logger::get("InnerBus");

    //logger.debug("%s- LogEvent(%d) with message: %s", cfg.id, level, str);
}

void InnerBusClient::on_error() {
    return;
}

//
//
//

InnerBus::InnerBus() {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.debug("InnerBus default Created");
}

InnerBus::~InnerBus() {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.debug("InnerBus Destroyed");
}

int InnerBus::loadConfig(std::string libpath) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    std::string confPath = libpath + "/" + "config.properties";

    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConf;
    pConf = new Poco::Util::PropertyFileConfiguration(confPath);

    defaultConf.id = pConf->getString("mqtt.idprefix");
    defaultConf.host = pConf->getString("mqtt.host");
    defaultConf.port = pConf->getInt("mqtt.port");
    defaultConf.will_topic = pConf->getString("mqtt.willtopic");
    defaultConf.will_payload = pConf->getString("mqtt.willpayload");
    defaultConf.subtopic = pConf->getString("mqtt.subtopic");
    defaultConf.pubtopic = pConf->getString("mqtt.pubtopic");
    defaultConf.keepalive = pConf->getInt("mqtt.keepalive");
    defaultConf.qos = pConf->getInt("mqtt.qos");
    defaultConf.will_qos = pConf->getInt("mqtt.willqos");
    defaultConf.clean_session = pConf->getBool("mqtt.cleansession");
    logger.debug("InnerBus Config Loaded");

    return 0;
}

bool InnerBus::isAvailable(){
    return true;
}

InnerBusClientIF* InnerBus::createIBusClient(){
    InnerBusClientIF* client = new InnerBusClient;
    client->setConfig(&defaultConf);
    return client;
}

int InnerBus::getConnectionsCount(){
    return 0;
}

int InnerBus::getClientsCount(){
    return 0;
}

void InnerBus::getInfo(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    logger.information("InnerBus - MQTT implementatioan with TLS/SSL support");
}
