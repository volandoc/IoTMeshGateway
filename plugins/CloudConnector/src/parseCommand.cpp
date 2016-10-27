#include <iostream>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "parseCommand.h"


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco;

std::string parseCommand::get(std::string json, std::string key)
{
    std::string returnValue;

    try {
        Parser parser;
        Var result = parser.parse(json);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var verKey = object->get(key.c_str());
        returnValue = verKey.convert<std::string>();
    }
    catch (...)
    {
        returnValue = "";
    }
    return returnValue;
}

