#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include <string>

class PluginContainer
{
private:
    const std::string pluginDirPath;
public:
    PluginContainer(std::string path="plugins");
};

#endif // PLUGINCONTAINER_H
