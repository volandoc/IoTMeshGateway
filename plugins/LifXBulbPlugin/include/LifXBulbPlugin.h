#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "pluginsapi.h"

class LifXBulbPlugin: public ucl::plugins::UCLPluginIf{
public:
    LifXBulbPlugin();
    virtual ~LifXBulbPlugin();

    virtual int startPlugin();
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();

};

#endif // DEVICEMANAGER_H
