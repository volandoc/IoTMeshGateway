#ifndef INNER_BUS_CLIENT_H
#define INNER_BUS_CLIENT_H

class InnerBusClientIF {
public:
    InnerBusClientIF(){}
    virtual ~InnerBusClientIF(){}
    virtual void init() = 0;
    virtual void free() = 0;
    virtual int connect() = 0;
    virtual int connect_async() = 0;
    virtual int disconnect() = 0;
    virtual int sendMessage(IBMessage message, std::string target="") = 0;
    virtual void setConfig(void *config) = 0;
    virtual void setListener(void *listener)=0;
    virtual void getInfo() = 0;
};

#endif //INNER_BUS_CLIENT_H
