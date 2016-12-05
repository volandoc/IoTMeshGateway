#ifndef CONNECTED_DEVICE_INFO_H
#define CONNECTED_DEVICE_INFO_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"


class ConnectedDevInfo {
private:
    std::string serial;
    std::string success;
    std::string errorMessage;
    Poco::JSON::Array propertiesArray;
public:
    std::string getSerial(){ return serial; }
    std::string getSuccess(){ return success; }
    std::string getErrorMessage(){ return errorMessage; }
    std::string getProperties(){
        std::stringstream strFormat;
        propertiesArray.stringify(strFormat);
        return strFormat.str();
    }

    void setSerial(std::string serial){ this->serial = serial; }
    void setSuccess(std::string success){ this->success = success; }
    void setErrorMessage(std::string errorMessage){ this->errorMessage = errorMessage; }
	void addProperty(int id, std::string value){
        Poco::JSON::Object property;
        property.set("id", id);
        property.set("value", value);
    }

    void clearProperties(){ this->propertiesArray.clear(); }

    std::string toJSON() {
        Poco::JSON::Object obj;
        std::stringstream strFormat;

        obj.set("serial", serial);
        obj.set("success", success);
        obj.set("errorMessage", errorMessage);
        obj.set("properties", propertiesArray);

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

            if( ds.contains("serial") ) {
                this->serial = ds["serial"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("success") ) {
                this->success = ds["success"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("errorMessage") ) {
                this->errorMessage = ds["errorMessage"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("properties") ) {
/*                Poco::JSON::Parser parser;
                Poco::Dynamic::Var result = parser.parse(ds["properties"]);
                this->propertiesArray =  = result.extract<Poco::JSON::Array>();*/
           } else {
                result = false;
            }
        } catch(Poco::Exception excp){
            result = false;
        }

        return result;
    }
};

#endif // CONNECTED_DEVICE_INFO_H
