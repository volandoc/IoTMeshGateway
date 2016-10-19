#ifndef INNERBUSSAPI_H
#define INNERBUSSAPI_H

#define UCL_IBUS_API_VERSION 1

#include "pluginsapi.h"

class InnerBusClientIF {
public:
    virtual ~InnerBusClientIF();
    virtual void init() = 0;
    virtual int connect() = 0;
    virtual int connect_async() = 0;
    virtual int disconnect() = 0;
    virtual int sendMessage(std::string message) = 0;
    virtual void setConfig(void *config) = 0;
    virtual void getInfo() = 0;
};

struct BusDetails {
    int apiVersion;
};

class InnerBusIF {
public:
    virtual InnerBusClientIF* createIBusClient() = 0;
	virtual int loadConfig() = 0;
	virtual bool isAvailable() = 0;
	virtual int getConnectionsCount() = 0;
	virtual int getClientsCount() = 0;
    virtual void getInfo() = 0;
};


#endif // INNERBUSSAPI_H
