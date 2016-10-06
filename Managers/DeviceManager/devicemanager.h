#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "../managersapi.h"

class DeviceManager: public gw::managers::GWManagerIf{
public:
    DeviceManager();
    virtual ~DeviceManager();

    virtual int startManager();
    virtual int stopManager();

};

#endif // DEVICEMANAGER_H
