#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "../managersapi.h"

class NetworkManager: public gw::managers::GWManagerIf{
public:
    NetworkManager();
    virtual ~NetworkManager();

    virtual int startManager();
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopManager();

};

#endif // NETWORKMANAGER_H
