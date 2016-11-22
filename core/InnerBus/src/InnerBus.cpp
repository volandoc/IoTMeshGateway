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

InnerBusClient::InnerBusClient(InnerBus *innerBus){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->ptrIBus = innerBus;
    logger.debug("InnerBus client created");
}

InnerBusClient::~InnerBusClient(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->free();
    ptrIBus->delClientFromList(cfg.id);
    callbackObj = nullptr;
    ptrIBus = nullptr;
    logger.debug("InnerBus client destroyed");
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

int InnerBusClient::publish(std::string topic, std::string message) {
    int rc = 0;
    cfg.msg_count++;

    rc = mosquitto_publish(m_mosq, &(cfg.msg_count), topic.c_str(), message.length(), message.c_str(), cfg.qos, false);

    return rc;
}


int InnerBusClient::sendCommand(IBMessage message) {
    int rc = 0;

    if(cfg.id == cfg.prefix){
    }

    if( nullptr != callbackObj ) {
        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_DEVICE) ){
        }

        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_COMM) ){
            std::list<std::string> pluginsList = ptrIBus->getClientsList();
            std::list<std::string>::iterator pluginsItr = pluginsList.begin();
            for(; pluginsItr != pluginsList.end(); pluginsItr++){
                std::string name = *pluginsItr;
                if( !name.compare(cfg.id) ){
                    std::string tmpTopic = cfg.prefix + "/" + name + "/" + cfg.command_topic;
                    rc = publish(tmpTopic, message.toJSON());
                }
            }
        }
    }

    return rc;
}

int InnerBusClient::sendEvent(IBMessage message) {
    int rc = 0;

    if(cfg.id == cfg.prefix){
    }

    if( nullptr != callbackObj ) {
        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_DEVICE) ){
            std::string tmpTopic = cfg.prefix + "/" + cfg.id + "/" + cfg.occurrence_topic;
            rc = publish(tmpTopic, message.toJSON());
        }

        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_COMM) ){
            std::string tmpTopic = cfg.prefix + "/" + cfg.id + "/" + cfg.occurrence_topic;
            rc = publish(tmpTopic, message.toJSON());
        }
    }

    return rc;
}

int InnerBusClient::sendStatus(std::string status) {
    return mosquitto_publish(m_mosq, &(cfg.msg_count), cfg.will_topic.c_str(), status.length(), status.c_str(), cfg.will_qos, true);
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
    return mosquitto_will_set(m_mosq, cfg.will_topic.c_str(), cfg.will_payload.length(), cfg.will_payload.c_str(), cfg.will_qos, true);
}

int InnerBusClient::will_clear() {
    return mosquitto_will_clear(m_mosq);
}

int InnerBusClient::subscribe() {
    for(size_t count=0; count < cfg.subTopicList.size(); count++){
        cfg.msg_count++;
        mosquitto_subscribe(m_mosq, &(cfg.msg_count), cfg.subTopicList[count].c_str(), cfg.qos);
    }
    return 0;
}

int InnerBusClient::unsubscribe() {
    for(size_t count=0; count < cfg.subTopicList.size(); count++){
        cfg.msg_count++;
        mosquitto_unsubscribe(m_mosq, &(cfg.msg_count), cfg.subTopicList[count].c_str());
    }
    return 0;
}

int InnerBusClient::generateSubTopics() {
    std::string tmpTopic;

    if(cfg.id == cfg.prefix){
        tmpTopic = cfg.prefix + "/" + cfg.occurrence_topic;
        cfg.subTopicList.push_back(tmpTopic);
        tmpTopic = cfg.prefix + "/" + cfg.command_topic;
        cfg.subTopicList.push_back(tmpTopic);
        return 0;
    }

    if( nullptr != callbackObj ) {
        cfg.subTopicList.clear();

        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_DEVICE) ){
            tmpTopic = cfg.prefix + "/" + cfg.id + "/" + cfg.command_topic;
            cfg.subTopicList.push_back(tmpTopic);
            tmpTopic = cfg.prefix + "/" + cfg.id + "/+/" + cfg.command_topic;
            cfg.subTopicList.push_back(tmpTopic);
            return 0;
        }

        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_COMM) ){
            tmpTopic = cfg.prefix + "/+/" + cfg.occurrence_topic;
            cfg.subTopicList.push_back(tmpTopic);
            tmpTopic = cfg.prefix + "/+/+/" + cfg.occurrence_topic;
            cfg.subTopicList.push_back(tmpTopic);
            return 0;
        }
    }

    return -1;
}

int InnerBusClient::generatePubTopics() {
    std::string tmpTopic;

    if(cfg.id == cfg.prefix){
        tmpTopic = cfg.prefix +"/{PLUGIN}/" + cfg.command_topic;
        cfg.pubTopicList.push_back(tmpTopic);
        return 0;
    }

    if( nullptr != callbackObj ) {
        cfg.pubTopicList.clear();

        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_DEVICE) ){
            tmpTopic = cfg.prefix + "/" + cfg.id + "/" + cfg.occurrence_topic;
            cfg.pubTopicList.push_back(tmpTopic);
            return 0;
        }
        if( (callbackObj->getPluginDetails()->type != NULL)
             && !strcmp(callbackObj->getPluginDetails()->type, _PD_T_COMM) ){
            tmpTopic = cfg.prefix +"/{PLUGIN}/" + cfg.command_topic;
            cfg.pubTopicList.push_back(tmpTopic);
            return 0;
        }
    }

    return -1;
}

//
//Public section of InnerBusClient
//

void InnerBusClient::init(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    m_mosq = mosquitto_new(cfg.id.c_str(), cfg.clean_session, this);
    if( nullptr != m_mosq){
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
    logger.debug("InnerBus client initialized");
}

void InnerBusClient::free(){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    if(connected) disconnect();
    if( nullptr != m_mosq){
        lib_cleanup();
        mosquitto_destroy(m_mosq);
        m_mosq = nullptr;
    }
    logger.debug("InnerBus client deinitialized");
}

void InnerBusClient::setConfig(void *config){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->cfg = *(struct mosquitto_config*)config;

    cfg.id = cfg.prefix;
    cfg.will_topic = cfg.prefix + "/" + cfg.will_topic;
    cfg.msg_count = 0;
    generateSubTopics();
    generatePubTopics();

    logger.debug("InnerBus client config setted");
}

void InnerBusClient::setListener(void *listener){
    if(listener!=NULL){
        callbackObj = (class UCLPluginIf *)listener;
        cfg.id = callbackObj->getPluginDetails()->className;
        generateSubTopics();
        generatePubTopics();
        cfg.will_topic = Poco::replace(cfg.will_topic, cfg.prefix, (cfg.prefix + "/" + cfg.id));
        ptrIBus->addClientToList(cfg.id);
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
    sendStatus(cfg.will_payload);
    rc = mosquitto_disconnect(m_mosq);
    this->loop_stop();
    return rc;
}

mosquitto * InnerBusClient::getMQTTInst(){
    return this->m_mosq;
}

int InnerBusClient::sendMessage(IBMessage message){
    IBPayload payload;
    payload.fromJSON(message.getPayload());

    if(payload.getType().compare("command")){
        return sendCommand(message);
    }

    if(payload.getType().compare("event")){
        return sendEvent(message);
    }
}

void InnerBusClient::getInfo() {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    std::stringstream infoStream;
    infoStream << ":\n"
               << cfg.id
               << "        InnerBus client info:\n"
               << "        mqtt.host: " << cfg.host << "\n"
               << "        mqtt.port: " << cfg.port << "\n"
               << "        mqtt.subscribed to:\n";
    for( size_t i=0; i<cfg.subTopicList.size(); i++){
        infoStream << "            " << cfg.subTopicList[i] << "\n";
    }
    infoStream << "        mqtt.publish to:\n";
    for( size_t i=0; i<cfg.pubTopicList.size(); i++){
        infoStream << "            " << cfg.pubTopicList[i] << "\n";;
    }

    logger.debug(infoStream.str());

}

void InnerBusClient::on_connect(int rc) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    if( rc == 0 ){
        logger.debug("%s- connected with server", cfg.id);
        connected = true;
        this->subscribe();
        this->sendStatus("available");
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

    if((cfg.id != cfg.prefix) && (nullptr!=this->callbackObj) ){
        int msize=message->payloadlen/sizeof(char);
        char messagearr[msize];
        strcpy(messagearr , (char*)message->payload);

        std::string messagestr(messagearr);

        IBMessage ibmessage;

        logger.debug("%s- Message(%d) on Topic(%s) with payload: %s", cfg.id, message->mid, message->topic, messagestr);

        if(ibmessage.fromJSON(messagestr)) {
            this->callbackObj->executeCommand(message->topic, ibmessage);
        }
    }
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
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    std::string messagestr(str);

    logger.trace("%s- LogEvent(%d) with message: %s", cfg.id, level, messagestr);
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

    defaultConf.prefix = pConf->getString("mqtt.idprefix");
    defaultConf.host = pConf->getString("mqtt.host");
    defaultConf.port = pConf->getInt("mqtt.port");
    defaultConf.will_topic = pConf->getString("mqtt.willtopic");
    defaultConf.will_payload = pConf->getString("mqtt.willpayload");
    defaultConf.occurrence_topic = pConf->getString("mqtt.occurrence_topic");
    defaultConf.command_topic = pConf->getString("mqtt.command_topic");
    defaultConf.keepalive = pConf->getInt("mqtt.keepalive");
    defaultConf.qos = pConf->getInt("mqtt.qos");
    defaultConf.will_qos = pConf->getInt("mqtt.willqos");
    defaultConf.clean_session = pConf->getBool("mqtt.cleansession");
    logger.debug("InnerBus Config Loaded");

    return 0;
}

int InnerBus::setRoot(std::string serial) {
    Poco::Logger& logger = Poco::Logger::get("InnerBus");

    defaultConf.prefix = serial;
    logger.debug("InnerBus Root topic setted: %s", serial);

    return 0;
}


bool InnerBus::isAvailable(){
    return true;
}

InnerBusClientIF* InnerBus::createIBusClient(){
    InnerBusClientIF* client = new InnerBusClient(this);
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

void InnerBus::addClientToList(std::string name){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->clientsList.push_back(name);
    logger.information("InnerBus - client %s added", name);
}

void InnerBus::delClientFromList(std::string name){
    Poco::Logger& logger = Poco::Logger::get("InnerBus");
    this->clientsList.remove(name);
    logger.information("InnerBus - client %s removed", name);
}

std::list<std::string> InnerBus::getClientsList() {
    return clientsList;
}
