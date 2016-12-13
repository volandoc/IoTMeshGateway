#ifndef IBMESSAGE_H
#define IBMESSAGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/String.h>
#include <Poco/Logger.h>


class IBPayload {
private:
    std::string type = "";
    std::string value = "";
    std::string cvalue = "";
    std::string content = "";
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
        Poco::Logger& logger = Poco::Logger::get("IBMessage");
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
            logger.error("JSON Exception: %s", excp.displayText());
            result = false;
        }

        return result;
    }
};

class IBMessage {
private:
    std::string id;
    IBPayload payload;
    std::string reference;
    std::time_t timestamp;
public:
    std::string getId(){ return id; }
    IBPayload getPayload(){ return payload; }
    std::string getReference(){ return reference; }
    std::time_t getTimestamp(){ return timestamp; }

    void setId(std::string id){ this->id = id; }
    void setPayload(IBPayload payload){ this->payload = payload; }
    void setReference(std::string reference){ this->reference = reference; }
    void setTimestamp(std::time_t timestamp){ this->timestamp = timestamp; }

    std::string toJSON() {
        Poco::JSON::Object msg;
        Poco::JSON::Object pload;
        std::stringstream strFormat;

        pload.set("type", payload.getType());
        pload.set("value", payload.getValue());
        pload.set("cvalue", payload.getCvalue());
        pload.set("content", payload.getContent());

        msg.set("id", id);
        msg.set("payload", pload);
        msg.set("reference", reference);
        msg.set("timestamp", timestamp);

        msg.stringify(strFormat);

        return strFormat.str();
    }

    bool fromJSON(std::string json_str) {
        Poco::Logger& logger = Poco::Logger::get("IBMessage");
        Poco::JSON::Parser parser;

        Poco::replaceInPlace(json_str,"\r","");
        Poco::replaceInPlace(json_str,"\n","");
        Poco::replaceInPlace(json_str,"\t","");
        std::string tmp;

        bool result = true;
        try {
            Poco::Dynamic::Var result = parser.parse(json_str);
            Poco::JSON::Object::Ptr pObject = result.extract<Poco::JSON::Object::Ptr>();
            Poco::DynamicStruct msg = *pObject;

            if( msg.contains("id") ) {
                this->id = msg["id"].convert<std::string>();
            } else {
                result = false;
            }

            if( msg.contains("payload") ) {
                tmp = msg["payload"].convert<std::string>();
                result = payload.fromJSON(tmp);
            } else {
                result = false;
            }

            if( msg.contains("reference") ) {
                this->reference = msg["reference"].convert<std::string>();
            } else {
                result = false;
            }

            if( msg.contains("timestamp") ) {
                this->timestamp = msg["timestamp"].convert<std::time_t>();
            } else {
                result = false;
            }
        } catch(Poco::Exception excp){
            logger.error("JSON Exception: %s", excp.displayText());
            result = false;
        }

        return result;
    }
};

#endif // IBMESSAGE_H
