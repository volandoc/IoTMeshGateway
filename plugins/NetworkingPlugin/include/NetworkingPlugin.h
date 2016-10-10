#ifndef NETWORKING_PLUGIN_H
#define NETWORKING_PLUGIN_H

#include "pluginsapi.h"

class NetworkingPlugin: public ucl::plugins::UCLPluginIf {
public:
    NetworkingPlugin();
    virtual ~NetworkingPlugin();

    virtual int startPlugin();
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();

};

#endif // NETWORKING_PLUGIN_H
