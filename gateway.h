#ifndef GATEWAY_H
#define GATEWAY_H

#include <string.h>
#include "mqttclient.h"
#include "Managers/managersapi.h"

using namespace std;

class gateway: public gw::managers::GWManagerListenerIF{
public:
    gateway(void);
    gateway(const char *iFN, const char *confFolder);
    virtual ~gateway(void);
    int start(void);
    int stop(void);

    virtual int sendToCloud();
    virtual int notifyListener(string);
    virtual int errorHandler();
    virtual int commandResult();

private:
    void loadConfig();

    const char *initFileName;
    const char *CONFIGURATION_FOLDER;
    mqttclient *mqttClient;
    const char *gatewayId;
    const char *mqttHost;
    int mqttPort;
};

#endif // GATEWAY_H
