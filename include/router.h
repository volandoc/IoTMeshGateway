#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include "mqttclient.h"
#include "pluginsapi.h"

using namespace std;

class router: public ucl::plugins::NotificationListenerIF{
public:
    router(void);
    router(const char *iFN, const char *confFolder);
    virtual ~router(void);
    int start(void);
    int stop(void);

    virtual int sendToCloud();
    virtual int notificationArived(string);
    virtual int errorHandler();
    virtual int commandResult();

private:
    void loadConfig();
    const char *initFileName;
    const char *CONFIGURATION_FOLDER;
    mqttclient *mqttClient;
    const char *routerId;
    const char *mqttHost;
    int mqttPort;
    int gwId;
};

#endif // ROUTER_H
