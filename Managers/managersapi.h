#ifndef MANAGERSAPI_H
#define MANAGERSAPI_H
namespace gw {
namespace managers {

#define GW_MANAGER_API_VERSION 1

#ifdef WIN32
#  ifdef GW_MANAGERS_EXPORTS
#    define GW_MANAGER_EXPORT __declspec(dllexport)
#  else
#    define GW_MANAGER_EXPORT __declspec(dllimport)
#  endif
#else
#   define GW_MANAGER_EXPORT // empty
#endif

class GWManagerIf{
public:
    virtual int startManager()=0;
    virtual int stopManager()=0;

};

GW_MANAGER_EXPORT typedef GWManagerIf* (*GetManagerFunc)();

#define GW_MANAGER_STANDART_INFO \
    GW_MANAGER_API_VERSION,      \
    __FILE__

struct ManagerDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* managerName;
    const char* managerVersion;
    GetManagerFunc initializeFunc;
};

#define GW_MANAGER(classType, managerName, managerVersion)          \
  extern "C" {                                                      \
      GW_MANAGER_EXPORT gw::managers::GWManagerIf* getManager()\
      {                                                             \
          static classType singleton;                               \
          return &singleton;                                        \
      }                                                             \
      GW_MANAGER_EXPORT gw::managers::ManagerDetails exportDetails = \
      {                                                             \
          GW_MANAGER_STANDART_INFO,                                 \
          #classType,                                               \
          managerName,                                              \
          managerVersion,                                           \
          getManager,                                               \
      };                                                            \
  }

}}
#endif // MANAGERSAPI_H
