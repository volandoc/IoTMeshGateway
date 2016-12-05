#include "messaging.h"

void LifxMessage::setPacketSize(Poco::UInt16 size){
    lifxPacket.size = size;
}

void LifxMessage::setPacketSource(Poco::UInt32 source){
    lifxPacket.source = source;
}

void LifxMessage::setPacketTarget(std::string mac){

}

void LifxMessage::setData(std::string mac){

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
    sendMessage("255.255.255.255");
}

void LifxMessage::sendMessage(std::string address){

    Poco::Timestamp now;
    lifxPacket.timestamp = now.epochMicroseconds();

    Poco::Buffer<char> messageBuffer(LifxHeaderSize+lifxPacket.data_size);
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
    std::cout << std::endl;
    for(int i=0; i < size; i++){
        std::cout << (short) messageBuffer[i] << " ";
    }
    std::cout << std::endl;

    Poco::Net::SocketAddress sa(address, LifxPort);
    Poco::Net::DatagramSocket dgs;
    dgs.connect(sa);
    dgs.sendBytes(messageBuffer.begin(), size);
}
