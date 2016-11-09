#ifndef DEVICEAPI_H
#define DEVICEAPI_H

#include <string>

#define _CAP_T_BOOL   0;
#define _CAP_T_INT    1;
#define _CAP_T_FLOAT  2;
#define _CAP_T_CHAR   3;
#define _CAP_T_STRING 4;

#define _CON_T_RANGE  0;
#define _CON_T_ENUM   1;
#define _CON_T_REGEXP 2;

#define _RW_T_READ    0;
#define _RW_T_WRITE   1;
#define _RW_T_BOTH    3;

static const char *CAPABILITY_TYPES[] = { "bool", "int", "float", "char", "string", "url" };
static const char *CONSTRAIN_TYPES[] = { "RANGE", "ENUM", "REGEXP" };
static const char *READWRITE_TYPES[] = { "READ", "WRITE", "BOTH" };

struct Capability {
    const char* name;
    const char* type;
    const char* constrain_type;
    const char* constraints_json;
    const char* default_value;
    const char* description;
    const char* read_write;
};

typedef std::list<Capability> CapabilitiesList;
typedef std::map<std::string, std::string> DeviceTypes;
typedef std::map<std::string, std::string>::iterator DeviceType;

#endif // DEVICEAPI_H
