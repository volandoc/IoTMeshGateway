#include "rest.h"
#include <cstring>
#include <iostream>

#define REST_CMD_HEADER             "wget"
#define REST_OPTION_METHOD          " --method="
#define REST_OPTION_CERTIFICATE     " --certificate="
#define REST_OPTION_PRIVATE_KEY     " --private-key="
#define REST_OPTION_NO_CHECK_CERT   " --no-check-certificate"
#define REST_OPTION_BODY_DATA       " --body-data="
#define REST_OPTION_HEADER          " --header="
#define REST_OPTION_URL             " "
#define REST_ABSENT_OPTION          ""

rest::rest()
    : rest_cmd(0)
    , method(0)
    , certificate(0)
    , privatekey(0)
    , nocheck_cert(false)
    , data_body(0)
    , header(0)
    , url(0)
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

bool rest::set_method(char* method)
{
    return set(this->method, method);
}

void rest::unset_method()
{
    clean(this->method);
}

bool rest::set_certificate(char* certfile)
{
    return set(this->certificate, certfile);
}
void rest::unset_certificate()
{
    clean(this->certificate);
}

bool rest::set_privatekey(char* keyfile)
{
    return set(this->privatekey, keyfile);
}
void rest::unset_privatekey()
{
    clean(this->privatekey);
}

bool rest::set_data_body(char* data)
{
    return set(this->data_body, data);
}
void rest::unset_data_body()
{
    clean(this->data_body);
}

bool rest::set_header(char* header)
{
    return set(this->header, header);
}
void rest::unset_header()
{
    clean(this->header);
}

bool rest::set_url(char* url)
{
    return set(this->url, url);
}
void rest::unset_url()
{
    clean(this->url);
}


bool rest::set_nocheck_cert(bool is_no_check)
{
    this->nocheck_cert = is_no_check;
    return true;
}

char* rest::build_rest()
{
    clean(this->rest_cmd);

    unsigned int len = 0;

    len += strlen(REST_CMD_HEADER);

    if (0 != this->method)
    {
        len += strlen(REST_OPTION_METHOD);
        len += strlen(this->method);
    }

    if (0 != this->certificate)
    {
        len += strlen(REST_OPTION_CERTIFICATE);
        len += strlen(this->certificate);
    }

    if (0 != this->privatekey)
    {
        len += strlen(REST_OPTION_PRIVATE_KEY);
        len += strlen(this->privatekey);
    }

    if (false != this->nocheck_cert)
    {
        len += strlen(REST_OPTION_NO_CHECK_CERT);
    }

    if (0 != this->data_body)
    {
        len += strlen(REST_OPTION_BODY_DATA);
        len += strlen(this->data_body);
    }

    if (0 != this->header)
    {
        len += strlen(REST_OPTION_HEADER);
        len += strlen(this->header);
    }

    if (0 != this->url)
    {
        len += strlen(REST_OPTION_URL);
        len += strlen(this->url);
    }

    this->rest_cmd = new char[len + 1];

    snprintf(this->rest_cmd, len, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
                REST_CMD_HEADER,
                (0 != this->method      ? REST_OPTION_METHOD        : REST_ABSENT_OPTION),
                (0 != this->method      ? this->method              : REST_ABSENT_OPTION),
                (0 != this->certificate ? REST_OPTION_CERTIFICATE   : REST_ABSENT_OPTION),
                (0 != this->certificate ? this->certificate         : REST_ABSENT_OPTION),
                (0 != this->privatekey  ? REST_OPTION_PRIVATE_KEY   : REST_ABSENT_OPTION),
                (0 != this->privatekey  ? this->privatekey          : REST_ABSENT_OPTION),
                (true == this->nocheck_cert ? REST_OPTION_NO_CHECK_CERT : REST_ABSENT_OPTION),
                (0 != this->data_body   ? REST_OPTION_BODY_DATA     : REST_ABSENT_OPTION),
                (0 != this->data_body   ? this->data_body           : REST_ABSENT_OPTION),
                (0 != this->header      ? REST_OPTION_HEADER        : REST_ABSENT_OPTION),
                (0 != this->header      ? this->header              : REST_ABSENT_OPTION),
                (0 != this->url         ? REST_OPTION_URL           : REST_ABSENT_OPTION),
                (0 != this->url         ? this->url                 : REST_ABSENT_OPTION));

    return rest_cmd;
}


void rest::clean(char*& value)
{
    if (0 != value)
    {
        delete[] value;
        value = 0;
    }
}

bool rest::set(char*& dest, char* value)
{
    bool ret_val = true;
    clean(dest);

    unsigned int len = 0;
    if (0 != value)
    {
        try
        {
            len = strlen(value);
            dest = new char[len + 1];
            memcpy(dest, value, len + 1);
        }
        catch(std::bad_alloc)
        {
            ret_val = false;
        }
    }

    return ret_val;
}



