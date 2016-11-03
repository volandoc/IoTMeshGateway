#ifndef GW_COMMAND_H
#define GW_COMMAND_H

#include <string>
#include "parseCommand.h"

#define GW_COMMAND_EVENT_SYNCDATA           "syncData"
#define GW_COMMAND_EVENT_DISCOVERSENSORS    "discoverSensors"
#define GW_COMMAND_EVENT_CONNECTSENSORS     "connectSensors"
#define GW_COMMAND_EVENT_UPDATEPLUGINS      "updatePlugins"
#define GW_COMMAND_EVENT_UPDATEFIRMWARE     "updateFirmware"
#define GW_COMMAND_EVENT_REBOOT             "reboot"
#define GW_COMMAND_EVENT_RESET              "reset"
#define GW_COMMAND_EVENT_BACKUP             "backup"
#define GW_COMMAND_EVENT_RESTORE            "restore"

class gwCommand: private parseCommand {
public:
    gwCommand(std::string json);

    std::string getId();
    std::string getRequestId();
    std::string getEventType();
    std::string getPayload();
    int getTimestamp();

private:
    std::string id;
    std::string requestId;
    std::string eventType;
    std::string payload;
    int timestamp;
};

#endif // GW_COMMAND_H
