#ifndef IBMESSAGE_H
#define IBMESSAGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"


class IBPayload {
private:
    std::string type;
    std::string value;
    std::string cvalue;
    std::string content;
public:
    std::string getType(){ return type; }
    std::string getValue(){ return value; }
    std::string getCvalue(){ return cvalue; }
    std::string getContent(){ return content; }

    void setType(std::string type){ this->type = type; }
    void setValue(std::string value){ this->value = value; }
    void setCvalue(std::string cvalue){ this->cvalue = cvalue; }
    void setContent(std::string content){ this->content = content; }

    std::string toJSON() {
        Poco::JSON::Object obj;
        std::stringstream strFormat;

        obj.set("type", type);
        obj.set("value", value);
        obj.set("cvalue", cvalue);
        obj.set("content", content);

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

            if( ds.contains("type") ) {
                this->type = ds["type"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("value") ) {
                this->value = ds["value"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("cvalue") ) {
                this->cvalue = ds["cvalue"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("content") ) {
                this->content = ds["content"].convert<std::string>();
            } else {
                result = false;
            }
        } catch(Poco::Exception excp){
            result = false;
        }

        return result;
    }
};

class IBMessage {
private:
    std::string id;
    std::string payload;
    std::string reference;
    std::time_t timestamp;
public:
    std::string getId(){ return id; }
    std::string getPayload(){ return payload; }
    std::string getReference(){ return reference; }
    std::time_t getTimestamp(){ return timestamp; }

    void setId(std::string id){ this->id = id; }
    void setPayload(std::string payload){ this->payload = payload; }
    void setReference(std::string reference){ this->reference = reference; }
    void setTimestamp(std::time_t timestamp){ this->timestamp = timestamp; }

    std::string toJSON() {
        Poco::JSON::Object obj;
        std::stringstream strFormat;

        obj.set("id", id);
        obj.set("payload", payload);
        obj.set("reference", reference);
        obj.set("timestamp", timestamp);

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

            if( ds.contains("id") ) {
                this->id = ds["id"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("payload") ) {
                this->payload = ds["payload"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("reference") ) {
                this->reference = ds["reference"].convert<std::string>();
            } else {
                result = false;
            }

            if( ds.contains("timestamp") ) {
                this->timestamp = ds["timestamp"].convert<std::time_t>();
            } else {
                result = false;
            }
        } catch(Poco::Exception excp){
            result = false;
        }

        return result;
    }
};

#endif // IBMESSAGE_H
