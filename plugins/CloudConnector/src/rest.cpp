#include "rest.h"
#include <iostream>

#define REST_CMD_HEADER             "wget"
#define REST_OPTION_METHOD          " --method="
#define REST_OPTION_CERTIFICATE     " --certificate="
#define REST_OPTION_PRIVATE_KEY     " --private-key="
#define REST_OPTION_NO_CHECK_CERT   " --no-check-certificate"
#define REST_OPTION_BODY_DATA       " --body-data="
#define REST_OPTION_HEADER          " --header="
#define REST_OPTION_OUTPUT_FILE     " -o"
#define REST_OPTION_GW_ID_FILE      " --output-document="
#define REST_OPTION_URL             " "


rest::rest()
    : rest_cmd()
    , method()
    , certificate()
    , privatekey()
    , nocheck_cert(false)
    , data_body()
    , header()
    , url()
{
}

rest::~rest()
{
    clean(this->rest_cmd);
    clean(this->method);
    clean(this->certificate);
    clean(this->privatekey);
    clean(this->data_body);
    clean(this->header);
    clean(this->url);
}

bool rest::setMethod(string method)
{
    return set(this->method, method);
}

void rest::unsetMethod()
{
    clean(this->method);
}

bool rest::setCertificate(string certfile)
{
    return set(this->certificate, certfile);
}
void rest::unsetCertificate()
{
    clean(this->certificate);
}

bool rest::setPrivatekey(string keyfile)
{
    return set(this->privatekey, keyfile);
}
void rest::unsetPrivatekey()
{
    clean(this->privatekey);
}

bool rest::setDataBody(string data)
{
    return set(this->data_body, data);
}
void rest::unsetDataBody()
{
    clean(this->data_body);
}

bool rest::setHeader(string header)
{
    return set(this->header, header);
}
void rest::unsetHeader()
{
    clean(this->header);
}

bool rest::setConsoleOutputFile(string outputFile)
{
    return set(this->outputFile, outputFile);
}
void rest::unsetConsoleOutputFile()
{
    clean(this->outputFile);
}

bool rest::setCloudResponseFile(string cloudResFile)
{
    return set(this->cloudResFile, cloudResFile);
}
void rest::unsetCloudResponseFile()
{
    clean(this->cloudResFile);
}


bool rest::setUrl(string url)
{
    return set(this->url, url);
}
void rest::unsetUrl()
{
    clean(this->url);
}

bool rest::setNocheckCert(bool is_no_check)
{
    this->nocheck_cert = is_no_check;
    return true;
}

string rest::buildRest()
{
    clean(this->rest_cmd);

    this->rest_cmd = REST_CMD_HEADER;

    addStrOption(this->method, REST_OPTION_METHOD);
    addStrOption(this->certificate, REST_OPTION_CERTIFICATE);
    addStrOption(this->privatekey, REST_OPTION_PRIVATE_KEY);

    if (false != this->nocheck_cert)
    {
        this->rest_cmd += REST_OPTION_NO_CHECK_CERT;
    }

    addStrOption(this->data_body, REST_OPTION_BODY_DATA);
    addStrOption(this->header, REST_OPTION_HEADER);
    addStrOption(this->outputFile, REST_OPTION_OUTPUT_FILE);
    addStrOption(this->cloudResFile, REST_OPTION_GW_ID_FILE);
    addStrOption(this->url, REST_OPTION_URL);

    return rest_cmd;
}


void rest::clean(string& value)
{
    value.clear();
}

bool rest::set(string& dest, string value)
{
    clean(dest);

    if (0 != value.length())
    {
        dest = value;
        return true;
    }

    return false;
}

bool rest::addStrOption(string value, string header)
{
    if (0 != value.length())
    {
        this->rest_cmd += header;
        this->rest_cmd += value;
        return true;
    }
    return false;
}

