#ifndef INNERBUSSAPI_H
#define INNERBUSSAPI_H

#define UCL_IBUS_API_VERSION 1

#include "innerbusclientapi.h"
#include "pluginsapi.h"

struct BusDetails {
    int apiVersion;
};

class InnerBusIF {
public:
    virtual InnerBusClientIF* createIBusClient() = 0;
    virtual int loadConfig(std::string libpath = "../core/InnerBus") = 0;
	virtual bool isAvailable() = 0;
	virtual int getConnectionsCount() = 0;
	virtual int getClientsCount() = 0;
    virtual void getInfo() = 0;
};


#endif // INNERBUSSAPI_H
