#ifndef MESSAGING_H
#define MESSAGING_H

#include <string>
#include <iostream>
#include <Poco/Types.h>
#include <Poco/Buffer.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Timestamp.h>
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"

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
    int data_size = 0;
};

const unsigned int LP_AllBulbsResponse = 21504; // 0x5400
const unsigned int LP_AllBulbsRequest  = 13312; // 0x3400
const unsigned int LP_BulbCommand      = 5120;  // 0x1400

const unsigned int LifxHeaderSize      = 36;
const unsigned int LifxPort            = 56700;  // local port to listen on

//PROTOCOLS
const Poco::UInt16 SERVICE_UDP = 0x01;
const Poco::UInt16 SERVICE_TCP = 0x02;

//packet types
const Poco::UInt16 GET_PAN_GATEWAY = 0x0002;
const Poco::UInt16 PAN_GATEWAY = 0x0003;

const Poco::UInt16 GET_WIFI_FIRMWARE_STATE = 0x0012;
const Poco::UInt16 WIFI_FIRMWARE_STATE = 0x0013;

const Poco::UInt16 GET_POWER_STATE = 0x0014;
const Poco::UInt16 GET_POWER_STATE2 = 0x0074;
const Poco::UInt16 SET_POWER_STATE = 0x0075;
const Poco::UInt16 SET_POWER_STATE2 = 0x0015;
const Poco::UInt16 POWER_STATE = 0x0016;
const Poco::UInt16 POWER_STATE2 = 0x0076;

const Poco::UInt16 GET_BULB_LABEL = 0x0017;
const Poco::UInt16 SET_BULB_LABEL = 0x0018;
const Poco::UInt16 BULB_LABEL = 0x0019;

const Poco::UInt16 GET_VERSION_STATE = 0x0020;
const Poco::UInt16 VERSION_STATE = 0x0021;

const Poco::UInt16 GET_BULB_TAGS = 0x001a;
const Poco::UInt16 SET_BULB_TAGS = 0x001b;
const Poco::UInt16 BULB_TAGS = 0x001c;

const Poco::UInt16 GET_BULB_TAG_LABELS = 0x001d;
const Poco::UInt16 SET_BULB_TAG_LABELS = 0x001e;
const Poco::UInt16 BULB_TAG_LABELS = 0x001f;

const Poco::UInt16 GET_LIGHT_STATE = 0x0065;
const Poco::UInt16 SET_LIGHT_STATE = 0x0066;
const Poco::UInt16 SET_WAVE_FORM = 0x0067;
const Poco::UInt16 LIGHT_STATUS = 0x006b;

class LifxMessage {
protected:
    LifxPacket lifxPacket;
public:
    LifxMessage(){}
    virtual ~LifxMessage(){}

    void setPacketSize(Poco::UInt16 size);
    void setPacketSource(Poco::UInt32 source);
    void setPacketTarget(std::string mac);
    void setData(std::string mac);
    void acknowledgeRequired(bool ack);
    void responseRequired(bool res);
    void setPacketType(Poco::UInt16 type);
    void sendMessage();
    void sendMessage(std::string address);

};



#endif // MESSAGING_H
