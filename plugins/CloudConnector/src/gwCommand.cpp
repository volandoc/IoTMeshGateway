#include <iostream>
#include "gwCommand.h"

#define GW_COMMAND_KEY_ID           "id"
#define GW_COMMAND_KEY_REQUEST_ID   "requestId"
#define GW_COMMAND_KEY_EVENT_TYPE   "eventType"
#define GW_COMMAND_KEY_PAYLOAD      "payload"
#define GW_COMMAND_KEY_TIMESTAMP    "timestamp"



gwCommand::gwCommand(std::string json)
{
    this->id = get(json, GW_COMMAND_KEY_ID);
    this->requestId = get(json, GW_COMMAND_KEY_REQUEST_ID);
    this->eventType = get(json, GW_COMMAND_KEY_EVENT_TYPE);
    this->payload = get(json, GW_COMMAND_KEY_PAYLOAD);
    std::string strTimestamp = get(json, GW_COMMAND_KEY_TIMESTAMP);

    int tempTimestamp = 0;
    try
    {
        tempTimestamp = stoi(strTimestamp);
        this->timestamp = tempTimestamp;
    }
    catch(...)
    {
        this->timestamp = 0;
    }

}

std::string gwCommand::getId()
{
    return this->id;
}

std::string gwCommand::getRequestId()
{
    return this->requestId;
}
std::string gwCommand::getEventType()
{
    return this->eventType;
}
std::string gwCommand::getPayload()
{
    return this->payload;
}
int gwCommand::getTimestamp()
{
    return this->timestamp;
}

