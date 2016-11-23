#ifndef DISCOVERED_DEVICE_INFO_H
#define DISCOVERED_DEVICE_INFO_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"


class DiscoveredDevInfo {
private:
    std::string name;
    std::string description;
    std::string serial;
    std::string type;
public:
    std::string getName(){ return name; }
    std::string getDescription(){ return description; }
    std::string getSerial(){ return serial; }
    std::string getType(){ return type; }

    void setName(std::string name){ this->name = name; }
    void setDescription(std::string description){ this->description = description; }
    void setSerial(std::string serial){ this->serial = serial; }
    void setType(std::string type){ this->type = type; }

    std::string toJSON() {
        Poco::JSON::Object obj;
        std::stringstream strFormat;

        obj.set("name", name);
        obj.set("description", description);
        obj.set("serial", serial);
        obj.set("type", type);

        obj.stringify(strFormat);
        return strFormat.str();
    }

    bool fromJSON(std::string json_str) {
        Poco::JSON::Parser parser;
        bool result = true;
        try {
            Poco::Dynamic::Var result = parser.parse(json_str);
            Poco::JSON::Object::Ptr pObject = result.extract<Poco::JSON::Object::Ptr>();
            Poco::DynamicStruct ds = *pObject;

            if( ds.contains("name") ) {
                this->name = ds["name"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("description") ) {
                this->description = ds["description"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("serial") ) {
                this->serial = ds["serial"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("type") ) {
                this->type = ds["type"].convert<std::string>();
            } else {
                result = false;
            }
        } catch(Poco::Exception excp){
            result = false;
        }

        return result;
    }
};

#endif // DISCOVERED_DEVICE_INFO_H
