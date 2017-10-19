#include "messaging.h"

void LifxMessage::setPacketSource(Poco::UInt32 source){
    lifxPacket.source = source;
}

void LifxMessage::setPacketTarget(std::string mac){

}

void LifxMessage::setData(Poco::UInt8 *databuf, Poco::UInt16 size){
    lifxPacket.size = LifxHeaderSize + size;
    lifxPacket.data_size = size;
    for(int pos=0; pos<size; pos++){
        lifxPacket.data[pos] = databuf[pos];
    }
}

void LifxMessage::acknowledgeRequired(bool ack){
    if(ack){
        lifxPacket.ack_res_bits |= 2;
    } else {
        lifxPacket.ack_res_bits &= ~2;
    }
}

void LifxMessage::responseRequired(bool res){
    if(res){
        lifxPacket.ack_res_bits |= 1;
    } else {
        lifxPacket.ack_res_bits &= ~1;
    }
}

void LifxMessage::setPacketType(Poco::UInt16 type){
    lifxPacket.packet_type = type;
}

void LifxMessage::sendMessage(){
    Poco::Net::NetworkInterface ni = Poco::Net::NetworkInterface::forName(ifname);
    sendMessage(ni.broadcastAddress().toString());
}

void LifxMessage::sendMessage(std::string address){
    Poco::Logger& logger = Poco::Logger::get("LifXBulbPlugin");

    Poco::Timestamp now;
    lifxPacket.timestamp = now.epochMicroseconds();

    Poco::Buffer<char> messageBuffer(lifxPacket.size);
    Poco::BasicMemoryBinaryWriter<char> messageWriter(messageBuffer, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);

    messageWriter << lifxPacket.size << lifxPacket.protocol << lifxPacket.source;

    for(int i=0; i < 6; i++){
        messageWriter << lifxPacket.bulbAddress[i];
    }
    messageWriter << lifxPacket.bAddrResrved;

    for(int i=0; i < 6; i++){
        messageWriter << lifxPacket.site[i];
    }

    messageWriter << lifxPacket.ack_res_bits << lifxPacket.sequence
                  << lifxPacket.timestamp << lifxPacket.packet_type << lifxPacket.pckReserved;

    for(int i=0; i < lifxPacket.data_size; i++){
        messageWriter << lifxPacket.data[i];
    }

    size_t size = messageBuffer.sizeBytes();

    try {
        //Poco::Net::SocketAddress sa(address, LifxPort);
        socketUDP->sendBytes(messageBuffer.begin(), size);
    } catch(Poco::Exception excp) {
        logger.log(excp, __FILE__, 77);
    }

}
