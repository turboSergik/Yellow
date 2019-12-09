#include "PacketQueue.hpp"
#include "Connection.hpp"
#include <iostream>

bool PacketQueue::trySend() {
    Connection & connection = Connection::instance();
    size_t sended;
    size_t remained = sendQueue.front().remained();
    connection.send(sendQueue.front().getPtr(), remained, sended);
    if (sended == remained) {
        sendQueue.pop();
        return true;
    } else {
        sendQueue.front().process(sended);
        return false;
    }
}

bool PacketQueue::tryReceive() {
    Connection & connection = Connection::instance();
    if (receivingPacket.getProcessed() < 8) {
        size_t remained = 8 - receivingPacket.getProcessed();
        size_t received;
        connection.receive(receivingPacket.getPtr(), remained, received);
        if (remained == received) {
            if (receivingPacket.getJsonSize() == 0) {
                receiveQueue.push(std::move(receivingPacket));
                receivingPacket = Packet();
                return true;
            } else {
                receivingPacket = Packet(receivingPacket.message.get());
            }
        } else {
            receivingPacket.process(received);
        }
        return false;
    } else {
        size_t remained = receivingPacket.remained();
        size_t received;
        connection.receive(receivingPacket.getPtr(), remained, received);
        if (received == remained) {
            receiveQueue.push(std::move(receivingPacket));
            receivingPacket = Packet();
            return true;
        } else {
            receivingPacket.process(received);
            return false;
        }
    }
}

void PacketQueue::update() {
    if (sending) {
        // this means if not empty return !trySend()
        sending = sendQueue.empty() || !trySend();
    } else {
        sending = tryReceive();
    }
}

void PacketQueue::sendPacket(Packet packet) {
    sendQueue.push(packet);
    sendedQueue.push(std::move(packet));

}

bool PacketQueue::hasReceived() {
    return !receiveQueue.empty();
}

PacketQueue & PacketQueue::instance() {
    static PacketQueue singleton;
    return singleton;
}

std::pair<Packet, Packet> PacketQueue::receivePacket() {
    Packet receivedPacket = std::move(receiveQueue.front());
    receiveQueue.pop();
    Packet sendedPacket = std::move(sendedQueue.front());
    sendedQueue.pop();
    return std::make_pair(std::move(sendedPacket), std::move(receivedPacket));
}

void PacketQueue::wait() {
    while (sendedQueue.size() != receiveQueue.size()) {
        update();
    }
}
