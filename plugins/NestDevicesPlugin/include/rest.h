#ifndef REST_CLASS_H
#define REST_CLASS_H

#include <string>

using namespace std;

#define REST_METHOD_PUT     "PUT"
#define REST_METHOD_GET     "GET"


class rest
{
public:
    rest();
    ~rest();

    bool setMethod(string method);
    void unsetMethod();
    bool setCertificate(string certfile);
    void unsetCertificate();
    bool setPrivatekey(string keyfile);
    void unsetPrivatekey();
    bool setNocheckCert(bool is_no_check);
    bool setDataBody(string data);
    void unsetDataBody();
    bool setHeader(string header);
    void unsetHeader();
    bool setConsoleOutputFile(string outputFile);
    void unsetConsoleOutputFile();
    bool setCloudResponseFile(string cloudResFile);
    void unsetCloudResponseFile();
    bool setUrl(string url);
    void unsetUrl();

    bool readFileContent(string& fileContent);
    string buildRest();
    string execute();

private:
    string rest_cmd;
    string method;
    string certificate;
    string privatekey;
    bool nocheck_cert;
    string data_body;
    string header;
    string outputFile;
    string cloudResFile;
    string url;

    void clean(string& value);
    bool set(string& dest, string value);
    bool addStrOption(string value, string header);
};

#endif //REST_CLASS_H
