#ifndef REST_CLASS_H
#define REST_CLASS_H

class rest
{
public:
    rest();
    ~rest();

    bool set_method(char* method);
    void unset_method();
    bool set_certificate(char* certfile);
    void unset_certificate();
    bool set_privatekey(char* keyfile);
    void unset_privatekey();
    bool set_nocheck_cert(bool is_no_check);
    bool set_data_body(char* data);
    void unset_data_body();
    bool set_header(char* header);
    void unset_header();
    bool set_url(char* url);
    void unset_url();

    char* build_rest();

private:
    char* rest_cmd;
    char* method;
    char* certificate;
    char* privatekey;
    bool nocheck_cert;
    char* data_body;
    char* header;
    char* url;

    void clean(char*& value);
    bool set(char*& dest, char* value);
};

#endif //REST_CLASS_H
