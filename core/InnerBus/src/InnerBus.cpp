#include "InnerBus.h"

static void on_connect_wrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_connect(rc);
}

static void on_disconnect_wrapper(struct mosquitto *mosq, void *userdata, int rc) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_disconnect(rc);
}

static void on_publish_wrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_publish(mid);
}

static void on_message_wrapper(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_message(message);
}

static void on_subscribe_wrapper(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_subscribe(mid, qos_count, granted_qos);
}

static void on_unsubscribe_wrapper(struct mosquitto *mosq, void *userdata, int mid) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_unsubscribe(mid);
}

static void on_log_wrapper(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    class InnerBusClient *m = (class InnerBusClient *)userdata;
    m->on_log(level, str);
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

const char* strerror(int mosq_errno){
    return mosquitto_strerror(mosq_errno);
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
    //this->callbackObj = callback;
    std::cout << "InnerBus client created" << std::endl;
}

InnerBusClient::~InnerBusClient(){
    //lib_cleanup();
    //mosquitto_destroy(m_mosq);
    std::cout << "InnerBus client destroyed" << std::endl;
    //this->callbackObj = NULL;
}

//
//Private section of InnerBusClient
//

int InnerBusClient::reinitialise()
{
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

int InnerBusClient::loop_start()
{
    return mosquitto_loop_start(m_mosq);
}

int InnerBusClient::loop_stop(bool force)
{
    return mosquitto_loop_stop(m_mosq, force);
}

int InnerBusClient::publish(std::string message) {
    cfg.msg_count++;
    return mosquitto_publish(m_mosq, &(cfg.msg_count), cfg.pubtopic.c_str(), message.length(), message.c_str(), cfg.qos, false);
}

int InnerBusClient::tls_set(const char *cafile, const char *capath, const char *certfile, const char *keyfile, int (*pw_callback)(char *buf, int size, int rwflag, void *userdata))
{
    return mosquitto_tls_set(m_mosq, cafile, capath, certfile, keyfile, pw_callback);
}

int InnerBusClient::tls_opts_set(int cert_reqs, const char *tls_version, const char *ciphers)
{
    return mosquitto_tls_opts_set(m_mosq, cert_reqs, tls_version, ciphers);
}

int InnerBusClient::tls_insecure_set(bool value)
{
    return mosquitto_tls_insecure_set(m_mosq, value);
}

int InnerBusClient::tls_psk_set(const char *psk, const char *identity, const char *ciphers)
{
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
    this->cfg = *(struct mosquitto_config*)config;

    //PluginDetails* plgDetails = callbackObj->getPluginDetails();
    //std::string plgName(plgDetails->pluginName);
    //cfg.id = plgName;
    //cfg.pubtopic = plgName + "/" + cfg.pubtopic;
    //cfg.subtopic = plgName + "/" + cfg.subtopic;
    //cfg.will_payload = plgName + ": " + cfg.will_payload;
    //cfg.msg_count = 0;
    std::cout << "InnerBus client config setted" << std::endl;
}

int InnerBusClient::connect() {
    int rc = mosquitto_connect(m_mosq, cfg.host.c_str(), cfg.port, cfg.keepalive);
    rc = this->loop_start();
    if( rc == MOSQ_ERR_SUCCESS ) connected = true;
    return rc;
}

int InnerBusClient::connect_async() {
    int rc = mosquitto_connect_async(m_mosq, cfg.host.c_str(), cfg.port, cfg.keepalive);
    rc = this->loop_start();
    if( rc == MOSQ_ERR_SUCCESS ) connected = true;
    return rc;
}

int InnerBusClient::disconnect() {
    int rc;
    disconnected_by_user = true;
    rc = mosquitto_disconnect(m_mosq);
    rc = this->loop_stop();
    return rc;
}

int InnerBusClient::sendMessage(std::string message){
    return publish(message);
}

void InnerBusClient::getInfo() {
    std::cout << "MQTT InnerBus client info: " << std::endl;
    std::cout << "mqtt.host: " << cfg.host << std::endl;
    std::cout << "mqtt.port: " << cfg.port << std::endl;
    std::cout << "mqtt.subtopic: " << cfg.subtopic << std::endl;
    std::cout << "mqtt.pubtopic: " << cfg.pubtopic << std::endl;
}
//
//
//


InnerBus::InnerBus() {
    std::cout << "InnerBus default Created" << std::endl;
    this->coreLibPath="../core/InnerBus";
    loadConfig();
}

InnerBus::InnerBus(std::string path) {
    std::cout << "InnerBus Created" << std::endl;
    this->coreLibPath = path;
    loadConfig();
}

InnerBus::~InnerBus() {
    std::cout << "InnerBus Destroyed";
}

int InnerBus::loadConfig() {
    std::string confPath = this->coreLibPath + "/" + "config.properties";

    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConf;
    pConf = new Poco::Util::PropertyFileConfiguration(confPath);

    defaultConf.host = pConf->getString("mqtt.host");
    defaultConf.will_topic = pConf->getString("mqtt.willtopic");
    defaultConf.will_payload = pConf->getString("mqtt.willpayload");
    defaultConf.subtopic = pConf->getString("mqtt.subtopic");
    defaultConf.pubtopic = pConf->getString("mqtt.pubtopic");
    defaultConf.keepalive = pConf->getInt("mqtt.keepalive");
    defaultConf.port = pConf->getInt("mqtt.port");
    defaultConf.qos = pConf->getInt("mqtt.qos");
    defaultConf.will_qos = pConf->getInt("mqtt.willqos");
    defaultConf.clean_session = pConf->getBool("mqtt.cleansession");
    std::cout << "InnerBus Config Loaded" << std::endl;
}

bool InnerBus::isAvailable(){
    return true;
}

InnerBusClientIF* InnerBus::createIBusClient(){
    std::cout << "InnerBus enter client creation" << std::endl;
    InnerBusClientIF* client = new InnerBusClient;
    client->setConfig(&defaultConf);
    std::cout << "InnerBus leave client creation" << std::endl;
    return client;
}

int InnerBus::getConnectionsCount(){
    return 0;
}

int InnerBus::getClientsCount(){
    return 0;
}

void InnerBus::getInfo(){
    std::cout << "InnerBus - MQTT implementatioan with TLS/SSL support" << std::endl;
}
