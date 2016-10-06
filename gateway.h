#ifndef GATEWAY_H
#define GATEWAY_H

#include <string.h>
#include "mqttclient.h"

using namespace std;

class gateway
{
public:
    gateway(void);
    gateway(const char *iFN, const char *confFolder);
    ~gateway(void);
    int start(void);
    int stop(void);

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
