#ifndef LIFX_BULB_PLUGIN_H
#define LIFX_BULB_PLUGIN_H

#include <Poco/ClassLibrary.h>
#include "pluginsapi.h"

class LifXBulbPlugin: public UCLPluginIf{
private:
    PluginDetails pluginDetails;
public:
    LifXBulbPlugin();
    virtual ~LifXBulbPlugin();

    virtual int startPlugin();
    virtual int executeCommand();
    virtual int getCommandSet();
    virtual int getCapabilitiesSet();
    virtual int stopPlugin();
    virtual PluginDetails* getPluginDetails();
};

POCO_BEGIN_MANIFEST(UCLPluginIf)
    POCO_EXPORT_SINGLETON(LifXBulbPlugin)
POCO_END_MANIFEST

#endif // LIFX_BULB_PLUGIN_H
