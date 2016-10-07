#ifndef PLUGINSAPI_H
#define PLUGINSAPI_H

#include <string>

namespace ucl {
namespace plugins {

#define UCL_PLUGINS_API_VERSION 1

#define UCL_PLUGIN_STANDART_INFO \
    UCL_PLUGINS_API_VERSION,     \
    __FILE__

#ifdef WIN32
#  ifdef UCL_PLUGINS_EXPORTS
#    define UCL_PLUGIN_EXPORT __declspec(dllexport)
#  else
#    define UCL_PLUGIN_EXPORT __declspec(dllimport)
#  endif
#else
#   define UCL_PLUGIN_EXPORT // empty
#endif

class NotificationListenerIF{
public :
    virtual int sendToCloud()=0;
    virtual int notificationArived(std::string message)=0;
    virtual int errorHandler()=0;
    virtual int commandResult()=0;
};

class UCLPluginIf{
private:
    NotificationListenerIF* pluginsListener;
public:
    virtual void setListener(NotificationListenerIF* mngrListener){
        this->pluginsListener=mngrListener;
    }

    virtual int notifyListener(std::string message){
        this->pluginsListener->notificationArived(message);
        return 0;
    }

    virtual int startPlugin()=0;
    virtual int executeCommand()=0;
    virtual int getCommandSet()=0;
    virtual int getCapabilitiesSet()=0;
    virtual int stopPlugin()=0;
};

UCL_PLUGIN_EXPORT typedef UCLPluginIf* (*GetPluginFunc)();

struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
    GetPluginFunc initializeFunc;
};

#define UCL_PLUGIN(classType, managerName, managerVersion)          \
  extern "C" {                                                      \
      UCL_PLUGIN_EXPORT ucl::plugins::UCLPluginIf* getPlugin()      \
      {                                                             \
          static classType singleton;                               \
          return &singleton;                                        \
      }                                                             \
      UCL_PLUGIN_EXPORT ucl::plugins::PluginDetails exportDetails = \
      {                                                             \
          UCL_PLUGIN_STANDART_INFO,                                 \
          #classType,                                               \
          managerName,                                              \
          managerVersion,                                           \
          getPlugin                                                 \
      };                                                            \
  }

}}
#endif // PLUGINSAPI_H
