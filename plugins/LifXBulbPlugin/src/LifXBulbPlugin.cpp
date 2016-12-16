#include "LifXBulbPlugin.h"
#include "messaging.h"
#include <iostream>

LifXBulbPlugin::LifXBulbPlugin() {
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    this->pluginDetails.type = _PD_T_DEVICE;
    this->pluginDetails.apiVersion = UCL_PLUGINS_API_VERSION;
    this->pluginDetails.className = "LifXBulbPlugin";
    this->pluginDetails.pluginName ="LifXBulb Plugin";
    this->pluginDetails.pluginVersion = "0.0.1";

    this->pollingTimer.setStartInterval(POLLING_START_INTERVAL);
    this->pollingTimer.setPeriodicInterval(POLLING_INTERVAL);

    logger.debug("Plugin Created");
}

LifXBulbPlugin::~LifXBulbPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("Plugin Removed");
    if( busClient != NULL ) {
        delete busClient;
        busClient = NULL;
    }
}

int LifXBulbPlugin::startPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    if(this->busClient!=NULL){
        this->busClient->init();
        this->busClient->connect_async();
        logger.debug("Started");
        pollingTimer.start(Poco::TimerCallback<LifXBulbPlugin>(*this, & LifXBulbPlugin::doPolling));
        return 0;
    } else {
        logger.error("No IBus Client found: can't start", __FILE__, 26);
        return -1;
    }
}

void LifXBulbPlugin::doPolling(Poco::Timer& timer){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");

    LifxMessage *message = new GetServiceMessage();
    message->sendMessage();

    logger.debug("Polling message sent");

    delete message;
}

int LifXBulbPlugin::setIBusClient(InnerBusClientIF* client){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    this->busClient = client;
    this->busClient->setListener(this);
    this->busClient->getInfo();
    return 0;
}

int LifXBulbPlugin::setWorkDir(std::string path){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("Running from %s", path);
    this->work_dir = path;
    return 0;
}

int LifXBulbPlugin::executeCommand(std::string source, IBMessage message){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    IBPayload payload = message.getPayload();
    logger.debug("executeCommand for {%s} msg{%s}", source, payload.toJSON());

    logger.debug("\"%s : %s : %s : %d\"", message.getId(), payload.toJSON(), message.getReference(), (int) message.getTimestamp());
    //logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());

    Poco::StringTokenizer t1(source, "/");
    if(4 < t1.count()) {
        sendOccurrence(false, "ERRORMSG", "Wrong message recepient, not proccessed", message.getId());
        return 0;
    }

    if(payload.getType().compare("command")) {
        sendOccurrence(false, "ERRORMSG", "Wrong message type, not command", message.getId());
        return 0;
    }

    if( !payload.getValue().compare("GET") ) {
        if(4 == t1.count()) {
            //proccessDeviceGetCommand(payload.getContent(), t1[3]);
            sendOccurrence(true, "TESTMSG", "GET command for device must be proccessed", message.getId());
        } else {
            //proccessPluginGetCommand(payload.getContent(), t1[3]);
            sendOccurrence(true, "TESTMSG", "GET command for plugin must be proccessed", message.getId());
        }
    } else if( !payload.getValue().compare("SET") ) {
        if(4 == t1.count()) {
            //proccessDeviceSetCommand(payload.getContent(), t1[3]);
            sendOccurrence(true, "TESTMSG", "SET command for device must be proccessed", message.getId());
        } else {
            //proccessPluginSetCommand(payload.getContent(), t1[3]);
            sendOccurrence(true, "TESTMSG", "SET command for plugin must be proccessed", message.getId());
        }
    } else {
        sendOccurrence(false, "ERRORMSG", "Wrong command value, not proccessed", message.getId());
        return 0;
    }

    return 0;
}

int LifXBulbPlugin::executeInternalCommand(std::string source, std::string message){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    return 0;
}

int LifXBulbPlugin::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference) {
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("sendOccurrence \"%b : %s : %s : %s\"", success, cvalue, content, reference);

    IBPayload payload;
    payload.setType("event");
    payload.setValue((success?"SUCCESS":"FAILED"));
    payload.setCvalue(cvalue);
    payload.setContent(content);

    IBMessage ibmessage;
    Poco::Timestamp now;
    ibmessage.setId(pluginDetails.pluginName + reference);
    ibmessage.setPayload(payload);
    ibmessage.setReference(reference);
    ibmessage.setTimestamp(now.epochTime());

    logger.debug("sendOccurrence: message prepared for sending");
    busClient->sendMessage(ibmessage);

    return 0;
}

int LifXBulbPlugin::getCommandSet(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("getCommandSet");
    return 0;
}

int LifXBulbPlugin::getCapabilitiesSet(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("getCapabilitiesSet");
    return 0;
}

PluginDetails* LifXBulbPlugin::getPluginDetails(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    return &pluginDetails;
}

int LifXBulbPlugin::stopPlugin(){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");

    pollingTimer.stop();

    logger.debug("Stopped");
    return 0;
}
