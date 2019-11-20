#ifndef PACKETQUEUE_HPP
#define PACKETQUEUE_HPP

#include <mutex>
#include <list>
#include <queue>
#include "Packet.hpp"
#include "Connection.hpp"

// TODO Thread safety
class PacketQueue {
    // queue of packets
    typedef std::queue<Packet, std::list<Packet>> PQueue;
    
    // queue of sended codes
    typedef std::queue<int32_t, std::list<int32_t>> CQueue;
    
    PQueue sendQueue;
    PQueue receiveQueue;
    
    CQueue codesQueue;
    Packet receivingPacket;
    
    PacketQueue() {}
    PacketQueue(const PacketQueue &) = delete;
    PacketQueue(Connection &&) = delete;
    PacketQueue & operator=(const PacketQueue &) = delete;
    PacketQueue & operator=(PacketQueue &&) = delete;
    
    void sendUpdate();
    void trySend();
    void tryReceive();
public:
    void update();
    void sendPacket(Packet);
    bool anyReceived();
    
    static PacketQueue & instance();
    
    // receivePackets valid if only anyReceived 
    // is true. If anyReceived returns false
    // behaviour is undefined
    
    std::pair<Packet, int32_t> receivePacket();
    
    // wait only for send packets
    void waitSending();
    
    // wait ALL answers
    void wait();
};

#endif // PACKETQUEUE_HPP
