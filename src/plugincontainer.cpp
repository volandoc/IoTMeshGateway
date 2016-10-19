#include "plugincontainer.h"
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Glob.h>

int PluginContainer::LoadPlugins() {
    int result = 0;
    Poco::File pluginDir(this->pluginDirPath);

    if( pluginDir.exists() && pluginDir.isDirectory() ) {
        std::set<std::string> files;
        Poco::Glob::glob(this->pluginDirPath + "/*/*.so",files);
        std::set<std::string>::iterator it = files.begin();
        for (; it != files.end(); ++it) {
            std::cout << *it << std::endl;
            this->LoadPlugin(*it);
        }
    } else {
        result = -1;
    }

    return result;
}

std::string PluginContainer::generatePluginUri(std::string pname){
    return this->pluginDirPath + "/" + pname;
}

int PluginContainer::LoadPlugin(std::string pname) {

    try{
        this->pluginLoader.loadLibrary(pname);
    } catch(Poco::Exception excp){
        std::cout << excp.displayText() << std::endl;
        return -1;
    }

    std::cout << (this->pluginLoader.isLibraryLoaded(pname)? "Loaded" : " Not Loaded") << std::endl;
//    PluginLoader::Iterator it(pluginLoader.begin());
//    for( ; it != pluginLoader.end(); ++it){
//        std::cout << "lib path: " << it->first << std::endl;
//    }
    return 0;
}

std::string PluginContainer::GetPluginsPath() {
    return this->pluginDirPath;
}

std::list<std::string> PluginContainer::GetPluginsList() {
    std::list<std::string> result;
    return result;
}

std::map<std::string, UCLPluginIf*> PluginContainer::GetLoadedPlugins() {
    return this->loadedPlugins;
}

std::map<std::string, int> PluginContainer::GetFailedPlugins() {
    return this->failedPlugins;
}

bool PluginContainer::HasLoadErrors() {
    return this->lerror;
}

int PluginContainer::unloadPlugins(){
    return 0;
}

PluginContainer::~PluginContainer(){

}
