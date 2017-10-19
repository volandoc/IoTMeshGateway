#ifndef MESSAGING_H
#define MESSAGING_H

#include <string>
#include <iostream>
#include <Poco/Types.h>
#include <Poco/Logger.h>
#include <Poco/Buffer.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>


#define lowByte(w) ((Poco::UInt8) ((w) & 0xff))
#define highByte(w) ((Poco::UInt8) ((w) >> 8))

struct LifxPacket {
//PACKET HEADERS
    //MAIN HEADRE 64bit
    Poco::UInt16 size = 36;        // 16bit all message size in Little Endian order
    Poco::UInt16 protocol = 0x3400;    // 16bit - 00, 0x14; 00, 0x34;  00, 0x54; in Little Endian order
                          // 0011 0100 0000 0000 - 0x3400;
                          // 00 The documentation says that the first two bits here are the message origin indicator and must be zero.
                          // 1 bit represents the tagged field. We want this packet to be processed by all devices that receive it so we need to set it to one (1).
                          // 1 bit represents the addressable field. This indicates that the next header will be a frame address header. Since all of our frames require this it will always be set to 1
                          // 0100  0000 0000 12 bits are the protocol field. This must be set to 1024 which is 0100 0000 0000 in binary.
    Poco::UInt32 source = 0;      // 32bit The source identifier. If a non-zero value, then the LIFX device will send a unicast message
                          // to the source IP address and port that the client used to send the originating message.
                          // If the source identifier is a zero value, then the LIFX device may send a broadcast message
                          // that can be received by all clients on the same sub-net.
    //ADRESSES HEADER 128bit
    Poco::UInt8 bulbAddress[6] = {127,127,127,127,127,127};  //
    Poco::UInt16 bAddrResrved = 0;//
    Poco::UInt8 site[6] = {127,127,127,127,127,127};
    Poco::UInt8 ack_res_bits = 3;//
    Poco::UInt8 sequence = 0;//
    //MESSAGE HEADER
    Poco::UInt64 timestamp = 0;   // 64bit timestamp for message
    Poco::UInt16 packet_type = 0x0002; // 16bit packet type in Little Endian
    Poco::UInt16 pckReserved = 0; // 16bit resrved for packet identifier
//PACKET BODY
    Poco::UInt8 data[128] = {0};
    Poco::UInt16 data_size = 0;
};

const Poco::UInt16 LP_AllBulbsResponse = 21504; // 0x5400
const Poco::UInt16 LP_AllBulbsRequest  = 13312; // 0x3400
const Poco::UInt16 LP_BulbCommand      = 5120;  // 0x1400

const Poco::UInt16 LifxHeaderSize      = 36;
const unsigned int LifxPort            = 56700;  // local port to listen on

//PROTOCOLS
const Poco::UInt16 SERVICE_UDP = 0x01;
const Poco::UInt16 SERVICE_TCP = 0x02;

//packet types
const Poco::UInt16 GET_SERVICE = 0x0002;
const Poco::UInt16 STATE_SERVICE = 0x0003;

const Poco::UInt16 GET_TIME = 0x0004;
const Poco::UInt16 SET_TIME = 0x0005;
const Poco::UInt16 STATE_TIME = 0x0006;

const Poco::UInt16 GET_RESETSWITCH = 0x0007;
const Poco::UInt16 STATE_RESETSWITCH = 0x0008;

const Poco::UInt16 GET_HOST_INFO = 0x000c;
const Poco::UInt16 STATE_HOST_INFO = 0x000d;
const Poco::UInt16 GET_HOST_FIRMWARE = 0x000e;
const Poco::UInt16 STATE_HOST_FIRMWARE = 0x000f;

const Poco::UInt16 GET_WIFI_INFO = 0x0010;
const Poco::UInt16 STATE_WIFI_INFO = 0x0011;
const Poco::UInt16 GET_WIFI_FIRMWARE = 0x0012;
const Poco::UInt16 STATE_WIFI_FIRMWARE = 0x0013;

const Poco::UInt16 GET_POWER = 0x0014;
const Poco::UInt16 SET_POWER = 0x0015;
const Poco::UInt16 STATE_POWER = 0x0016;

const Poco::UInt16 GET_BULB_LABEL = 0x0017;
const Poco::UInt16 SET_BULB_LABEL = 0x0018;
const Poco::UInt16 STATE_BULB_LABEL = 0x0019;

const Poco::UInt16 GET_BULB_TAGS = 0x001a;
const Poco::UInt16 SET_BULB_TAGS = 0x001b;
const Poco::UInt16 STATE_BULB_TAGS = 0x001c;

const Poco::UInt16 GET_BULB_TAG_LABELS = 0x001d;
const Poco::UInt16 SET_BULB_TAG_LABELS = 0x001e;
const Poco::UInt16 STATE_BULB_TAG_LABELS = 0x001f;

const Poco::UInt16 GET_VERSION = 0x0020;
const Poco::UInt16 STATE_VERSION = 0x0021;

const Poco::UInt16 GET_RUNINFO = 0x0022;
const Poco::UInt16 STATE_RUNINFO = 0x0023;

const Poco::UInt16 GET_MCU_VOLTAGE = 0x0024;
const Poco::UInt16 STATE_MCU_VOLTAGE = 0x0025;

const Poco::UInt16 _REBOOT_ = 0x0026;
const Poco::UInt16 ENABLE_TEST_MODE = 0x0027;
const Poco::UInt16 DISABLE_TEST_MODE = 0x0028;

const Poco::UInt16 _ACKNOWLEDGEMENT_ = 0x002d;

const Poco::UInt16 GET_LOCATION = 0x0030;
const Poco::UInt16 SET_LOCATION = 0x0031;
const Poco::UInt16 STATE_LOCATION = 0x0032;

const Poco::UInt16 GET_GROUP = 0x0033;
const Poco::UInt16 SET_GROUP = 0x0034;
const Poco::UInt16 STATE_GROUP = 0x0035;

const Poco::UInt16 ECHO_REQUEST = 0x003a;
const Poco::UInt16 ECHO_RESPONSE = 0x003b;

const Poco::UInt16 GET_LIGHT_STATE = 0x0065;
const Poco::UInt16 SET_LIGHT_COLOR_HSBK = 0x0066;
const Poco::UInt16 SET_WAVE_FORM = 0x0067;
const Poco::UInt16 SET_DIM_TO = 0x0068;
const Poco::UInt16 SET_DIM_FROM = 0x0069;
const Poco::UInt16 SET_LIGHT_COLOR_RGB = 0x006a;
const Poco::UInt16 STATE_LIGHT = 0x006b;
const Poco::UInt16 SET_TEMPERATURE = 0x006e;
const Poco::UInt16 STATE_TEMPERATURE = 0x006f;
const Poco::UInt16 GET_POWER2 = 0x0074;
const Poco::UInt16 SET_POWER2 = 0x0075;
const Poco::UInt16 STATE_POWER2 = 0x0076;




class LifxMessage {
private:
    Poco::Net::DatagramSocket* socketUDP=nullptr;
    std::string ifname;
protected:
    LifxPacket lifxPacket;

    void setPacketSource(Poco::UInt32 source);
    void setPacketTarget(std::string mac);
    void setData(Poco::UInt8 *databuf, Poco::UInt16 size);
    void acknowledgeRequired(bool ack);
    void responseRequired(bool res);
    void setPacketType(Poco::UInt16 type);
public:
    LifxMessage(){}
    virtual ~LifxMessage(){}


    void setSocket(Poco::Net::DatagramSocket* socket){
        socketUDP = socket;
    }

    void setNetwork(std::string iname){
        ifname = iname;
    }

    void sendMessage();
    void sendMessage(std::string address);
    virtual void formatData()=0;
};

class GetServiceMessage: public LifxMessage {
public:
    GetServiceMessage(){
        setPacketType(GET_SERVICE);
        acknowledgeRequired(false);
        responseRequired(true);
    }
    virtual ~GetServiceMessage(){}
    virtual void formatData(){}
};

class GetPowerMessage: public LifxMessage {
public:
    GetPowerMessage(){
        setPacketType(GET_POWER);
        acknowledgeRequired(false);
        responseRequired(true);

    }
    virtual ~GetPowerMessage(){}
    virtual void formatData(){}
};

class SetPowerMessage: public LifxMessage {
private:
    Poco::UInt16 power;
public:
    SetPowerMessage(Poco::UInt16 pwr):power(pwr){
        setPacketType(SET_POWER);
        acknowledgeRequired(false);
        responseRequired(true);
        this->formatData();
    }
    virtual ~SetPowerMessage(){}

    virtual void formatData(){
        Poco::UInt8 part[2];
        part[0] = lowByte(power);
        part[1] = highByte(power);
        setData(part, 2);
    }
};

class LifxMessageFactory {
private:
    Poco::Net::NetworkInterface netInterface;
    Poco::Net::DatagramSocket socket;
    std::string ifname;
public:

    LifxMessageFactory(std::string iname): ifname(iname) {
        netInterface = Poco::Net::NetworkInterface::forName(iname);
        Poco::Net::SocketAddress sa(netInterface.broadcastAddress(), LifxPort);
        socket = Poco::Net::DatagramSocket(sa);
        socket.setBroadcast(true);
        socket.setReceiveTimeout(500000);
        socket.connect(sa);
    }

    ~LifxMessageFactory(){
        socket.close();
    }

    Poco::Net::DatagramSocket* getUDPSocket(){
        return &socket;
    }

    LifxMessage* getMessage(Poco::UInt16 type){
        return getMessage(type, 0, 0, 0, 0);
    }

    LifxMessage* getMessage(Poco::UInt16 type, Poco::UInt16 arg1){
        return getMessage(type, arg1, 0, 0, 0);
    }

    LifxMessage* getMessage(Poco::UInt16 type, Poco::UInt16 arg1, Poco::UInt16 arg2){
        return getMessage(type, arg1, arg2, 0, 0);
    }

    LifxMessage* getMessage(Poco::UInt16 type, Poco::UInt16 arg1, Poco::UInt16 arg2, Poco::UInt16 arg3){
        return getMessage(type, arg1, arg2, arg3, 0);
    }

    LifxMessage* getMessage(Poco::UInt16 type, Poco::UInt16 arg1, Poco::UInt16 arg2, Poco::UInt16 arg3, Poco::UInt16 arg4){
        Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");
        logger.debug("Creating (%d) message", (int) type);
        LifxMessage* tmpMessage;

        switch(type){
            case GET_SERVICE: tmpMessage = new GetServiceMessage(); break;
            case GET_POWER:   tmpMessage = new GetPowerMessage(); break;
            case SET_POWER:   tmpMessage = new SetPowerMessage(arg1); break;
            default: tmpMessage = nullptr;
        }
        tmpMessage->setNetwork(ifname);
        tmpMessage->setSocket(&socket);

        return tmpMessage;
    }
};

#endif // MESSAGING_H
