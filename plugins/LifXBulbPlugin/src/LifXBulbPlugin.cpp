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
    LifxMessage message;
    message.acknowledgeRequired(true);
    message.responseRequired(true);
    message.sendMessage("");
    Poco::Logger& logger = Poco::Logger::get("CloudConnector");
    logger.debug("Gateway provision failed, retrying in 5 s...");
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
    logger.debug("executeCommand for {%s} msg{%s}", source, message.getPayload());

    logger.debug("\"%s : %s : %s : %d\"", message.getId(), message.getPayload(), message.getReference(), (int) message.getTimestamp());

    IBPayload payload;
    if(payload.fromJSON(message.getPayload()))
        logger.debug("\"%s : %s : %s : %s\"", payload.getType(), payload.getValue(), payload.getCvalue(), payload.getContent());

    Poco::StringTokenizer t1(source, "/");
    if(4 < t1.count()) {
        sendOccurrence(false, "ERRORMSG", "Wrong message recepient, not proccessed", message.getId());
        return 0;
    }

    sendOccurrence(true, "TESTMSG", "It is a simple test message", message.getId());

    return 0;
}

int LifXBulbPlugin::executeInternalCommand(std::string source, std::string message){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
    logger.debug("executeInternalCommand from {%s} msg{%s}", source, message);

    return 0;
}

int LifXBulbPlugin::sendOccurrence(bool success, std::string cvalue, std::string content, std::string reference) {
    IBPayload payload;
    payload.setType("occurence");
    payload.setValue((success?"SUCCESS":"FAILED"));
    payload.setCvalue(cvalue);
    payload.setContent(content);

    IBMessage ibmessage;
    Poco::Timestamp now;
    ibmessage.setId(pluginDetails.pluginName + reference);
    ibmessage.setPayload(payload.toJSON());
    ibmessage.setReference(reference);
    ibmessage.setTimestamp(now.epochTime());

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
